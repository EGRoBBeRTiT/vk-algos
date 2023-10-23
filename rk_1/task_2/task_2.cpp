#include <assert.h>
#include <iostream>

class Stack {
   public:
    Stack(int bufferSize) : bufferSize(bufferSize), top(0) {
        buffer = new char[bufferSize];
    };
    Stack() : bufferSize(0), top(-1), buffer(nullptr){};
    ~Stack() { delete[] buffer; };

    Stack operator=(const Stack& stack) {
        delete[] this->buffer;

        this->buffer = new char[stack.top];

        for (int i = 0; i < this->top; ++i) {
            buffer[i] = buffer[i];
        }

        return *this;
    }

    void Push(const char& letter) {
        if (top == bufferSize - 1) {
            resize();
        }

        buffer[top++] = letter;
    };
    char Pop() {
        assert(!IsEmpty());
        return buffer[--top];
    };

    char Top() { return buffer[top - 1]; }

    bool IsEmpty() const { return top == -1; }

   private:
    void resize() {
        bufferSize <<= 1;

        char* tmp = new char[bufferSize];
        for (int i = 0; i < top; ++i) {
            tmp[i] = buffer[i];
        }

        delete[] buffer;
        buffer = tmp;
    }

    char* buffer;
    int bufferSize;
    int top;
};

void check_words(Stack& stack,
                 const std::string& word1,
                 const std::string& word2) {
    int j = 0;

    if (word1.length() != word2.length()) {
        stack.Push('a');
        return;
    }

    for (size_t i = 0; i < word1.length(); i++) {
        stack.Push(word1[i]);

        while (stack.Top() == word2[j]) {
            stack.Pop();
            j++;
        }
    }
}

int main() {
    std::string word1 = "";
    std::string word2 = "";
    std::cin >> word1 >> word2;

    Stack stack(8);

    check_words(stack, word1, word2);

    std::cout << (stack.IsEmpty() ? "YES" : "NO") << std::endl;

    return 0;
}