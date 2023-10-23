#include <assert.h>
#include <iostream>
#include <sstream>

/*
    Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых
    чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный
    индекс элемента массива A[k], ближайшего по значению к B[i].
    Требования:
    Время работы поиска для каждого элемента B[i]: O(log(k)).
    Внимание!
    В этой задаче для каждого B[i] сначала нужно определить диапазон для
    бинарного поиска размером порядка k с помощью экспоненциального поиска, а
    потом уже в нем делать бинарный поиск.
*/

void run(std::istream& in, std::ostream& out);
void test();

int main(void) {
    // test();
    run(std::cin, std::cout);

    return 0;
}

// -----------------РЕШЕНИЕ-------------------

int exponential_max_index_search(int* A,
                                 const int& length_A,
                                 const int& search_value) {
    if (length_A == 0 || length_A == 1) {
        return 0;
    }

    if (length_A == 2) {
        return 1;
    }

    if (search_value <= A[0]) {
        return 0;
    }

    int max_search_index = 1;

    while (max_search_index < length_A - 1 &&
           search_value > A[max_search_index]) {
        if (max_search_index * 2 > length_A - 1) {
            max_search_index = length_A - 1;
            break;
        }

        max_search_index *= 2;
    }

    return max_search_index;
}

int get_nearest_index(int* A,
                      const int& min,
                      const int& max,
                      const int& value) {
    int min_value_diff = value - A[min];
    int max_value_diff = A[max] - value;

    if (min_value_diff <= max_value_diff) {
        return min;
    }

    return max;
}

int binary_search(int* A, const int& length_A, const int& search_value) {
    if (length_A == 0) {
        return -1;
    }

    if (length_A == 1) {
        return 0;
    }

    if (length_A == 2) {
        return get_nearest_index(A, 0, 1, search_value);
    }

    int max_search_index =
        exponential_max_index_search(A, length_A, search_value);
    int min_search_index = max_search_index / 2;

    while (max_search_index > min_search_index) {
        int mid = (max_search_index + min_search_index) / 2;

        if (search_value <= *(A + mid)) {
            max_search_index = mid;
        } else {
            min_search_index = mid + 1;
        }
    }

    // Условия для нахождения интервала длиной 2, в котором находится нужное
    // значение
    if (A[min_search_index] > search_value && min_search_index > 0) {
        return get_nearest_index(A, min_search_index - 1, min_search_index,
                                 search_value);
    }

    if (A[min_search_index] < search_value && min_search_index < length_A - 1) {
        return get_nearest_index(A, min_search_index, min_search_index + 1,
                                 search_value);
    }

    return min_search_index;
}

void run(std::istream& in, std::ostream& out) {
    unsigned int length_A = 0;
    in >> length_A;

    int* A = new int[length_A];
    for (int i = 0; i < length_A; ++i) {
        in >> A[i];
    }

    unsigned int length_B = 0;
    in >> length_B;

    int* B = new int[length_B];
    int* searchIndexes = new int[length_B];
    for (int i = 0; i < length_B; ++i) {
        in >> B[i];
        searchIndexes[i] = binary_search(A, length_A, B[i]);
    }
    delete[] A;
    delete[] B;

    for (int i = 0; i < length_B; ++i) {
        if (i > 0) {
            out << ' ';
        }

        out << searchIndexes[i];
    }

    delete[] searchIndexes;
}

// -----------------ТЕСТИРОВАНИЕ-------------------

void test() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "3" << std::endl;
        in << "10 20 30" << std::endl;
        in << "3" << std::endl;
        in << "9 15 35" << std::endl;

        run(in, out);

        std::cout << out.str() << " - ";

        assert(out.str() == "0 0 2");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "3" << std::endl;
        in << "10 20 30" << std::endl;
        in << "4" << std::endl;
        in << "8 9 10 32" << std::endl;

        run(in, out);

        std::cout << out.str() << " - ";

        assert(out.str() == "0 0 0 2");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "5" << std::endl;
        in << "11 13 15 17 19" << std::endl;
        in << "5" << std::endl;
        in << "12 14 16 18 20" << std::endl;

        run(in, out);

        std::cout << out.str() << " - ";

        assert(out.str() == "0 1 2 3 4");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "5" << std::endl;
        in << "11 13 15 17 19" << std::endl;
        in << "5" << std::endl;
        in << "11 13 15 17 19" << std::endl;

        run(in, out);

        std::cout << out.str() << " - ";

        assert(out.str() == "0 1 2 3 4");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "5" << std::endl;
        in << "11 13 15 17 19" << std::endl;
        in << "5" << std::endl;
        in << "5 13 15 17 25" << std::endl;

        run(in, out);

        std::cout << out.str() << " - ";

        assert(out.str() == "0 1 2 3 4");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "2" << std::endl;
        in << "11 19" << std::endl;
        in << "6" << std::endl;
        in << "5 10 12 15 16 30" << std::endl;

        run(in, out);

        std::cout << out.str() << " - ";

        assert(out.str() == "0 0 0 0 1 1");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "2" << std::endl;
        in << "11 15" << std::endl;
        in << "4" << std::endl;
        in << "5 12 14 30" << std::endl;

        run(in, out);

        std::cout << out.str() << " - ";

        assert(out.str() == "0 0 1 1");

        std::cout << "OK" << std::endl;
    }
}
