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

template <typename T, typename Cmp = std::less<T>>
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
    AvlTree() : root_(nullptr) {}

    ~AvlTree() { destroyTree_(root_); }

    void add(const T& data) { root_ = addInternal_(root_, data); }

    size_t findInsertedPos(const T& key) {
        return findInsertedPos_(root_, key);
    }

    bool has(const T& data) {
        Node* tmp = root_;
        while (tmp) {
            if (tmp->data == data) {
                return true;
            } else if (cmp_(tmp->data, data)) {
                tmp = tmp->right;
            } else {
                tmp = tmp->left;
            }
        }
        return false;
    }

    void erase(const T& data) { root_ = deleteInternal_(root_, data); }

   private:
    size_t findInsertedPos_(Node* node, const T& key) {
        if (!node) {
            return 0;
        }
        if (cmp_(key, node->data)) {
            return findInsertedPos_(node->left, key);
        }
        return findInsertedPos_(node->right, key) + getCount_(node->left) + 1;
    }

    void destroyTree_(Node* node) {
        if (node) {
            destroyTree_(node->left);
            destroyTree_(node->right);
            delete node;
        }
    }

    Node* deleteInternal_(Node* node, T data) {
        if (!node) {
            return nullptr;
        }
        if (getCount_(node->left) > data) {
            node->left = deleteInternal_(node->left, data);
        } else if (getCount_(node->left) < data) {
            node->right =
                deleteInternal_(node->right, data - getCount_(node->left) - 1);
        } else {
            Node* left = node->left;
            Node* right = node->right;

            delete node;

            if (!right) {
                return left;
            }

            Node* newNode = nullptr;
            if (getHeight_(right) > getHeight_(left)) {
                Node* newRight = findAndRemoveMin_(right, newNode);
                newNode->left = left;
                newNode->right = newRight;
            } else {
                Node* newLeft = findAndRemoveMax_(left, newNode);
                newNode->left = newLeft;
                newNode->right = right;
            }

            return doBalance_(newNode);
        }
        return doBalance_(node);
    }

    Node* findAndRemoveMin_(Node* minNode, Node*& subtreeRoot) {
        if (!minNode->left) {
            subtreeRoot = minNode;
            return minNode->right;
        }
        minNode->left = findAndRemoveMin_(minNode->left, subtreeRoot);
        return doBalance_(minNode);
    }

    Node* findAndRemoveMax_(Node* maxNode, Node*& subtreeRoot) {
        if (!maxNode->right) {
            subtreeRoot = maxNode;
            return maxNode->left;
        }
        maxNode->right = findAndRemoveMax_(maxNode->right, subtreeRoot);
        return doBalance_(maxNode);
    }

    Node* addInternal_(Node* node, const T& data) {
        if (!node) {
            return new Node(data);
        }

        if (!cmp_(data, node->data)) {
            node->right = addInternal_(node->right, data);
        } else {
            node->left = addInternal_(node->left, data);
        }

        return doBalance_(node);
    }

    size_t getHeight_(Node* node) { return node ? node->height : 0; }

    void fixHeight_(Node* node) {
        node->height =
            std::max(getHeight_(node->left), getHeight_(node->right)) + 1;
    }

    size_t getCount_(Node* node) { return node ? node->count : 0; }

    void fixCount_(Node* node) {
        node->count = getCount_(node->left) + getCount_(node->right) + 1;
    }

    int getBalance_(Node* node) {
        return getHeight_(node->right) - getHeight_(node->left);
    }

    Node* rotateLeft_(Node* node) {
        Node* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight_(node);
        fixHeight_(tmp);
        fixCount_(node);
        fixCount_(tmp);
        return tmp;
    }

    Node* rotateRight_(Node* node) {
        Node* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight_(node);
        fixHeight_(tmp);
        fixCount_(node);
        fixCount_(tmp);
        return tmp;
    }

    Node* doBalance_(Node* node) {
        fixHeight_(node);
        fixCount_(node);
        switch (getBalance_(node)) {
            case 2: {
                if (getBalance_(node->right) < 0) {
                    node->right = rotateRight_(node->right);
                }
                return rotateLeft_(node);
            }
            case -2: {
                if (getBalance_(node->left) > 0) {
                    node->left = rotateLeft_(node->left);
                }
                return rotateRight_(node);
            }
            default:
                return node;
        }
    }

    Node* root_;
    Cmp cmp_;
};

int main() {
    size_t n = 0;
    size_t action = 0, data = 0;
    AvlTree<size_t, std::greater<>> tree;

    std::cin >> n;

    for (size_t i = 0; i < n; i++) {
        std::cin >> action >> data;
        if (action == 1) {
            std::cout << tree.findInsertedPos(data) << std::endl;
            tree.add(data);
        } else {
            tree.erase(data);
        }
    }

    return 0;
}

//----------Описание класса