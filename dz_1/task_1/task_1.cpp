#include <assert.h>
#include <iostream>
#include <sstream>

//  1.4. Инвертирование бита

//  Инвертируйте значение бита в числе N по его номеру K.
//  Формат входных данных. Число N, номер бита K.
//  Формат выходных данных. Число с инвертированным битом в десятичном виде.

void run(std::istream& in, std::ostream& out);
unsigned int inverse_bit(const unsigned int& N, const unsigned int& K);
void test_algorithm();

int main(void) {
    // test();
    run(std::cin, std::cout);

    return 0;
}

// -------------------------------------------

unsigned int inverse_bit(const unsigned int& N, const unsigned int& K) {
    return N ^ (1 << K);
}

void run(std::istream& in, std::ostream& out) {
    unsigned int N = 0;
    unsigned int K = 0;

    in >> N >> K;

    out << inverse_bit(N, K);
}

void test_algorithm() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "25 1";

        run(in, out);

        assert(out.str() == "27");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "25 4";

        run(in, out);

        assert(out.str() == "9");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "25 0";

        run(in, out);

        assert(out.str() == "24");

        std::cout << "OK" << std::endl;
    }

    {
        std::stringstream in;
        std::stringstream out;

        in << "1 0";

        run(in, out);

        assert(out.str() == "0");

        std::cout << "OK" << std::endl;
    }
}
