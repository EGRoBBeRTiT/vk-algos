#include <assert.h>
#include <iostream>

#include <sstream>

/*
    Во всех задачах данного раздела необходимо реализовать и использовать
    сортировку слиянием в виде шаблонной функции. Решение должно поддерживать
    передачу функции сравнения снаружи. Общее время работы алгоритма O(n log n).

    5.4. Закраска прямой в один слой

    На числовой прямой окрасили N отрезков. Известны координаты левого и правого
    концов каждого отрезка (Li и Ri). Найти сумму длин частей числовой прямой,
    окрашенных ровно в один слой.
*/

struct Point {
    int x;
    short is_start;  // 1 or -1

    bool operator<(const Point& point) const { return this->x < point.x; }
};

template <class Key>
struct DefaultComparator {
    bool operator()(const Key& l, const Key& r) const { return l < r; }
};

template <class Key, class Comparator = DefaultComparator<Key>>
void merge_sort(Key* arr,
                const size_t& length,
                Comparator comp = DefaultComparator<Key>());
void run(std::istream& in, std::ostream& out);
void test_algorithm();
void input_array(std::istream& in, Point* arr, const size_t& length);
size_t count_one_color_length(Point* arr, const size_t& length);

int main(void) {
    // test_algorithm();
    run(std::cin, std::cout);

    return 0;
}

//------------------Решение-------------------------

void run(std::istream& in, std::ostream& out) {
    size_t N = 0;

    in >> N;

    size_t length = N * 2;
    Point* arr = new Point[length];

    input_array(in, arr, length);
    merge_sort(arr, length);

    size_t one_color_length = count_one_color_length(arr, length);

    out << one_color_length << std::endl;

    delete[] arr;
}

void input_array(std::istream& in, Point* arr, const size_t& length) {
    for (size_t i = 0; i < length; i += 2) {
        int start = 0;
        int end = 0;

        in >> start >> end;

        arr[i] = {x : start, is_start : 1};
        arr[i + 1] = {x : end, is_start : -1};
    }
}

size_t count_one_color_length(Point* arr, const size_t& length) {
    size_t one_color_length = 0;
    uint16_t counter = 0;
    int start_color = 0;

    for (size_t i = 0; i < length; ++i) {
        if (counter > 1) {
            start_color = arr[i].x;
        }

        counter += arr[i].is_start;

        if (counter == 1) {
            start_color = arr[i].x;
        }

        if (counter == 0 || counter == 2) {
            one_color_length += (arr[i].x - start_color);
        }
    }

    return one_color_length;
}

template <class Key, class Comparator = DefaultComparator<Key>>
void merge(Key* arr,
           const size_t& start,
           const size_t& mid,
           const size_t& end,
           Comparator comp = DefaultComparator<Key>()) {
    size_t index_a = start;
    size_t index_b = mid;
    size_t merge_index = 0;

    Key* merged_arr = new Key[end - start];

    while (index_a < mid && index_b < end) {
        if (comp(arr[index_a], arr[index_b])) {
            merged_arr[merge_index] = arr[index_a];
            index_a++;
        } else {
            merged_arr[merge_index] = arr[index_b];
            index_b++;
        }

        merge_index++;
    }

    size_t index = index_a >= mid ? index_b : index_a;
    size_t length = index_a >= mid ? end : mid;

    while (index < length) {
        merged_arr[merge_index] = arr[index];
        index++;
        merge_index++;
    }

    for (size_t i = start; i < end; ++i) {
        arr[i] = merged_arr[i - start];
    }

    delete[] merged_arr;
}
template <class Key, class Comparator>
void merge_sort(Key* arr, const size_t& length, Comparator comp) {
    // Разбиение на 2^k подмассивов
    for (size_t curr_length = 1; curr_length < length; curr_length *= 2) {
        // Обход по парам подмассивов
        for (size_t start = 0; start + curr_length < length;
             start += curr_length * 2) {
            size_t end = start + curr_length * 2;

            if (end > length) {
                end = length;
            }
            // слияние двух подмассивов с помощью буфферного массива
            merge<Key, Comparator>(arr, start, start + curr_length, end, comp);
        }
    }
}

//---------------Тестирование--------------------

void test_algorithm() {
    {
        int a[4] = {8, 9, 6, 7};

        int merged[4] = {6, 7, 8, 9};

        merge(a, 0, 2, 4);

        for (int i = 0; i < 4; ++i) {
            assert(a[i] == merged[i]);
        }

        std::cout << "OK" << std::endl;
    }

    {
        int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        int sorted[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

        merge_sort(a, 10, [](int a, int b) { return a >= b; });

        for (int i = 0; i < 10; ++i) {
            assert(a[i] == sorted[i]);
        }

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "3" << std::endl;
        in << "1 4" << std::endl;
        in << "7 8" << std::endl;
        in << "2 5" << std::endl;

        run(in, out);

        assert(out.str() == "3\n");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "3" << std::endl;
        in << "4 5" << std::endl;
        in << "1 5" << std::endl;
        in << "3 5" << std::endl;

        run(in, out);

        assert(out.str() == "2\n");

        std::cout << "OK" << std::endl;
    }
}
