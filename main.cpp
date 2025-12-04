#include <iostream>
#include <map>
#include "allocator.h"
#include "my_forward_list.h"

// Факториал
long long factorial(int n) {
    long long r = 1;
    for (int i = 2; i <= n; i++) r *= i;
    return r;
}

// Печать std::map
template <typename Map>
void print_map(const Map& m) {
    for (auto& kv : m)
        std::cout << kv.first << " " << kv.second << "\n";
}

// Печать MyForwardList
template <typename List>
void print_list(const List& l) {
    for (auto it = l.begin(); it != l.end(); ++it)
        std::cout << *it << "\n";
}

int main() {

    // ---------------------------------------
    // 1. std::map с обычным аллокатором
    // ---------------------------------------
    std::map<int, int> m_default;
    for (int i = 0; i <= 9; i++)
        m_default[i] = factorial(i);

    // ---------------------------------------
    // 2. std::map с пользовательским PoolAllocator
    // ---------------------------------------
    using Pair = std::pair<const int, int>;

    PoolAllocator<Pair> poolAllocForMap(10);
    std::map<int, int, std::less<int>, PoolAllocator<Pair>>
        m_pool(std::less<int>(), poolAllocForMap);

    for (int i = 0; i <= 9; i++)
        m_pool[i] = factorial(i);

    std::cout << "Map (default allocator):\n";
    print_map(m_default);

    std::cout << "\nMap (pool allocator):\n";
    print_map(m_pool);

    // ---------------------------------------
    // 3. Пользовательский контейнер
    // ---------------------------------------
    MyForwardList<int> list_default;
    for (int i = 0; i <= 9; i++)
        list_default.push_back(i);

    PoolAllocator<int> poolAllocList(10);
    MyForwardList<int, PoolAllocator<int>> list_pool(poolAllocList);

    for (int i = 0; i <= 9; i++)
        list_pool.push_back(i);

    std::cout << "\nMyForwardList (default allocator):\n";
    print_list(list_default);

    std::cout << "\nMyForwardList (pool allocator):\n";
    print_list(list_pool);

    return 0;
}
