#include <iostream>

template <typename T, typename Cmp = std::less<T>>
class AvlTree {
    struct Node {
        Node(const T& data)
            : data(data), left(nullptr), right(nullptr), height(1) {}

        T data;
        Node* left;
        Node* right;
        size_t height;
    };

   public:
    AvlTree() : root(nullptr) {}
    ~AvlTree() { destroy_tree(root); }

    void Add(const T& data) { root = add_internal(root, data); }
    bool Has(const T&);
    void Delete(const T& data) { root = delete_internal(root, data); }

    std::string Next(const T&);
    std::string Prev(const T&);

   private:
    void destroy_tree(Node*);
    Node* delete_internal(Node*, T);
    Node* find_and_remove_min(Node*, Node*&);
    Node* find_and_remove_max(Node*, Node*&);
    Node* add_internal(Node*, const T&);
    size_t get_height(Node* node) { return node ? node->height : 0; }
    void fix_height(Node* node) {
        node->height =
            std::max(get_height(node->left), get_height(node->right)) + 1;
    }
    int get_balance(Node* node) {
        return get_height(node->right) - get_height(node->left);
    }
    Node* rotate_left(Node*);
    Node* rotate_right(Node*);
    Node* do_balance(Node*);

    Node* root;
    Cmp cmp;
};

int main() {
    AvlTree<int> tree;
    std::string operation = "";
    int value;

    while (std::cin >> operation >> value) {
        if (operation == "insert") {
            tree.Add(value);
            continue;
        }
        if (operation == "delete") {
            tree.Delete(value);
            continue;
        }
        if (operation == "exists") {
            std::cout << std::boolalpha << tree.Has(value) << std::endl;
            continue;
        }
        if (operation == "next") {
            std::cout << tree.Next(value) << std::endl;
            continue;
        }
        if (operation == "prev") {
            std::cout << tree.Prev(value) << std::endl;
            continue;
        }

        break;
    }
    return 0;
}

// ----------------Описание класса------------------

template <typename T, typename Cmp>
bool AvlTree<T, Cmp>::Has(const T& data) {
    Node* tmp = root;
    while (tmp) {
        if (tmp->data == data) {
            return true;
        } else if (cmp(tmp->data, data)) {
            tmp = tmp->right;
        } else {
            tmp = tmp->left;
        }
    }
    return false;
}

template <typename T, typename Cmp>
std::string AvlTree<T, Cmp>::Next(const T& data) {
    Node* currentNode = root;
    Node* parent = nullptr;

    while (currentNode) {
        if (currentNode->data > data) {
            parent = currentNode;
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }

    if (parent) {
        return std::to_string(parent->data);
    } else {
        return "none";
    }
}

template <typename T, typename Cmp>
std::string AvlTree<T, Cmp>::Prev(const T& data) {
    Node* currentNode = root;
    Node* parent = nullptr;

    while (currentNode) {
        if (currentNode->data < data) {
            parent = currentNode;
            currentNode = currentNode->right;
        } else {
            currentNode = currentNode->left;
        }
    }

    if (parent) {
        return std::to_string(parent->data);
    } else {
        return "none";
    }
}

template <typename T, typename Cmp>
void AvlTree<T, Cmp>::destroy_tree(Node* node) {
    if (node) {
        destroy_tree(node->left);
        destroy_tree(node->right);

        delete node;
    }
}

template <typename T, typename Cmp>
typename AvlTree<T, Cmp>::Node* AvlTree<T, Cmp>::delete_internal(Node* node,
                                                                 T data) {
    if (!node) {
        return nullptr;
    }
    if (cmp(node->data, data)) {
        node->right = delete_internal(node->right, data);
    } else if (cmp(data, node->data)) {
        node->left = delete_internal(node->left, data);
    } else {
        Node* left = node->left;
        Node* right = node->right;

        delete node;

        if (!right) {
            return left;
        }

        Node* newNode = nullptr;
        if (get_height(right) > get_height(left)) {
            Node* newRight = find_and_remove_min(right, newNode);
            newNode->left = left;
            newNode->right = newRight;
        } else {
            Node* newLeft = find_and_remove_max(left, newNode);
            newNode->left = newLeft;
            newNode->right = right;
        }

        return do_balance(newNode);
    }
    return do_balance(node);
}

template <typename T, typename Cmp>
typename AvlTree<T, Cmp>::Node* AvlTree<T, Cmp>::find_and_remove_min(
    Node* minNode,
    Node*& subtreeRoot) {
    if (!minNode->left) {
        subtreeRoot = minNode;
        return minNode->right;
    }
    minNode->left = find_and_remove_min(minNode->left, subtreeRoot);
    return do_balance(minNode);
}

template <typename T, typename Cmp>
typename AvlTree<T, Cmp>::Node* AvlTree<T, Cmp>::find_and_remove_max(
    Node* maxNode,
    Node*& subtreeRoot) {
    if (!maxNode->right) {
        subtreeRoot = maxNode;
        return maxNode->left;
    }
    maxNode->right = find_and_remove_max(maxNode->right, subtreeRoot);
    return do_balance(maxNode);
}

template <typename T, typename Cmp>
typename AvlTree<T, Cmp>::Node* AvlTree<T, Cmp>::add_internal(Node* node,
                                                              const T& data) {
    if (!node) {
        return new Node(data);
    }

    if (!cmp(data, node->data)) {
        node->right = add_internal(node->right, data);
    } else {
        node->left = add_internal(node->left, data);
    }

    return do_balance(node);
}

template <typename T, typename Cmp>
typename AvlTree<T, Cmp>::Node* AvlTree<T, Cmp>::rotate_left(Node* node) {
    Node* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    fix_height(node);
    fix_height(tmp);
    return tmp;
}

template <typename T, typename Cmp>
typename AvlTree<T, Cmp>::Node* AvlTree<T, Cmp>::rotate_right(Node* node) {
    Node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    fix_height(node);
    fix_height(tmp);
    return tmp;
}

template <typename T, typename Cmp>
typename AvlTree<T, Cmp>::Node* AvlTree<T, Cmp>::do_balance(Node* node) {
    fix_height(node);

    switch (get_balance(node)) {
        case 2: {
            if (get_balance(node->right) < 0) {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        }
        case -2: {
            if (get_balance(node->left) > 0) {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }
        default:
            return node;
    }
}