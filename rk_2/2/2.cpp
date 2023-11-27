#include <iostream>
#include <queue>
#include <stack>

/*
    2. Кратчайшее расстояние от корня до листа

    Напишите функцию, вычисляющую кратчайшее расстояние от корня дерева до
    листа.
    Значения - int.

    По очереди вводятся узлы бинарного дерева в порядке
    вставки.

    Вывести самое короткое расстояние от корня до листа.
*/

class BinaryTree {
    struct Node {
        Node(const int& value) : value(value), left(nullptr), right(nullptr) {}

        Node* left;
        Node* right;
        int value;
    };

   public:
    BinaryTree() : root(nullptr) {}
    BinaryTree(const int& root) : root(new Node(root)){};
    ~BinaryTree();

    void Add(const int&);
    size_t GetMinHeight();

   private:
    Node* root;
};

int main() {
    BinaryTree tree;
    int value = 0;

    while (std::cin >> value) {
        tree.Add(value);
    }

    std::cout << tree.GetMinHeight() << std::endl;

    return 0;
}

//-------------Описание класса------------------

void BinaryTree::Add(const int& value) {
    if (!root) {
        root = new Node(value);

        return;
    }

    Node* current = root;

    while (current) {
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

size_t BinaryTree::GetMinHeight() {
    std::queue<Node*> queue;
    queue.push(root);

    Node* current_node = nullptr;
    size_t layer_width = 0;
    size_t min_height = 0;
    bool is_leaf = false;

    while (!queue.empty() && !is_leaf) {
        min_height++;
        layer_width = queue.size();

        while (layer_width--) {
            current_node = queue.front();
            queue.pop();

            if (!current_node->left && !current_node->right) {
                is_leaf = true;

                continue;
            }

            if (current_node->left) {
                queue.push(current_node->left);
            }

            if (current_node->right) {
                queue.push(current_node->right);
            }
        }
    }

    return min_height;
}

BinaryTree::~BinaryTree() {
    std::stack<Node*> stack;
    Node* current_node = root;
    Node* deleted_node = nullptr;

    while (!stack.empty() || current_node) {
        while (current_node) {
            stack.push(current_node);
            current_node = current_node->left;
        }

        deleted_node = stack.top();
        stack.pop();
        current_node = deleted_node->right;

        delete deleted_node;
    }
}