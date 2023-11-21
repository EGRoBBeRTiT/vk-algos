#include <assert.h>
// #include <fstream>
#include <iostream>
#include <sstream>

/*
    1.2. Хеш-таблица

    Реализуйте структуру данных типа “множество строк” на основе динамической
    хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
    строчных латинских букв.

    Хеш-функция строки должна быть реализована с
    помощью вычисления значения многочлена методом Горнера.
    Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте
    при добавлении элементов в случае, когда коэффициент заполнения таблицы
    достигает 3/4.

    Структура данных должна поддерживать операции добавления
    строки в множество, удаления строки из множества и проверки принадлежности
    данной строки множеству.

    Для разрешения коллизий используйте двойное хеширование.
*/

static const size_t DEFAULT_SIZE = 8;
static const float MAX_ALPHA = 0.75;

static constexpr size_t BUCKETS_SIZES_LIST[] = {
    7, 17, 37, 73, 149, 251, 509, 1021, 2027, 5003, 8191, 16381};

void run(std::istream& in, std::ostream& out);
void test_algorithm();
void test_table();

int main(void) {
    // test_algorithm();
    // test_table();
    run(std::cin, std::cout);

    return 0;
}

// ----------------РЕШЕНИЕ---------------------

template <class Key>
class NilDelValues {
   public:
    static const Key NIL;
    static const Key DEL;
};

template <>
class NilDelValues<int> {
   public:
    static const int NIL = -1;
    static const int DEL = -2;
};

template <>
class NilDelValues<std::string> {
   public:
    static constexpr char NIL[] = "NIL";
    static constexpr char DEL[] = "DEL";
};

template <class Key>
class Hash {
   public:
    Hash(size_t prime) : prime(prime) {}

   private:
    size_t prime;
};

template <>
class Hash<std::string> {
   public:
    Hash(size_t prime = BUCKETS_SIZES_LIST[11]) : prime(prime) {}

    size_t operator()(const std::string& str) {
        size_t hash = 0;

        for (char i : str)
            hash = hash * prime + i;

        return hash;
    }

   private:
    size_t prime;
};

template <>
class Hash<int> {
   public:
    Hash(size_t prime) : prime(prime) {}

    size_t operator()(int value) { return value; }

   private:
    size_t prime;
};

// --------------Класс хеш таблицы ------------

template <typename Key,
          typename Hasher1 = Hash<Key>,
          typename Hasher2 = Hash<Key>>
class HashTable {
   public:
    HashTable(const size_t& initial_size = DEFAULT_SIZE);
    HashTable(const HashTable&);
    HashTable(HashTable&&);
    ~HashTable() { delete[] table; };

    HashTable& operator=(const HashTable&);
    HashTable& operator=(HashTable&&);

    bool Add(const Key&);
    bool Has(const Key&);
    bool Delete(const Key&);

   private:
    void resize();

    Key* table;
    size_t table_size;
    size_t items_count;
    Hasher1 hasher_1;
    Hasher2 hasher_2;
    NilDelValues<Key> table_values;
};

//------------------------------------------------

void run(std::istream& in, std::ostream& out) {
    HashTable<std::string> hash_table;

    char command = '\0';
    std::string value;

    while (in >> command >> value) {
        bool result = false;

        switch (command) {
            case '+':
                result = hash_table.Add(value);
                break;
            case '-':
                result = hash_table.Delete(value);
                break;
            case '?':
                result = hash_table.Has(value);
                break;
            default:
                return;
        }

        if (result) {
            out << "OK\n";
        } else {
            out << "FAIL\n";
        }
    }
}

void test_table() {
    HashTable<int> ht;
    int k = 10000;
    for (int i = 0; i < k; ++i) {
        assert(!ht.Has(i));
    }

    for (int i = 0; i < k; ++i) {
        assert(ht.Add(i));
    }

    for (int i = 0; i < k; ++i) {
        assert(ht.Has(i));
    }

    for (int i = 0; i < k; ++i) {
        assert(ht.Delete(i));
        assert(!ht.Has(i));
    }

    std::cout << "OK" << std::endl;
}

void test_algorithm() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "+ hello\n";
        in << "+ bye\n";
        in << "? bye\n";
        in << "+ bye\n";
        in << "- bye\n";
        in << "? bye\n";
        in << "? hello\n";
        in << "hello\n";

        run(in, out);

        assert(out.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");

        std::cout << "OK" << std::endl;
    }
}

// --------------Описание хеш таблицы ------------

template <typename Key, typename Hasher1, typename Hasher2>
HashTable<Key, Hasher1, Hasher2>::HashTable(const size_t& initial_size)
    : table_size(initial_size),
      items_count(0),
      hasher_1(Hasher1(BUCKETS_SIZES_LIST[10])),
      hasher_2(Hasher2(BUCKETS_SIZES_LIST[11])) {
    table = new Key[table_size];

    for (size_t i = 0; i < table_size; ++i) {
        table[i] = table_values.NIL;
    }
};

template <typename Key, typename Hasher1, typename Hasher2>
HashTable<Key, Hasher1, Hasher2>::HashTable(const HashTable& hash_table)
    : table_size(hash_table.table_size),
      items_count(hash_table.items_count),
      hasher_1(hash_table.hasher_1),
      hasher_2(hash_table.hasher_2),
      table_values(hash_table.table_values) {
    table = new Key[table_size];

    for (size_t i = 0; i < table_size; ++i) {
        table[i] = hash_table.table[i];
    }
};

template <typename Key, typename Hasher1, typename Hasher2>
HashTable<Key, Hasher1, Hasher2>::HashTable(HashTable&& hash_table)
    : table_size(hash_table.table_size),
      items_count(hash_table.items_count),
      hasher_1(hash_table.hasher_1),
      hasher_2(hash_table.hasher_2),
      table(hash_table.table),
      table_values(hash_table.table_values) {
    hash_table.table_size = hash_table.items_count = 0;
    hash_table.table = nullptr;
};

template <typename Key, typename Hasher1, typename Hasher2>
HashTable<Key, Hasher1, Hasher2>& HashTable<Key, Hasher1, Hasher2>::operator=(
    const HashTable<Key, Hasher1, Hasher2>& hash_table) {
    delete[] table;

    table_size = hash_table.table_size;
    items_count = hash_table.items_count;

    table = new Key[table_size];

    for (size_t i = 0; i < table_size; ++i) {
        table[i] = hash_table.table[i];
    }

    return *this;
};

template <typename Key, typename Hasher1, typename Hasher2>
HashTable<Key, Hasher1, Hasher2>& HashTable<Key, Hasher1, Hasher2>::operator=(
    HashTable<Key, Hasher1, Hasher2>&& hash_table) {
    if (this != &hash_table) {
        table_size = hash_table.table_size;
        items_count = hash_table.items_count;
        table = hash_table.table;

        hash_table.table = nullptr;
        hash_table.table_size = hash_table.items_count = 0;
    }

    return *this;
};

template <typename Key, typename Hasher1, typename Hasher2>
bool HashTable<Key, Hasher1, Hasher2>::Add(const Key& key) {
    if (items_count > table_size * MAX_ALPHA) {
        resize();
    }

    size_t hash_1 = hasher_1(key);

    size_t hash = hash_1 % table_size;
    size_t del_index = table_size;

    size_t hash_2 = 0;
    if (table[hash] != table_values.NIL && table[hash] != key) {
        hash_2 = hasher_2(key);
    }

    size_t i = 1;

    while (table[hash] != table_values.NIL) {
        if (table[hash] == key) {
            return false;
        }

        // запоминаем первый встречный "DEL"
        if (table[hash] == table_values.DEL && del_index == table_size) {
            del_index = hash;
        }

        if (i >= table_size) {
            break;
        }

        hash = (hash_1 + i * (hash_2 * 2 + 1)) % table_size;
        i++;
    }

    if (del_index < table_size) {
        table[del_index] = key;
    } else {
        table[hash] = key;
    }

    items_count++;

    return true;
};

template <typename Key, typename Hasher1, typename Hasher2>
bool HashTable<Key, Hasher1, Hasher2>::Delete(const Key& key) {
    size_t hash_1 = hasher_1(key);

    size_t hash = hash_1 % table_size;

    size_t hash_2 = 0;
    if (table[hash] != table_values.NIL && table[hash] != key) {
        hash_2 = hasher_2(key);
    }

    size_t i = 1;
    while (table[hash] != table_values.NIL) {
        if (table[hash] == key) {
            table[hash] = table_values.DEL;
            items_count--;
            return true;
        }

        if (i >= table_size) {
            break;
        }

        hash = (hash_1 + i * (hash_2 * 2 + 1)) % table_size;
        i++;
    }

    return false;
};

template <typename Key, typename Hasher1, typename Hasher2>
bool HashTable<Key, Hasher1, Hasher2>::Has(const Key& key) {
    size_t hash_1 = hasher_1(key);

    size_t hash = hash_1 % table_size;

    size_t hash_2 = 0;
    if (table[hash] != table_values.NIL && table[hash] != key) {
        hash_2 = hasher_2(key);
    }

    size_t i = 1;
    while (table[hash] != table_values.NIL) {
        if (table[hash] == key) {
            return true;
        }

        if (i >= table_size) {
            break;
        }

        hash = (hash_1 + i * (hash_2 * 2 + 1)) % table_size;
        i++;
    }

    return false;
};

template <typename Key, typename Hasher1, typename Hasher2>
void HashTable<Key, Hasher1, Hasher2>::resize() {
    HashTable<Key, Hasher1, Hasher2> new_hash_table(table_size * 2);

    for (size_t i = 0; i < table_size; i++) {
        Key key = table[i];

        if (key == table_values.NIL || key == table_values.DEL) {
            continue;
        }

        new_hash_table.Add(key);
    }

    *this = std::move(new_hash_table);
};

//---------------------------------------------
