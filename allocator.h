#pragma once
#include <cstddef>
#include <vector>
#include <memory>
#include <new>

template <typename T>
class PoolAllocator {
public:
    using value_type = T;

    PoolAllocator() noexcept
        : state(std::make_shared<PoolState>(sizeof(T))) {}

    explicit PoolAllocator(std::size_t initial)
        : state(std::make_shared<PoolState>(sizeof(T))) {
        state->allocate_new_chunk(initial);
    }

    template<typename U>
    PoolAllocator(const PoolAllocator<U>&)
        : state(std::make_shared<PoolState>(sizeof(T))) {}

    PoolAllocator(const PoolAllocator&) noexcept = default;
    PoolAllocator& operator=(const PoolAllocator&) noexcept = default;

    T* allocate(std::size_t n) {
        if (n != 1)
            return static_cast<T*>(::operator new(n * sizeof(T)));

        return static_cast<T*>(state->allocate_block());
    }

    void deallocate(T* p, std::size_t n) noexcept {
        if (n == 1 && state->pointer_in_chunks(p)) {
            state->freelist.push_back(p);
            return;
        }
        ::operator delete(p);
    }

    template<class U>
    struct rebind { using other = PoolAllocator<U>; };

private:
    struct PoolState {
        explicit PoolState(size_t elem_size)
            : element_size(elem_size), chunk_capacity(0) {}

        void allocate_new_chunk(size_t count) {
            if (count == 0) count = 4;
            if (chunk_capacity == 0) chunk_capacity = count;
            else chunk_capacity = std::max(chunk_capacity, count);

            size_t bytes = chunk_capacity * element_size;
            void* mem = ::operator new(bytes);
            chunks.push_back(mem);

            char* base = (char*)mem;
            for (size_t i = 0; i < chunk_capacity; i++)
                freelist.push_back(base + i * element_size);
        }

        void* allocate_block() {
            if (freelist.empty())
                allocate_new_chunk(chunk_capacity == 0 ? 4 : chunk_capacity * 2);

            void* p = freelist.back();
            freelist.pop_back();
            return p;
        }

        bool pointer_in_chunks(void* p) const noexcept {
            for (void* chunk : chunks) {
                char* base = (char*)chunk;
                char* end  = base + chunk_capacity * element_size;
                if ((char*)p >= base && (char*)p < end)
                    return true;
            }
            return false;
        }

        size_t element_size;
        size_t chunk_capacity;
        std::vector<void*> chunks;
        std::vector<void*> freelist;

        ~PoolState() {
            for (void* chunk : chunks)
                ::operator delete(chunk);
        }
    };

    std::shared_ptr<PoolState> state;
};
