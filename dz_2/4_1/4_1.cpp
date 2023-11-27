#include <iostream>

/*
    В одной военной части решили построить в одну шеренгу по росту. Т.к. часть
    была далеко не образцовая, то солдаты часто приходили не вовремя, а то их и
    вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. Однако
    солдаты в процессе прихода и ухода должны были всегда быть выстроены по
    росту – сначала самые высокие, а в конце – самые низкие. За расстановку
    солдат отвечал прапорщик, который заметил интересную особенность – все
    солдаты в части разного роста. Ваша задача состоит в том, чтобы помочь
    прапорщику правильно расставлять солдат, а именно для каждого приходящего
    солдата указывать, перед каким солдатом в строе он должен становится.

    Требования: скорость выполнения команды - O(log n).

    Формат входных данных.
        Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000).
    В каждой следующей строке содержится описание команды: число 1 и X если
    солдат приходит в строй (X – рост солдата, натуральное число до 100 000
    включительно) и число 2 и Y если солдата, стоящим в строе на месте Y надо
    удалить из строя. Солдаты в строе нумеруются с нуля.

    Формат выходных данных.
        На каждую команду 1 (добавление в строй) вы должны выводить число K –
    номер позиции, на которую должен встать этот солдат (все стоящие за ним
    двигаются назад).
*/

void run(std::istream& in, std::ostream& out);
void test_algorithm();

int main(void) {
    // test_algorithm();
    run(std::cin, std::cout);

    return 0;
}

// ----------------РЕШЕНИЕ------------------------

template <typename T>
struct DefaultComparator {
    int operator()(const T& l, const T& r) const {
        if (l < r)
            return -1;
        if (l > r)
            return 1;
        return 0;
    }
};
template <typename T, typename Cmp = DefaultComparator<T>>
class AvlTree {
    struct Node {
        Node(const T& data)
            : data(data), left(nullptr), right(nullptr), height(1), count(1) {}

        T data;
        Node* left;
        Node* right;
        size_t height;
        size_t count;
    };

   public:
    AvlTree(Cmp cmp = Cmp()) : root(nullptr), cmp(cmp) {}

    ~AvlTree() { destroy_tree(root); }

    void Add(const T& data) { root = add_internal(root, data); }

    size_t findInsertedPos(const T& key) {
        return find_inserted_pos(root, key);
    }

    bool Has(const T&);
    void Delete(const T& data) { root = delete_internal(root, data); }

   private:
    size_t find_inserted_pos(Node* node, const T& key);
    void destroy_tree(Node* node);
    Node* delete_internal(Node* node, T data);
    Node* find_and_remove_min(Node* minNode, Node*& subtreeRoot);
    Node* find_and_remove_max(Node* maxNode, Node*& subtreeRoot);
    Node* add_internal(Node* node, const T& data);
    size_t get_height(Node* node) { return node ? node->height : 0; }
    Node* rotate_left(Node* node);
    Node* rotate_right(Node* node);
    Node* do_balance(Node* node);

    void fix_height(Node* node) {
        node->height =
            std::max(get_height(node->left), get_height(node->right)) + 1;
    }

    size_t get_count(Node* node) { return node ? node->count : 0; }

    void fix_count(Node* node) {
        node->count = get_count(node->left) + get_count(node->right) + 1;
    }

    int get_balance(Node* node) {
        return get_height(node->right) - get_height(node->left);
    }

    Node* root;
    Cmp cmp;
};

//---------------Ввод данных---------------------

void run(std::istream& in, std::ostream& out) {
    size_t n = 0;
    size_t action = 0, data = 0;
    AvlTree<size_t, std::greater<>> tree;

    in >> n;

    for (size_t i = 0; i < n; i++) {
        in >> action >> data;
        if (action == 1) {
            out << tree.findInsertedPos(data) << std::endl;
            tree.Add(data);
        } else {
            tree.Delete(data);
        }
    }
}

// --------------Описание класса ------------
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
size_t AvlTree<T, Cmp>::find_inserted_pos(Node* node, const T& key) {
    if (!node) {
        return 0;
    }
    if (cmp(key, node->data)) {
        return find_inserted_pos(node->left, key);
    }
    return find_inserted_pos(node->right, key) + get_count(node->left) + 1;
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
    if (get_count(node->left) > data) {
        node->left = delete_internal(node->left, data);
    } else if (get_count(node->left) < data) {
        node->right =
            delete_internal(node->right, data - get_count(node->left) - 1);
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
    fix_count(node);
    fix_count(tmp);
    return tmp;
}

template <typename T, typename Cmp>
typename AvlTree<T, Cmp>::Node* AvlTree<T, Cmp>::rotate_right(Node* node) {
    Node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    fix_height(node);
    fix_height(tmp);
    fix_count(node);
    fix_count(tmp);
    return tmp;
}

template <typename T, typename Cmp>
typename AvlTree<T, Cmp>::Node* AvlTree<T, Cmp>::do_balance(Node* node) {
    fix_height(node);
    fix_count(node);

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