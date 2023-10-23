#include <iostream>

/*
    Дан отсортированный по возрастанию массив попарно различных целых чисел
    A[0..n-1]. На вход также подается некоторое целое число k. Необходимо
    вернуть индекс элемента k в массиве A, если он там присутствует, либо
    вернуть позицию, куда его следовало бы вставить, чтобы массив остался
    отсортированным.

    Требование: сложность O(logN)
*/

int binary_search(int* A, const int& length_A, const int& search_value);

int main(void) {
    unsigned int length_A = 0;
    std::cin >> length_A;

    int* A = new int[length_A];
    for (int i = 0; i < length_A; ++i) {
        std::cin >> A[i];
    }

    int k = 0;
    std::cin >> k;

    std::cout << binary_search(A, length_A, k) << std::endl;

    delete[] A;

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

int binary_search(int* A, const int& length_A, const int& search_value) {
    if (length_A == 0) {
        return -1;
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

    if (A[min_search_index] >= search_value) {
        return min_search_index;
    }

    return min_search_index + 1;
}
