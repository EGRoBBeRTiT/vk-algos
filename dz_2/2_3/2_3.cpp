#include <assert.h>
#include <iostream>
#include <sstream>
#include <stack>

/*
    2.3. Порядок обхода

    Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31]
    длиной N.
    Требуется построить бинарное дерево, заданное наивным порядком вставки.
    Т.е., при добавлении очередного числа K в дерево с корнем root, если
    root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое
    поддерево root.
    Требования: Рекурсия запрещена. Решение должно поддерживать
    передачу функции сравнения снаружи.

    Выведите элементы в порядке post-order (снизу вверх).
*/

void run(std::istream& in, std::ostream& out);
void test_algorithm();

int main(void) {
    test_algorithm();
    // run(std::cin, std::cout);

    return 0;
}

// ----------------РЕШЕНИЕ------------------------

// ------------Класс бинарного дерева------------
class BinaryTree {
    struct Node {
        Node(const long& value) : value(value), left(nullptr), right(nullptr) {}

        Node* left;
        Node* right;
        long value;
    };

   public:
    BinaryTree() : root(nullptr){};
    ~BinaryTree();

    void Add(const long&);
    void BypassPostOrder(void (*out)(const long&));

   private:
    Node* root;
};

//---------------Ввод данных---------------------

void run(std::istream& in, std::ostream& out) {
    BinaryTree tree;

    size_t n = 0;
    long value = 0;

    in >> n;

    for (size_t i = 0; i < n; i++) {
        in >> value;

        tree.Add(value);
    }

    static std::ostream& OUT = out;

    tree.BypassPostOrder([](const long& value) { OUT << value << " "; });
}

// --------------Описание класса ------------

void BinaryTree::Add(const long& value) {
    if (!root) {
        root = new Node(value);
        return;
    }

    Node* current = root;

    while (true) {
        if (current->value <= value && !current->right) {
            current->right = new Node(value);

            return;
        }
        if (current->value > value && !current->left) {
            current->left = new Node(value);

            return;
        }
        if (current->value <= value) {
            current = current->right;
        } else {
            current = current->left;
        }
    }
}

// destructor post order
BinaryTree::~BinaryTree() {
    std::stack<Node*> stack;
    Node* current_node = nullptr;
    Node* prev_node = nullptr;
    stack.push(root);

    while (!stack.empty()) {
        current_node = stack.top();

        if (current_node->left) {
            stack.push(current_node->left);

            continue;
        }

        if (current_node->right) {
            stack.push(current_node->right);

            continue;
        }

        prev_node = current_node;

        delete current_node;
        current_node = nullptr;
        stack.pop();

        if (!stack.empty()) {
            current_node = stack.top();

            if (prev_node == current_node->left) {
                current_node->left = nullptr;
            } else {
                current_node->right = nullptr;
            }
        }
    }

    root = nullptr;
}

void BinaryTree::BypassPostOrder(void (*out)(const long&)) {
    std::stack<Node*> stack;
    Node* current_node = root;
    Node* stack_node = nullptr;
    Node* prev_stack_node = nullptr;
    stack.push(current_node);

    while (!stack.empty()) {
        if (current_node->left) {
            stack.push(current_node->left);
            current_node = current_node->left;

            continue;
        }

        if (current_node->right) {
            stack.push(current_node->right);
            current_node = current_node->right;

            continue;
        }

        stack_node = stack.top();
        stack.pop();

        out(stack_node->value);

        if (!stack.empty()) {
            prev_stack_node = stack.top();

            if (prev_stack_node->left == stack_node && prev_stack_node->right) {
                stack.push(prev_stack_node->right);
                current_node = prev_stack_node->right;
            }
        }
    }
}

//---------------Тестирование алгоритма---------------

void test_algorithm() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "3\n2\n1\n3\n";

        run(in, out);

        assert(out.str() == "1 3 2 ");

        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;

        in << "3\n1\n2\n3\n";

        run(in, out);

        assert(out.str() == "3 2 1 ");

        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;

        in << "3\n3\n1\n2\n";

        run(in, out);

        assert(out.str() == "2 1 3 ");

        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;

        in << "10\n7\n2\n10\n8\n5\n3\n6\n4\n1\n9\n";

        run(in, out);

        assert(out.str() == "1 4 3 6 5 2 9 8 10 7 ");

        std::cout << "OK" << std::endl;
    }
}
