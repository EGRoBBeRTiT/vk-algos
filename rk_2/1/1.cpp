#include <iostream>
#include <stack>

/*
    1. Проверка дерева на одинаковые значения

    Напишите функцию, проверяющую, что во всех узлах бинарного дерева поиска
    хранится одинаковое значение.
    Значения - int.

    По очереди вводятся узлы бинарного дерева в порядке вставки.
    0 - если все значения в узлах бинарного дерева разные.
    1 - если все значения в узлах бинарного дерева одинаковые.
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
    ~BinaryTree();

    void Add(const int&);
    bool EqualityCheck();

   private:
    Node* root;
};

int main() {
    BinaryTree tree;
    int value = 0;

    while (std::cin >> value) {
        tree.Add(value);
    }

    std::cout << (tree.EqualityCheck() ? 1 : 0) << std::endl;

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

bool BinaryTree::EqualityCheck() {
    std::stack<Node*> stack;
    Node* currentNode = root;

    while (!stack.empty() || currentNode) {
        if (currentNode) {
            stack.push(currentNode);
            currentNode = currentNode->left;
        } else {
            currentNode = stack.top();
            stack.pop();
            if (currentNode->value != root->value) {
                return false;
            }
            currentNode = currentNode->right;
        }
    }

    return true;
}

BinaryTree::~BinaryTree() {
    std::stack<Node*> stack;
    Node* currentNode = root;
    Node* delNode = nullptr;

    while (!stack.empty() || currentNode) {
        while (currentNode) {
            stack.push(currentNode);
            currentNode = currentNode->left;
        }

        delNode = stack.top();
        stack.pop();
        currentNode = delNode->right;

        delete delNode;
    }
}