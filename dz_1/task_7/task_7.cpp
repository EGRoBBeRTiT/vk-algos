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
    test_algorithm();
    // run(std::cin, std::cout);

    return 0;
}

//---------------------Решение------------------------
// Решил вместо рекурсии попробовать использовать стек
template <class T>
class Stack {
   public:
    Stack();
    Stack(const size_t& capacity);
    Stack(const Stack& stack);
    ~Stack() { delete[] buffer; }

    Stack& operator=(const Stack& stack);

    bool IsEmpty() { return size == 0; }
    void Push(T value);
    T Pop();

   private:
    bool canPush() { return size < capacity; }
    void resize();

    T* buffer;
    size_t size;
    size_t capacity;
};

// Параметры для вызова функции
template <typename T>
struct CallParams {
    T* arr;
    int left;
    int right;
    short bit_mask;
};

template <typename T>
int partition(T* arr,
              const int& left,
              const int& right,
              const short& bit_mask) {
    int i = left;

    for (size_t j = left; j < right + 1; j++) {
        if (!(arr[j] & (1 << bit_mask))) {
            std::swap(arr[j], arr[i++]);
        }
    }

    return i;
}

template <typename T>
class BinaryMSD {
   public:
    void Run(T* arr,
             const int& left,
             const int& right,
             const short& bit_mask = 63) {
        call_stack.Push({arr, left, right, bit_mask});

        while (!call_stack.IsEmpty()) {
            CallParams<T> next_params = call_stack.Pop();

            runInteration(next_params.arr, next_params.left, next_params.right,
                          next_params.bit_mask);
        }
    }

   private:
    void runInteration(T* arr,
                       const int& left,
                       const int& right,
                       const short& bit_mask) {
        if (left >= right || bit_mask < 0) {
            return;
        }

        int pivot_index = partition(arr, left, right, bit_mask);

        short new_bit_mask = bit_mask - 1;

        call_stack.Push({arr, left, pivot_index - 1, new_bit_mask});
        call_stack.Push({arr, pivot_index, right, new_bit_mask});
    }

    Stack<CallParams<T>> call_stack;
};

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

    BinaryMSD<uint64_t> binary_MSD;
    binary_MSD.Run(arr, 0, N - 1);

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

// -----------------Описание стека------------------------
template <typename T>
Stack<T>::Stack() : capacity(8), size(0), buffer(new T[8]){};

template <typename T>
Stack<T>::Stack(const size_t& capacity)
    : capacity(capacity), size(0), buffer(new T[capacity]){};

template <typename T>
Stack<T>::Stack(const Stack& stack)
    : capacity(stack.capacity),
      size(stack.size),
      buffer(new T[stack.capacity]) {
    for (size_t i = 0; i < size; ++i) {
        buffer[i] = stack.buffer[i];
    }
};

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& stack) {
    delete[] buffer;

    capacity = stack.capacity;
    size = stack.size;

    buffer = new T[size];

    for (size_t i = 0; i < size; ++i) {
        buffer[i] = stack.buffer[i];
    }

    return *this;
};

template <typename T>
void Stack<T>::Push(T value) {
    if (!canPush()) {
        resize();
    }

    buffer[size] = value;
    size++;
}

template <typename T>
T Stack<T>::Pop() {
    assert(!IsEmpty());

    return buffer[--size];
}

template <typename T>
void Stack<T>::resize() {
    capacity *= 2;

    T* new_buffer = new T[capacity];

    for (size_t i = 0; i < size; ++i) {
        new_buffer[i] = buffer[i];
    }

    delete[] buffer;

    buffer = new_buffer;
}
