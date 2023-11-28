#include <assert.h>
#include <iostream>
#include <sstream>

/*
    3.1. Во всех задачах из следующего списка следует написать структуру данных,
    обрабатывающую команды push* и pop*.

    Реализовать очередь с динамическим зацикленным буфером (на основе
    динамического массива). Требования: Очередь должна быть реализована в виде
    класса.
*/

void run(std::istream& in, std::ostream& out);
void test_algorithm();
void test_queue();

template <class Key>
class Queue {
   private:
    int head;
    int tail;
    int buffer_size;
    int size;
    Key* buffer;

    bool CanPush() { return ((tail + 1) % buffer_size) != head; };

    void Resize();

   public:
    Queue();
    Queue(const unsigned int& buffer_size);
    Queue(const Queue& queue);
    ~Queue() { delete[] buffer; };

    Queue& operator=(const Queue& queue);

    bool IsEmpty() { return head == tail; };

    void Push(Key value);

    Key Pop();
};

int main(void) {
    // test_queue();
    // test_algorithm();
    run(std::cin, std::cout);

    return 0;
}

// -----------------РЕШЕНИЕ-------------------

template <class Key>
bool do_command(const unsigned int& command, const Key& value, Queue<Key>& queue) {
    switch (command) {
        case 2:
            return queue.Pop() == value;
        case 3:
            queue.Push(value);

            return true;
        default:
            return true;
    }
}

void run(std::istream& in, std::ostream& out) {
    unsigned int N = 0;

    in >> N;

    unsigned int command = 0;
    int value = 0;

    Queue<int> queue;
    bool is_correct = true;

    for (int i = 0; i < N; ++i) {
        in >> command >> value;

        if (!is_correct) {
            continue;
        }

        is_correct = do_command(command, value, queue);
    }

    out << (is_correct ? "YES" : "NO") << std::endl;
}

template <class Key>
Queue<Key>::Queue() : buffer_size(8), head(0), tail(0), size(0) {
    buffer = new Key[buffer_size];
}

template <class Key>
Queue<Key>::Queue(const unsigned int& buffer_size)
    : buffer_size(buffer_size), head(0), tail(0), size(0) {
    buffer = new Key[buffer_size];
}

template <class Key>
Queue<Key>::Queue(const Queue& queue)
    : buffer_size(queue.buffer_size),
      head(queue.head),
      tail(queue.tail),
      size(queue.size) {
    buffer = new Key[buffer_size];

    for (int i = 0; i < size; ++i) {
        buffer[(i + head) % buffer_size] =
            queue.buffer[(i + head) % buffer_size];
    }
};

template <class Key>
Queue<Key>& Queue<Key>::operator=(const Queue<Key>& queue) {
    delete[] buffer;

    buffer_size = queue.buffer_size;
    head = queue.head;
    tail = queue.tail;
    size = queue.size;
    buffer = new Key[buffer_size];

    for (int i = 0; i < size; ++i) {
        buffer[(i + head) % buffer_size] =
            queue.buffer[(i + head) % buffer_size];
    }

    return *this;
}

template <class Key>
void Queue<Key>::Push(Key value) {
    if (!CanPush()) {
        Resize();
    }

    buffer[tail] = value;
    tail = (tail + 1) % buffer_size;
    size++;
}

template <class Key>
Key Queue<Key>::Pop() {
    if (IsEmpty()) {
        return -1;
    }

    Key head_value = buffer[head];

    head = (head + 1) % buffer_size;
    size--;

    return head_value;
}

template <class Key>
void Queue<Key>::Resize() {
    Key* new_buffer = new Key[buffer_size * 2];

    for (int i = 0; i < buffer_size - 1; ++i) {
        new_buffer[i] = buffer[(i + head) % buffer_size];
    }

    delete[] buffer;
    buffer = new_buffer;

    head = 0;
    tail = buffer_size - 1;

    buffer_size *= 2;
};

// -----------------ТЕСТИРОВАНИЕ-------------------

void test_queue() {
    {
        Queue<int> queue;
        assert(queue.IsEmpty());

        queue.Push(1);
        assert(!queue.IsEmpty());

        queue.Push(2);
        assert(queue.Pop() == 1);
        assert(queue.Pop() == 2);
        assert(queue.IsEmpty());
    }

    {
        Queue<int> queue(4);
        queue.Push(1);  // [1, , , ]
        queue.Push(2);  // [1, 2, , ]
        queue.Push(3);  // [1, 2, 3, ]
        queue.Pop();    // [ , 2, 3, ]
        queue.Push(4);  // [ , 2, 3, 4]
        queue.Push(5);  // [2, 3, 4, 5, , , , ]

        assert(queue.Pop() == 2);
    }

    {
        Queue<int> queue(4);
        queue.Push(1);  // [1, , , ]
        queue.Push(2);  // [1, 2, , ]
        queue.Push(3);  // [1, 2, 3, ]
        queue.Pop();    // [ , 2, 3, ]
        queue.Pop();    // [ , , 3, ]
        queue.Push(4);  // [ , , 3, 4]
        queue.Push(5);  // [ 5, , 3, 4]

        assert(queue.Pop() == 3);

        queue.Pop();
        queue.Pop();
        assert(queue.IsEmpty());
    }

    {
        Queue<int> queue_1(3);
        queue_1.Push(1);

        Queue<int> queue_2 = queue_1;

        queue_1.Pop();
        assert(queue_1.IsEmpty());
        assert(!queue_2.IsEmpty());

        Queue<int> queue_3;
        queue_3 = queue_2;

        queue_2.Pop();
        assert(queue_2.IsEmpty());
        assert(!queue_3.IsEmpty());

        assert(queue_3.Pop() == 1);
        assert(queue_3.IsEmpty());
    }
}

void test_algorithm() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "3" << std::endl;
        in << "3 44" << std::endl;
        in << "3 50" << std::endl;
        in << "2 44" << std::endl;

        run(in, out);

        assert(out.str() == "YES\n");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "2" << std::endl;
        in << "2 -1" << std::endl;
        in << "3 10" << std::endl;

        run(in, out);

        assert(out.str() == "YES\n");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "2" << std::endl;
        in << "3 44" << std::endl;
        in << "2 66" << std::endl;

        run(in, out);

        assert(out.str() == "NO\n");

        std::cout << "OK" << std::endl;
    }
}
