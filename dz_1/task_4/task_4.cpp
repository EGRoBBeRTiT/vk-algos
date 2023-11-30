#include <assert.h>
#include <iostream>
#include <sstream>

/*
    Решение всех задач данного раздела предполагает использование кучи,
    реализованной в виде шаблонного класса. Решение должно поддерживать передачу
    функции сравнения снаружи. Куча должна быть динамической.

    4.1. Слияние массивов.
    Напишите программу, которая использует кучу для слияния K отсортированных
    массивов суммарной длиной N.

    Требования: время работы O(N * logK). Ограничение на размер кучи O(K).
*/

template <class Key>
struct DefaultComparator {
    bool operator()(const Key& l, const Key& r) const { return l < r; }
};

template <class Key, class Comparator = DefaultComparator<Key>>
class Heap;

template <class Key>
class Array;

void run(std::istream& in, std::ostream& out);
void test_algorithm();
void read_array(std::istream& in, Heap<Array<int>>& heap);
void out_heap_array(std::ostream& out, Heap<Array<int>>& heap);

int main(void) {
    test_algorithm();
    // run(std::cin, std::cout);

    return 0;
}

// -----------------РЕШЕНИЕ-------------------

template <class Key, class Comparator>
class Heap {
   public:
    Heap(Comparator comp = DefaultComparator<Key>());
    Heap(size_t size, Comparator comp = DefaultComparator<Key>());
    Heap(const Heap& heap);
    ~Heap() { delete[] buffer; };

    Heap& operator=(const Heap& heap) {
        delete[] buffer;

        comparator = heap.comparator;
        capacity = heap.capacity;
        size = heap.size;

        buffer = new Key[capacity];

        for (int i = 0; i < size; ++i) {
            buffer[i] = heap.buffer[i];
        }

        return *this;
    }

    void Insert(const Key& val);
    bool IsEmpty() { return size == 0; };
    Key ExtractTop();

   private:
    void siftUp(size_t index);
    void siftDown(const size_t& index);
    bool canInsert() { return size < capacity; };
    void resize();

    Comparator comparator;

    Key* buffer;
    size_t capacity;
    size_t size;
};

template <class Key>
class Array {
   public:
    Array() : buffer(new Key[8]), capacity(8), curPos(0), size(0){};
    Array(size_t capacity)
        : buffer(new Key[capacity]), capacity(capacity), curPos(0), size(0){};
    Array(const Array& arr)
        : buffer(new Key[arr.capacity]),
          capacity(arr.capacity),
          curPos(arr.curPos),
          size(arr.size) {
        for (int i = 0; i < size; ++i) {
            buffer[i] = arr.buffer[i];
        }
    };
    Array& operator=(const Array& arr) {
        delete[] buffer;

        curPos = arr.curPos;
        capacity = arr.capacity;
        size = arr.size;

        buffer = new Key[capacity];

        for (int i = 0; i < size; ++i) {
            buffer[i] = arr.buffer[i];
        }

        return *this;
    }
    ~Array() { delete[] buffer; };

    void Push(const Key& value) {
        if (size < capacity) {
            buffer[size] = value;
            size++;
        }
    }

    Key GetCurValue() {
        if (size > 0 && curPos < size) {
            return buffer[curPos];
        }

        return -1;
    }

    size_t GetCurPos() { return curPos; }
    size_t GetCapacity() { return capacity; }

    void IncPos() {
        if (curPos < size) {
            curPos++;
        }
    }
    void DecPos() {
        if (curPos > size) {
            curPos--;
        }
    }

    bool operator<(const Array& arr) const {
        return buffer[curPos] < arr.buffer[arr.curPos];
    }

   private:
    Key* buffer;
    size_t capacity;
    size_t curPos;
    size_t size;
};

void run(std::istream& in, std::ostream& out) {
    size_t K = 0;
    in >> K;

    Heap<Array<int>> heap;

    for (int i = 0; i < K; ++i) {
        read_array(in, heap);
    }

    out_heap_array(out, heap);

    out << std::endl;
}

void read_array(std::istream& in, Heap<Array<int>>& heap) {
    size_t arr_length = 0;

    in >> arr_length;

    Array<int> arr(arr_length);

    for (int i = 0; i < arr_length; ++i) {
        int value = 0;

        in >> value;

        arr.Push(value);
    }

    heap.Insert(arr);
}

void out_heap_array(std::ostream& out, Heap<Array<int>>& heap) {
    size_t i = 0;
    while (!heap.IsEmpty()) {
        Array<int> arr = heap.ExtractTop();

        if (i > 0) {
            out << " ";
        }

        out << arr.GetCurValue();

        arr.IncPos();

        if (arr.GetCurPos() < arr.GetCapacity()) {
            heap.Insert(arr);
        }

        i++;
    }
}

// -----------------Описание кучи-------------------

template <class Key, class Comparator>
Heap<Key, Comparator>::Heap(Comparator comp)
    : size(0), capacity(8), comparator(comp) {
    buffer = new Key[capacity];
}

template <class Key, class Comparator>
Heap<Key, Comparator>::Heap(size_t size, Comparator comp)
    : size(0), capacity(size), comparator(comp) {
    buffer = new Key[capacity];
}

template <class Key, class Comparator>
Heap<Key, Comparator>::Heap(const Heap& heap)
    : size(heap.size), capacity(heap.size), comparator(heap.comparator) {
    buffer = new Key[capacity];

    for (int i = 0; i < size; ++i) {
        buffer[i] = heap.buffer[i];
    }
}

template <class Key, class Comparator>
void Heap<Key, Comparator>::siftDown(const size_t& index) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;

    // Ищем наименьшего сына, если такой есть.
    size_t smallest = index;

    if (left < size && comparator(buffer[left], buffer[smallest]))
        smallest = left;

    if (right < size && comparator(buffer[right], buffer[smallest]))
        smallest = right;

    if (smallest != index) {
        std::swap(buffer[index], buffer[smallest]);
        siftDown(smallest);
    }
}

template <class Key, class Comparator>
void Heap<Key, Comparator>::siftUp(size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;

        if (!comparator(buffer[index], buffer[parent]))
            return;

        std::swap(buffer[index], buffer[parent]);

        index = parent;
    }
}

template <class Key, class Comparator>
void Heap<Key, Comparator>::resize() {
    capacity *= 2;

    Key* new_buffer = new Key[capacity];

    for (int i = 0; i < size; ++i) {
        new_buffer[i] = buffer[i];
    }

    delete[] buffer;

    buffer = new_buffer;
}

template <class Key, class Comparator>
void Heap<Key, Comparator>::Insert(const Key& value) {
    if (!canInsert()) {
        resize();
    }

    buffer[size] = value;
    size++;

    siftUp(size - 1);
}

template <class Key, class Comparator>
Key Heap<Key, Comparator>::ExtractTop() {
    assert(!IsEmpty());

    Key result = buffer[0];
    buffer[0] = buffer[--size];

    if (!IsEmpty()) {
        siftDown(0);
    }

    return result;
}

// -----------------ТЕСТИРОВАНИЕ-------------------

void test_algorithm() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "3" << std::endl;
        in << "1" << std::endl;
        in << "6" << std::endl;
        in << "2" << std::endl;
        in << "50 90" << std::endl;
        in << "3" << std::endl;
        in << "1 10 70" << std::endl;

        run(in, out);

        assert(out.str() == "1 6 10 50 70 90\n");

        std::cout << out.str() << "OK\n";
    }
}
