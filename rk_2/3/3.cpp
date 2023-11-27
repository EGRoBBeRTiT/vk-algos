#include <iostream>
#include <unordered_set>

/*
    3. Хеш-таблица

    Нужно кастомизировать std::unordered_set для операций со структурой данных:
    struct Node {
        std::string product_name;
        int color;
        int size;
    };

    Каждая строка входных данных задает одну операцию над множеством.
    Запись операции состоит из типа операции и следующей за ним через пробел
    тройки значений структуры, над которой проводится операция. Тип операции –
    один из трех символов:
    + означает добавление данной строки в множество;
    - означает удаление строки из множества;
    ? означает проверку принадлежности данной строки множеству.
    При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в
    этом множестве.
    При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он
    присутствует в этом множестве.

*/

struct Node {
    Node() : color(0), size(0), product_name("") {}

    std::string product_name;
    int color;
    int size;
};

bool operator==(const Node& left, const Node& right) {
    return left.product_name == right.product_name &&
           left.color == right.color && left.size == right.size;
}

// Функция сопряжения Cantor
size_t pair_hash(const size_t& a, const size_t& b) {
    return 5 * (a + b) * (a + b + 1) + b;
}

size_t pair_hash(const size_t& a, const size_t& b, const size_t& c) {
    size_t hash_a_b = pair_hash(a, b);

    return pair_hash(hash_a_b, c);
}

template <class Key>
struct Hash {};

template <>
struct Hash<Node> {
    size_t operator()(const Node& node) const noexcept {
        size_t hash_1 = std::hash<std::string>{}(node.product_name);
        size_t hash_2 = std::hash<int>{}(node.color);
        size_t hash_3 = std::hash<int>{}(node.size);

        return pair_hash(hash_1, hash_2, hash_3);
    }
};

int main() {
    std::unordered_set<Node, Hash<Node>> table;
    char operation = ' ';
    Node node;

    while (std::cin >> operation) {
        std::cin >> node.product_name >> node.color >> node.size;

        bool result = false;

        switch (operation) {
            case '+':
                result = table.insert(node).second;
                break;
            case '?':
                result = table.count(node);
                break;
            case '-':
                result = table.erase(node);
                break;
            default:
                break;
        }

        std::cout << (result ? "OK" : "FAIL") << std::endl;
    }

    return 0;
}