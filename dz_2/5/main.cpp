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

void visualizeBuffer(const std::vector<unsigned char>& buffer) {
    for (auto& b : buffer) {
        std::cout << std::bitset<8>(b) << "|";
    }
    std::cout << std::endl;
}

int main() {
    std::map<byte, std::vector<byte>> codes;

    IInputStream original("input.txt");
    IOutputStream compressed_output("compressed.txt");
    Encode(original, compressed_output);
    compressed_output.Close();

    IInputStream compressed_input("compressed.txt");
    IOutputStream decoded("decoded.txt");
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
    HuffmanNode(byte letter, const size_t& freq = 0)
        : letter(letter), freq(freq), left(nullptr), right(nullptr){};
    HuffmanNode(HuffmanNode* left, HuffmanNode* right)
        : left(left), right(right) {
        this->freq = left->freq + right->freq;
        letter = *"";
    };

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

        get_bits_count_internal(root, code, bits_count);

        return bits_count;
    };

    void DFS(std::map<byte, std::vector<byte>>& map) {
        std::vector<byte> code;

        dfs_internal(root, code, map);
    }

    void EncodeTree(BitWriter& bw) { encode_tree_internal(root, bw); };

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

        if (node->letter) {
            map.insert({node->letter, code});
        }

        code.pop_back();

        return;
    };

    void encode_tree_internal(HuffmanNode* node, BitWriter& bw) {
        if (node->left) {
            encode_tree_internal(node->left, bw);
        }
        if (node->right) {
            encode_tree_internal(node->right, bw);
        }

        if (node->letter) {
            bw.WriteBit(1);
            bw.WriteByte(node->letter);

        } else {
            bw.WriteBit(0);
        }
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

    std::cout << "\nКоличества повторений: \n";

    for (auto it = map.begin(); it != map.end(); it++) {
        nodes_heap.push_back(new HuffmanNode(it->first, it->second));

        std::cout << "'" << it->first << "': " << it->second << std::endl;
    }
    std::cout << "\n";

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

    std::cout << "\nКоды букв: \n";

    for (auto it = compressed_map.begin(); it != compressed_map.end(); it++) {
        std::cout << "'" << it->first << "': ";

        for (auto& code : it->second) {
            std::cout << code;
        }

        std::cout << std::endl;
    }

    BitWriter bw;
    // Записываем количество букв алфавита
    bw.WriteByte(compressed_map.size());
    // Записываем закодированное дерево
    huffman_tree.EncodeTree(bw);
    // Записываем полезную длину последнего байта
    size_t bit_count = (bw.GetBitCount() + huffman_tree.GetBitsCount()) % 8;
    std::cout << "\nКоличество полезных битов в последнем байте: " << bit_count
              << std::endl;
    bw.WriteByte(bit_count);
    // Записываем само сообщение
    for (auto& letter : buffer) {
        auto code = compressed_map.find(letter)->second;

        for (auto& bit : code) {
            bw.WriteBit(bit);
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

    // Читаем количество букв с алфавите
    compressed.Read(value);
    letters_count = value;

    // Читаем дерево
    std::vector<byte> compressed_bits;
    std::stack<HuffmanNode*> stack;

    std::string string_bits;

    while (compressed.Read(value)) {
        string_bits = std::bitset<8>(value).to_string();

        for (size_t i = 0; i < 8; i++) {
            compressed_bits.push_back(string_bits[i]);
        }
    }

    unsigned short read_letters_count = 0;
    HuffmanTree huffman_tree;
    size_t stop_index = 0;

    for (size_t i = 0; i < compressed_bits.size(); ++i) {
        byte bit = compressed_bits[i];

        if (bit == *"1") {
            unsigned char letter = 0;

            for (short j = 1; j < 9; ++j) {
                letter += ((compressed_bits[i + j] - '0') * pow(2, 8 - j));
            }

            stack.push(new HuffmanNode(letter));
            read_letters_count++;

            i += 8;
        }

        if (bit == *"0") {
            HuffmanNode* right = stack.top();
            stack.pop();
            HuffmanNode* left = stack.top();
            stack.pop();

            if (read_letters_count == letters_count && stack.empty()) {
                huffman_tree.SetRoot(new HuffmanNode(left, right));

                stop_index = i;

                break;
            }

            stack.push(new HuffmanNode(left, right));
        }
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

    std::cout << "\nРасшифрованная таблица:\n";

    for (auto it = decoded_codes.begin(); it != decoded_codes.end(); it++) {
        for (auto& letter : it->first) {
            std::cout << letter;
        }
        std::cout << ": " << it->second;

        std::cout << std::endl;
    }

    //----------------------------

    // Считываем количество полезных символов
    size_t useful_bit_count = 0;
    for (size_t i = 1; i < 9; ++i) {
        useful_bit_count +=
            ((compressed_bits[i + stop_index] - '0') * pow(2, 8 - i));
    }

    stop_index += 9;

    std::cout << "Количество полезных битов: " << useful_bit_count << " ";

    //-------------------------

    // Декодируем сообщение

    std::vector<byte> encoded;

    std::vector<byte> code;
    std::vector<byte> decoded;

    while (stop_index < compressed_bits.size() - 8 + useful_bit_count) {
        code.push_back(compressed_bits[stop_index]);

        auto letter = decoded_codes.find(code);

        if (letter != decoded_codes.end()) {
            decoded.push_back(letter->second);
            code.clear();
        }

        stop_index++;
    }

    std::cout << std::endl;

    for (auto& letter : decoded) {
        original.Write(letter);
        std::cout << letter;
    }

    std::cout << std::endl;
}