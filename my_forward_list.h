#pragma once
#include <memory>
#include <cstddef>
#include <utility>

// ---------------------------------------
//   MyForwardList<T, Alloc>
//   простой односвязный список с поддержкой аллокатора
// ---------------------------------------

template <typename T, typename Alloc = std::allocator<T>>
class MyForwardList {
private:
    struct Node {
        Node* next;
        T value;

        Node(const T& v) : next(nullptr), value(v) {}
        Node(T&& v) : next(nullptr), value(std::move(v)) {}
    };

    using NodeAlloc = typename Alloc::template rebind<Node>::other;
    using NodeTraits = std::allocator_traits<NodeAlloc>;

public:
    // ---------------------------------------
    //   Итератор
    // ---------------------------------------
    struct iterator {
        using reference = T&;
        using pointer = T*;

        iterator(Node* n = nullptr) : node(n) {}

        reference operator*() const { return node->value; }
        pointer operator->() const { return &node->value; }

        iterator& operator++() {
            node = node->next;
            return *this;
        }

        bool operator==(const iterator& o) const { return node == o.node; }
        bool operator!=(const iterator& o) const { return node != o.node; }

    private:
        Node* node;
        friend class MyForwardList;
    };

    // ---------------------------------------
    //   Конструкторы
    // ---------------------------------------
    MyForwardList() : head(nullptr), tail(nullptr), sz(0), alloc() {}

    explicit MyForwardList(const Alloc& a)
        : head(nullptr), tail(nullptr), sz(0), alloc(a) {}

    ~MyForwardList() { clear(); }

    // ---------------------------------------
    //   Итераторы
    // ---------------------------------------
    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

    iterator begin() const { return iterator(head); }
    iterator end() const { return iterator(nullptr); }

    // ---------------------------------------
    //   Методы
    // ---------------------------------------
    void push_back(const T& value) {
        Node* node = NodeTraits::allocate(alloc, 1);
        NodeTraits::construct(alloc, node, value);
        append(node);
    }

    void push_back(T&& value) {
        Node* node = NodeTraits::allocate(alloc, 1);
        NodeTraits::construct(alloc, node, std::move(value));
        append(node);
    }

    std::size_t size() const noexcept { return sz; }
    bool empty() const noexcept { return sz == 0; }

    void clear() noexcept {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            NodeTraits::destroy(alloc, cur);
            NodeTraits::deallocate(alloc, cur, 1);
            cur = nxt;
        }
        head = tail = nullptr;
        sz = 0;
    }

private:
    // данные
    Node* head;
    Node* tail;
    std::size_t sz;
    NodeAlloc alloc;

    void append(Node* node) {
        node->next = nullptr;
        if (!head)
            head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
        sz++;
    }
};
