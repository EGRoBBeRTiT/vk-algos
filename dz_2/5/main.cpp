#include <math.h>
#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

typedef char byte;

class IInputStream {
   public:
    IInputStream(const std::string& file_name);
    ~IInputStream();

    // Возвращает false, если поток закончился
    bool Read(byte& value);
    void Close() {
        if (input.is_open()) {
            input.close();
        }
    };

   private:
    std::ifstream input;
};

class IOutputStream {
   public:
    IOutputStream(const std::string& file_name);
    ~IOutputStream();

    void Write(byte value);
    void Close() {
        if (output.is_open()) {
            output.close();
        }
    };

   private:
    std::ofstream output;
};

void Encode(IInputStream& original, IOutputStream& compressed);
void Decode(IInputStream& compressed, IOutputStream& original);

class BitWriter {
   public:
    BitWriter() : bitCount(0) {}

    void WriteBit(unsigned char bit) {
        if (bitCount % 8 == 0) {
            buffer.push_back(0);
        }
        if (bit == 1 | bit == '1') {
            buffer[bitCount / 8] |= 1 << (7 - bitCount % 8);
        }
        bitCount++;
    }

    void WriteByte(unsigned char byte) {
        if (bitCount % 8 == 0) {
            buffer.push_back(byte);
        } else {
            int offset = bitCount % 8;
            buffer[bitCount / 8] |= byte >> offset;
            buffer.push_back(byte << (8 - offset));
        }
        bitCount += 8;
    }

    const std::vector<unsigned char>& GetBuffer() const { return buffer; }

    size_t GetBitCount() const { return bitCount; }

   private:
    std::vector<unsigned char> buffer;
    size_t bitCount;
};

int main() {
    std::map<byte, std::vector<byte>> codes;

    IInputStream original("sample_1280×853.bmp");
    IOutputStream compressed_output("compressed.bmp");
    Encode(original, compressed_output);
    compressed_output.Close();

    IInputStream compressed_input("compressed.bmp");
    IOutputStream decoded("decoded.bmp");
    Decode(compressed_input, decoded);

    return 0;
}

// -----------------Интерфейсы-----------------------

IInputStream::IInputStream(const std::string& file_name) {
    input.open(file_name);
}

IInputStream::~IInputStream() {
    if (input.is_open()) {
        input.close();
    }
}

bool IInputStream::Read(byte& value) {
    if (input.is_open()) {
        value = input.get();

        if (input.eof()) {
            return false;

            input.close();
        }

        return true;
    }

    return false;
}

void IOutputStream::Write(byte value) {
    if (output.is_open()) {
        output << value;
    }

    return;
}

IOutputStream::IOutputStream(const std::string& file_name) {
    output.open(file_name);
}

IOutputStream::~IOutputStream() {
    if (output.is_open()) {
        output.close();
    }
}

static void copyStream(IInputStream& input, IOutputStream& output) {
    byte value;

    while (input.Read(value)) {
        output.Write(value);
    }

    return;
}

struct HuffmanNode {
    HuffmanNode(byte letter = *"", const size_t& freq = 0)
        : letter(letter), freq(freq), left(nullptr), right(nullptr){};
    HuffmanNode(HuffmanNode* left = nullptr, HuffmanNode* right = nullptr)
        : left(left), right(right), freq(0), letter(*"") {
        if (left && left->freq) {
            freq += left->freq;
        }
        if (right && right->freq) {
            freq += right->freq;
        }
    };

    HuffmanNode(const HuffmanNode& node) {
        left = node.left;
        right = node.right;

        freq = node.freq;
        letter = node.letter;
    };
    HuffmanNode& operator=(const HuffmanNode& node) {
        left = node.left;
        right = node.right;

        freq = node.freq;
        letter = node.letter;

        return *this;
    }

    HuffmanNode* left;
    HuffmanNode* right;
    size_t freq;
    byte letter;
};

class HuffmanTree {
   public:
    HuffmanTree() : root(nullptr), letters_count(0){};
    HuffmanTree(HuffmanNode* root) : root(root), letters_count(root->freq){};
    ~HuffmanTree(){};

    HuffmanNode* GetRoot() { return root; };
    void SetRoot(HuffmanNode* root) {
        if (root) {
            this->root = root;
            letters_count = root->freq;
        }
    };
    size_t GetLettersCount() { return letters_count; };

    size_t GetBitsCount() {
        std::vector<byte> code;
        size_t bits_count = 0;

        if (root && root->letter) {
            code.push_back(*"1");
        }

        if (root) {
            get_bits_count_internal(root, code, bits_count);
        }

        return bits_count;
    };

    void DFS(std::map<byte, std::vector<byte>>& map) {
        std::vector<byte> code;

        if (root && root->letter) {
            code.push_back(*"1");
        }

        if (root) {
            dfs_internal(root, code, map);
        }
    }

    void EncodeTree(BitWriter& bw) {
        if (root) {
            encode_tree_internal(root, bw);
        }
    };

   private:
    void dfs_internal(HuffmanNode* node,
                      std::vector<byte>& code,
                      std::map<byte, std::vector<byte>>& map) {
        if (node->left) {
            code.push_back(*"1");
            dfs_internal(node->left, code, map);
        }
        if (node->right) {
            code.push_back(*"0");
            dfs_internal(node->right, code, map);
        }

        if (!node->left && !node->right) {
            map.insert({node->letter, code});
        }

        code.pop_back();

        return;
    };

    void encode_tree_internal(HuffmanNode* node, BitWriter& bw) {
        if (!node->left && !node->right) {
            bw.WriteBit(1);
            bw.WriteByte(node->letter);

            return;
        }
        if (node->left) {
            encode_tree_internal(node->left, bw);
        }
        if (node->right) {
            encode_tree_internal(node->right, bw);
        }

        bw.WriteBit(0);
    };

    void get_bits_count_internal(HuffmanNode* node,
                                 std::vector<byte>& code,
                                 size_t& bits_count) {
        if (node->left) {
            code.push_back(*"1");
            get_bits_count_internal(node->left, code, bits_count);
        }
        if (node->right) {
            code.push_back(*"0");
            get_bits_count_internal(node->right, code, bits_count);
        }

        if (node->letter) {
            bits_count += (code.size()) * node->freq;
        }

        code.pop_back();

        return;
    }

    HuffmanNode* root;
    size_t letters_count;
};

bool huffman_node_cmp(HuffmanNode* left, HuffmanNode* right) {
    return left->freq > right->freq;
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    // Читаем и подсчитываем количество букв
    std::vector<byte> buffer;
    std::map<byte, int> map;

    byte value;

    while (original.Read(value)) {
        buffer.push_back(value);
        map.insert({value, map[value]++});
    }

    //--------------------------------------

    // Создание и заполнение min Heap

    std::vector<HuffmanNode*> nodes_heap;

    auto make_nodes_heap = [&]() {
        std::make_heap(nodes_heap.begin(), nodes_heap.end(), huffman_node_cmp);
    };
    auto pop_nodes_heap = [&]() {
        std::pop_heap(nodes_heap.begin(), nodes_heap.end(), huffman_node_cmp);

        HuffmanNode* node = nodes_heap.back();
        nodes_heap.pop_back();

        return node;
    };
    auto push_nodes_heap = [&](HuffmanNode* node) {
        nodes_heap.push_back(node);
        make_nodes_heap();
    };

    for (auto it = map.begin(); it != map.end(); it++) {
        nodes_heap.push_back(new HuffmanNode(it->first, it->second));
    }

    make_nodes_heap();

    //--------------------------------------

    // Построение дерева Хаффмана

    auto build_huffman_tree = [&]() {
        HuffmanTree huffman_tree;

        while (nodes_heap.size()) {
            HuffmanNode* node_left = pop_nodes_heap();

            if (!nodes_heap.size()) {
                huffman_tree.SetRoot(node_left);

                break;
            }

            HuffmanNode* node_right = pop_nodes_heap();

            push_nodes_heap(new HuffmanNode(node_left, node_right));
        }

        return huffman_tree;
    };

    HuffmanTree huffman_tree = build_huffman_tree();

    //--------------------------------------

    // Построение таблицы кодов букв

    std::map<byte, std::vector<byte>> compressed_map;

    huffman_tree.DFS(compressed_map);

    BitWriter bw;
    // Записываем количество букв алфавита
    bw.WriteByte(compressed_map.size());
    // Записываем закодированное дерево
    huffman_tree.EncodeTree(bw);
    // Записываем полезную длину последнего байта
    size_t bit_count = (bw.GetBitCount() + huffman_tree.GetBitsCount()) % 8;

    bw.WriteByte(bit_count);
    // Записываем само сообщение
    for (auto& letter : buffer) {
        auto code = compressed_map.find(letter);

        if (code != compressed_map.end()) {
            for (auto& bit : code->second) {
                bw.WriteBit(bit);
            }
        }
    }

    for (auto& byte : bw.GetBuffer()) {
        compressed.Write(byte);
    }

    return;
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    // ---------------------------------------

    byte value;
    std::string string_bit;
    size_t letters_count = 0;

    // Читаем количество букв в алфавите
    compressed.Read(value);
    letters_count = 0;
    std::string bits{};
    bits = std::bitset<8>(value).to_string();
    for (size_t i = 0; i < 8; ++i) {
        auto num = bits[i] == *"1" ? 1 : 0;

        letters_count += num * pow(2, 7 - i);
    }

    // Читаем дерево
    std::vector<byte> compressed_bits;

    std::string string_bits;

    while (compressed.Read(value)) {
        string_bits = std::bitset<8>(value).to_string();

        for (size_t i = 0; i < 8; i++) {
            compressed_bits.push_back(string_bits[i]);
        }
    }

    size_t read_letters_count = 0;
    HuffmanTree huffman_tree;
    size_t current_index = 0;
    std::stack<HuffmanNode*> stack;

    if (compressed_bits.size() && letters_count == 0) {
        letters_count = 256;
    }

    while ((read_letters_count < letters_count || !stack.empty()) &&
           current_index < compressed_bits.size()) {
        byte bit = compressed_bits[current_index];

        if (bit == *"1") {
            unsigned char letter = 0;

            for (short j = 1; j < 9; ++j) {
                auto num = compressed_bits[current_index + j] == *"1" ? 1 : 0;

                letter += num * pow(2, 8 - j);
            }

            current_index += 8;

            if (letters_count == 1) {
                huffman_tree.SetRoot(new HuffmanNode(letter));

                current_index++;
                break;
            }

            stack.push(new HuffmanNode(letter));
            read_letters_count++;
        }

        if (bit == *"0") {
            HuffmanNode* right = nullptr;

            if (!stack.empty()) {
                right = stack.top();
                stack.pop();
            }

            HuffmanNode* left = nullptr;

            if (!stack.empty()) {
                left = stack.top();
                stack.pop();
            }

            if (read_letters_count >= letters_count && stack.empty()) {
                huffman_tree.SetRoot(new HuffmanNode(left, right));

                current_index++;

                break;
            }

            stack.push(new HuffmanNode(left, right));
        }

        current_index++;
    }
    // -----------------------------------------
    // Составляем таблицу
    std::map<byte, std::vector<byte>> compressed_map;

    huffman_tree.DFS(compressed_map);

    // Меняю ключи и значения местами
    std::map<std::vector<byte>, byte> decoded_codes;

    for (auto it = compressed_map.begin(); it != compressed_map.end(); it++) {
        decoded_codes.insert({it->second, it->first});
    }

    //----------------------------

    // Считываем количество полезных символов
    size_t useful_bit_count = 0;
    for (size_t i = 0; i < 8; ++i) {
        auto num = compressed_bits[i + current_index] == *"1" ? 1 : 0;

        useful_bit_count += (num * pow(2, 7 - i));
    }

    if (useful_bit_count == 0) {
        useful_bit_count = 8;
    }

    current_index += 8;

    //-------------------------

    // Декодируем сообщение

    std::vector<byte> encoded;

    std::vector<byte> code;
    std::vector<byte> decoded;

    while (current_index < (compressed_bits.size() - (8 - useful_bit_count))) {
        code.push_back(compressed_bits[current_index]);

        auto letter = decoded_codes.find(code);

        if (letter != decoded_codes.end()) {
            decoded.push_back(letter->second);
            code.clear();
        }

        current_index++;
    }

    unsigned char null = 0;

    if (decoded.size() && decoded[decoded.size() - 1] == null) {
        decoded.push_back(null);
    }

    for (auto& letter : decoded) {
        original.Write(letter);
    }
}