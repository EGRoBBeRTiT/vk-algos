#include <assert.h>
#include <iostream>
#include <sstream>

/*
    7.3. Binary MSD для long long

    Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не
    больше 10^6. Отсортировать массив методом MSD по битам (бинарный QuickSort).
*/

void run(std::istream& in, std::ostream& out);
void test_algorithm();

int main(void) {
    // test_algorithm();
    run(std::cin, std::cout);

    return 0;
}

//---------------------Решение с РЕКУРСИЕЙ------------------------
template <typename T>
int partition(T* arr,
              const int& left,
              const int& right,
              const short& bit_mask) {
    int i = left;

    for (size_t j = left; j < right + 1; j++) {
        if (!((arr[j] >> bit_mask) & 1)) {
            std::swap(arr[j], arr[i++]);
        }
    }

    return i;
}

template <typename T>
void binary_MSD(T* arr,
                const int& left,
                const int& right,
                const short& bit_mask = 63) {
    if (left >= right || bit_mask < 0) {
        return;
    }

    int pivot_index = partition(arr, left, right, bit_mask);

    binary_MSD(arr, left, pivot_index - 1, bit_mask - 1);
    binary_MSD(arr, pivot_index, right, bit_mask - 1);
}

void read_array(std::istream& in, uint64_t* arr, const size_t& length) {
    for (size_t i = 0; i < length; i++) {
        in >> arr[i];
    }
}

void array_output(std::ostream& out, uint64_t* arr, const size_t& length) {
    for (size_t i = 0; i < length; i++) {
        if (i > 0) {
            out << " ";
        }

        out << arr[i];
    }

    out << std::endl;
}

void run(std::istream& in, std::ostream& out) {
    size_t N = 0;
    in >> N;

    uint64_t* arr = new uint64_t[N];

    read_array(in, arr, N);

    binary_MSD(arr, 0, N - 1);

    array_output(out, arr, N);

    delete[] arr;
}

// -----------------Тестирование------------------------
void test_algorithm() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "3" << std::endl;
        in << "4 1000000 7" << std::endl;

        run(in, out);

        assert(out.str() == "4 7 1000000\n");

        std::cout << "OK" << std::endl;
    }
}