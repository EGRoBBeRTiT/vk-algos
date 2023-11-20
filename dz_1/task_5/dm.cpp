#include <algorithm>
#include <iostream>

/*
    Группа людей называется современниками если был такой момент,
    когда они могли собраться вместе. Для этого в этот момент
    каждому из них должно было уже исполниться 18 лет,
    но ещё не исполниться 80 лет. Дан список Жизни Великих Людей.
    Необходимо получить максимальное количество современников.
    В день 18летия человек уже может принимать участие в собраниях,
    а в день 80летия и в день смерти уже не может.
    Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия.
    В этих случаях принимать участие в собраниях он не мог.
*/
// Сложность алгоритма О(N * log(N)), т.к. сортируем события слиянием,
// дополнительная память О(N)

struct Event {
    Event(const int& d, const int& m, const int& y, const int& t = 0)
        : day(d), month(m), year(y), type(t) {}

    Event() : day(0), month(0), year(0), type(0) {}

    int day;
    int month;
    int year;
    int type;
};

std::istream& operator>>(std::istream& input, Event& event) {
    input >> event.day >> event.month >> event.year;

    return input;
}

bool cmpevents(const Event& left, const Event& right) {
    return (left.year < right.year) ||
           (left.year == right.year && left.month < right.month) ||
           (left.year == right.year && left.month == right.month &&
            left.day < right.day) ||
           (left.year == right.year && left.month == right.month &&
            left.day == right.day && left.type < right.type);
}

bool DieAfter18(const Event& birth, const Event& death) {
    return cmpevents(birth, death);
}

Event FindMinDeathOrEighty(const Event& death,
                           const Event& birthPlus80Minus18) {
    if (cmpevents(death, birthPlus80Minus18))
        return death;

    return birthPlus80Minus18;
}

template <typename T>
class MyVector {
   public:
    MyVector() : capacity_(8), size_(0), arr_(new T[capacity_]) {}

    MyVector(const MyVector& other)
        : capacity_(other.capacity_),
          size_(other.size_),
          arr_(new T[capacity_]) {}

    MyVector& operator=(const MyVector& other) {
        delete[] arr_;

        size_ = other.size_;
        capacity_ = other.capacity_;
        arr_ = new T[capacity_];

        for (size_t i = 0; i < size_; i++) {
            arr_[i] = other.arr_[i];
        }
    }

    size_t size() const { return size_; }

    void PushBack(const T& element) {
        if (size_ == capacity_) {
            Resize();
        }

        arr_[size_++] = element;
    }

    T& operator[](const int& i) { return arr_[i]; }

    ~MyVector() { delete[] arr_; }

   private:
    void Resize() {
        capacity_ <<= 1;

        T* tmp = new T[capacity_];

        for (size_t i = 0; i < size_; i++) {
            tmp[i] = arr_[i];
        }

        delete[] arr_;

        arr_ = tmp;
    }

    size_t capacity_;
    size_t size_;
    T* arr_;
};

typedef bool (*Comparator)(const Event&, const Event&);

template <typename T, Comparator cmp>
void Merge(T& arr, size_t left, size_t middle, size_t right) {
    T result;
    size_t i = left;    // счётчик левой части
    size_t j = middle;  // счётчик правой части

    while (i < middle && j < right) {
        if (cmp(arr[i], arr[j]))
            result.PushBack(arr[i++]);
        else
            result.PushBack(arr[j++]);
    }

    while (i < middle) {
        result.PushBack(arr[i++]);
    }

    while (j < right) {
        result.PushBack(arr[j++]);
    }

    for (size_t i = left; i < right; i++) {
        arr[i] = result[i - left];
    }
}

template <typename T, Comparator cmp>
void MergeSort(T& arr, size_t left, size_t right) {
    if (right - left < 2)
        return;

    size_t middle = left + (right - left) / 2;

    MergeSort<T, cmp>(arr, left, middle);
    MergeSort<T, cmp>(arr, middle, right);

    Merge<T, cmp>(arr, left, middle, right);
}

void InputAndCheckEvents(MyVector<Event>& events, const int& n) {
    // При рождении тип события 1, при смерти или достижении 80 -1
    Event birth = {0, 0, 0, 1};
    Event death = {0, 0, 0, -1};

    for (int i = 0; i < n; i++) {
        std::cin >> birth;
        std::cin >> death;

        // Возраст, при котором человек может стать современником
        birth.year += 18;

        if (DieAfter18(birth, death)) {
            events.PushBack(birth);
            events.PushBack(FindMinDeathOrEighty(
                death,
                Event(birth.day, birth.month, birth.year + 62, death.type)));
        }
    }
}

int CountMax(MyVector<Event>& events) {
    int count = 0;
    int max_count = 0;

    for (size_t i = 0; i < events.size(); i++) {
        count += events[i].type;

        if (count > max_count)
            max_count = count;
    }

    return max_count;
}

int main() {
    MyVector<Event> events;
    int n = 0;
    std::cin >> n;

    InputAndCheckEvents(events, n);

    MergeSort<MyVector<Event>, cmpevents>(events, 0, events.size());

    std::cout << CountMax(events);

    return 0;
}