#include <assert.h>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

/*
    Постройте B-дерево минимального порядка t и выведите его по слоям.
    В качестве ключа используются числа, лежащие в диапазоне [0..2^32-1]

    Требования:
    B-дерево должно быть реализовано в виде шаблонного класса.
    Решение должно поддерживать передачу функции сравнения снаружи.

    Сначала вводится минимальный порядок дерева t.
    Затем вводятся элементы дерева.

    Программа должна вывести B-дерево по слоям. Каждый слой на новой
    строке, элементы должны выводится в том порядке, в котором они лежат в
    узлах.
*/

void run(std::istream& in, std::ostream& out);
void test_algorithm();

int main(void) {
    // test_algorithm();
    run(std::cin, std::cout);

    return 0;
}

//------------------Класс-------------------------

template <typename Key, typename Cmp = std::less<Key>>
class BTree {
    struct Node {
        Node(bool leaf) : leaf(leaf) {}

        ~Node() {
            for (Node* child : children) {
                delete child;
            }
        }

        bool leaf;
        std::vector<Key> keys;  // t-1, 2t-1, в корне от 1 до 2t-1
        std::vector<Node*> children;  // 2t
    };

   public:
    BTree(size_t min_degree) : t(min_degree), root(nullptr) {}
    ~BTree() {
        if (root) {
            delete root;
        }
    }

    void Insert(const Key& key);
    bool Find(const Key& key) { return find_internal(root, key); }

    void OutputByLevels(void (*out)(const std::string&));

   private:
    bool is_node_full(Node* node) { return node->keys.size() == 2 * t - 1; }
    void split(Node* node, const size_t& index);
    void insert_non_full(Node* node, const Key& key);
    bool find_internal(Node* node, const Key& key);

    size_t t;
    Node* root;
    Cmp cmp;
};

void run(std::istream& in, std::ostream& out) {
    size_t t = 0;
    size_t value = 0;
    in >> t;
    BTree<size_t> tree(t);

    while (in >> value) {
        tree.Insert(value);
    }

    static std::ostream& OUT = out;

    tree.OutputByLevels([](const std::string& value) { OUT << value; });
}

//---------------Описание класса -----------------

template <typename Key, typename Cmp>
void BTree<Key, Cmp>::Insert(const Key& key) {
    if (!root) {
        root = new Node(true);
    }

    if (is_node_full(root)) {
        Node* newRoot = new Node(false);
        newRoot->children.push_back(root);
        root = newRoot;
        split(root, 0);
    }

    insert_non_full(root, key);
}

template <typename Key, typename Cmp>
void BTree<Key, Cmp>::OutputByLevels(void (*out)(const std::string&)) {
    std::queue<Node*> q;
    Node* currentNode = nullptr;
    q.push(root);
    size_t levelSize = 0;

    while (!q.empty()) {
        levelSize = q.size();

        while (levelSize--) {
            currentNode = q.front();
            q.pop();

            for (const auto& key : currentNode->keys) {
                out(std::to_string(key));
                out(" ");
            }

            for (const auto& child : currentNode->children) {
                q.push(child);
            }
        }
        out("\n");
    }
}

template <typename Key, typename Cmp>
void BTree<Key, Cmp>::split(Node* node, const size_t& index) {
    node->children.resize(node->children.size() + 1);
    node->keys.resize(node->keys.size() + 1);

    for (size_t i = node->children.size() - 1; i > index; i--) {
        node->children[i] = node->children[i - 1];
    }

    for (size_t i = node->keys.size() - 1; i > index; i--) {
        node->keys[i] = node->keys[i - 1];
    }

    Node* left = node->children[index];
    Node* right = new Node(left->leaf);

    node->keys[index] = left->keys[t - 1];
    node->children[index + 1] = right;

    for (size_t i = t; i < 2 * t - 1; i++) {
        right->keys.push_back(left->keys[i]);
    }

    left->keys.resize(t - 1);

    if (!left->leaf) {
        for (size_t i = t; i < 2 * t; i++) {
            right->children.push_back(left->children[i]);
        }

        left->children.resize(t);
    }
}

template <typename Key, typename Cmp>
void BTree<Key, Cmp>::insert_non_full(Node* node, const Key& key) {
    int pos = node->keys.size() - 1;

    if (node->leaf) {
        node->keys.resize(node->keys.size() + 1);

        while (pos >= 0 && cmp(key, node->keys[pos])) {
            node->keys[pos + 1] = node->keys[pos];
            pos--;
        }

        node->keys[pos + 1] = key;
    } else {
        while (pos >= 0 && cmp(key, node->keys[pos])) {
            pos--;
        }

        if (is_node_full(node->children[pos + 1])) {
            split(node, pos + 1);

            if (cmp(node->keys[pos + 1], key)) {
                pos++;
            }
        }
        insert_non_full(node->children[pos + 1], key);
    }
}

template <typename Key, typename Cmp>
bool BTree<Key, Cmp>::find_internal(Node* node, const Key& key) {
    int i = 0;
    while (i < node->keys.size() && key > node->keys[i])
        i++;

    if (i < node->keys.size() && key == node->keys[i])
        return true;
    else if (node->leaf)
        return false;
    else
        return find_internal(node->children[i], key);
}

//---------------Тестирование алгоритма---------------

void test_algorithm() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "2\n0 1 2 3 4 5 6 7 8 9";

        run(in, out);

        assert(out.str() == "3 \n1 5 7 \n0 2 4 6 8 9 \n");

        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;

        in << "4\n0 1 2 3 4 5 6 7 8 9\n";

        run(in, out);

        assert(out.str() == "3 \n0 1 2 4 5 6 7 8 9 \n");

        std::cout << "OK" << std::endl;
    }
}