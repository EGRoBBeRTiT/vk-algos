#include <assert.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

/*
    6.2 Порядковая статистика и параметры множества

    Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию
    Partition реализуйте методом прохода двумя итераторами от конца массива к
    началу.
*/

void run(std::istream& in, std::ostream& out);
void test_algorithm();

int main(void) {
    // test_algorithm();
    run(std::cin, std::cout);

    return 0;
}

// -------------------------------------------

template <class Key>
struct DefaultComparator {
    bool operator()(const Key& l, const Key& r) const { return l < r; }
};

template <typename Key, typename Comp = DefaultComparator<Key>>
size_t get_pivot_index(Key* arr,
                       const size_t& l,
                       const size_t& r,
                       Comp comp = Comp()) {
    srand((unsigned)time(NULL));

    size_t index_1 = l + rand() % (r - l);
    size_t index_2 = l + rand() % (r - l);
    size_t index_3 = l + rand() % (r - l);

    if (!comp(arr[index_1], arr[index_2]) &&
        !comp(arr[index_3], arr[index_1])) {
        return index_1;
    }

    if (!comp(arr[index_2], arr[index_1]) &&
        !comp(arr[index_3], arr[index_2])) {
        return index_2;
    }

    return index_3;
}

template <typename Key, typename Comp = DefaultComparator<Key>>
size_t partition(Key* arr, const size_t& l, const size_t& r, Comp comp = Comp()) {
    size_t pivot_idx = get_pivot_index(arr, l, r, comp);

    std::swap(arr[l], arr[pivot_idx]);

    int i = r - 1;
    int j = r - 1;

    while (j > l) {
        for (; comp(arr[j], arr[l]); j--) {
        }

        if (j > l) {
            std::swap(arr[j], arr[i]);

            j--;
            i--;
        }
    }

    std::swap(arr[l], arr[i]);

    return i;
}

template <class Key, class Comp = DefaultComparator<Key>>
Key kth_element(Key* arr, const size_t& size, const size_t& k, Comp comp = Comp()) {
    size_t l = 0;
    size_t r = size;

    while (l < r) {
        size_t m = partition(arr, l, r, comp);

        if (m == k) {
            return arr[m];
        }

        else if (k < m) {
            r = m;
        } else {
            l = m + 1;
        }
    }

    return arr[k];
}

void run(std::istream& in, std::ostream& out) {
    size_t n = 0;

    in >> n;

    int* arr = new int[n];

    for (size_t i = 0; i < n; ++i) {
        in >> arr[i];
    }

    out << kth_element(arr, n, n * 0.1) << '\n';
    out << kth_element(arr, n, n * 0.5) << '\n';
    out << kth_element(arr, n, n * 0.9);

    delete[] arr;
}

void test_algorithm() {
    {
        std::vector<int> arr = {1, 3, 5, 7, 3, 4, 6, 12, 2, 2};

        std::vector<int> expected(arr);
        std::sort(expected.begin(), expected.end());

        for (int i = 0; i < arr.size(); ++i) {
            kth_element(&arr[0], arr.size(), i);
            assert(arr[i] == expected[i]);
        }

        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;

        in << "10" << std::endl;
        in << "1 2 3 4 5 6 7 8 9 10";

        run(in, out);

        assert(out.str() == "2\n6\n10");

        std::cout << "OK" << std::endl;
    }
}
