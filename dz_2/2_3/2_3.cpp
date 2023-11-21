#include <assert.h>
// #include <fstream>
#include <iostream>
#include <sstream>

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
    // test_algorithm();
    // test_table();
    run(std::cin, std::cout);

    return 0;
}

// ----------------РЕШЕНИЕ------------------------

// --------------Класс----------------------------

//------------------------------------------------

void run(std::istream& in, std::ostream& out) {
    char command = '\0';
    std::string value;
}

void test_algorithm() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "+ hello\n";

        run(in, out);

        assert(out.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");

        std::cout << "OK" << std::endl;
    }
}

// --------------Описание класса ------------

//---------------------------------------------
