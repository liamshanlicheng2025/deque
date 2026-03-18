#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"
#include <cstddef>

namespace sjtu {

    template<class T>
    class deque {
    private:
        T** map;
        size_t map_capacity;
        size_t first_block;
        size_t first_pos;
        size_t last_block;
        size_t last_pos;
        size_t total_size;

        void extend(size_t front, size_t back) {
            size_t used_block_num = last_block - first_block + 1;
            size_t new_capacity = map_capacity * 2;
            if (new_capacity == 0) new_capacity = 8;
            while (new_capacity < used_block_num + front + back)
                new_capacity *= 2;
            T** new_map = new T*[new_capacity];
            for (size_t i = 0; i < new_capacity; i++)
                new_map[i] = nullptr;
            size_t new_first_block = (new_capacity - used_block_num) / 2;
            for (size_t i = 0; i < used_block_num; i++)
                new_map[new_first_block + i] = map[first_block + i];
            delete [] map;
            map = new_map;
            map_capacity = new_capacity;
            last_block = new_first_block + used_block_num - 1;
            first_block = new_first_block;
        }

        T* element_ptr(size_t index) {
            size_t global = first_pos + index;
            size_t block_index = first_block + global / 16;
            size_t offset = global % 16;
            return &map[block_index][offset];
        }
        const T* element_ptr(size_t index) const {
            size_t global = first_pos + index;
            size_t block_index = first_block + global / 16;
            size_t offset = global % 16;
            return &map[block_index][offset];
        }

    public:
        class const_iterator;
        class iterator {
        public:
            deque* deq;
            T* ptr;
            size_t block;

            iterator(deque* d = nullptr, size_t b = 0, T* p = nullptr) : deq(d), block(b), ptr(p) {}

            size_t get_index() const {
                if (ptr == nullptr) return deq->total_size;
                size_t block_offset = block - deq->first_block;
                size_t offset = ptr - deq->map[block];
                return block_offset * 16 + offset - deq->first_pos;
            }

            iterator operator+(const int &n) const {
                iterator tmp = *this;
                tmp += n;
                return tmp;
            }
            iterator operator-(const int &n) const {
                iterator tmp = *this;
                tmp -= n;
                return tmp;
            }
            int operator-(const iterator &rhs) const {
                if (deq != rhs.deq) throw invalid_iterator();
                return static_cast<int>(get_index() - rhs.get_index());
            }
            iterator operator+=(const int &n) {
                if (n == 0) return *this;
                int new_idx = static_cast<int>(get_index()) + n;
                if (new_idx < 0) new_idx = 0;
                if (static_cast<size_t>(new_idx) >= deq->total_size) {
                    *this = deq->end();
                    return *this;
                }
                size_t global = deq->first_pos + new_idx;
                size_t new_block = deq->first_block + global / 16;
                size_t new_offset = global % 16;
                block = new_block;
                ptr = deq->map[new_block] + new_offset;
                return *this;
            }
            iterator operator-=(const int &n) {
                return *this += (-n);
            }
            iterator operator++(int) {
                iterator tmp = *this;
                ++*this;
                return tmp;
            }
            iterator& operator++() {
                *this += 1;
                return *this;
            }
            iterator operator--(int) {
                iterator tmp = *this;
                --*this;
                return tmp;
            }
            iterator& operator--() {
                *this += -1;
                return *this;
            }
            T& operator*() const {
                return *ptr;
            }
            T* operator->() const noexcept {
                return ptr;
            }
            bool operator==(const iterator &rhs) const {
                return (deq == rhs.deq && ptr == rhs.ptr);
            }
            bool operator==(const const_iterator &rhs) const;
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const;
        };

        class const_iterator {
        public:
            const deque* deq;
            const T* ptr;
            size_t block;

            const_iterator(const deque* d = nullptr, size_t b = 0, const T* p = nullptr) : deq(d), block(b), ptr(p) {}
            const_iterator(const iterator &other) : deq(other.deq), block(other.block), ptr(other.ptr) {}

            size_t get_index() const {
                if (ptr == nullptr) return deq->total_size;
                size_t block_offset = block - deq->first_block;
                size_t offset = ptr - deq->map[block];
                return block_offset * 16 + offset - deq->first_pos;
            }

            const_iterator operator+(const int &n) const {
                const_iterator tmp = *this;
                tmp += n;
                return tmp;
            }
            const_iterator operator-(const int &n) const {
                const_iterator tmp = *this;
                tmp -= n;
                return tmp;
            }
            int operator-(const const_iterator &rhs) const {
                if (deq != rhs.deq) throw invalid_iterator();
                return static_cast<int>(get_index() - rhs.get_index());
            }
            const_iterator operator+=(const int &n) {
                if (n == 0) return *this;
                int new_idx = static_cast<int>(get_index()) + n;
                if (new_idx < 0) new_idx = 0;
                if (static_cast<size_t>(new_idx) >= deq->total_size) {
                    *this = deq->cend();
                    return *this;
                }
                size_t global = deq->first_pos + new_idx;
                size_t new_block = deq->first_block + global / 16;
                size_t new_offset = global % 16;
                block = new_block;
                ptr = deq->map[new_block] + new_offset;
                return *this;
            }
            const_iterator operator-=(const int &n) {
                return *this += (-n);
            }
            const_iterator operator++(int) {
                const_iterator tmp = *this;
                ++*this;
                return tmp;
            }
            const_iterator& operator++() {
                *this += 1;
                return *this;
            }
            const_iterator operator--(int) {
                const_iterator tmp = *this;
                --*this;
                return tmp;
            }
            const_iterator& operator--() {
                *this += -1;
                return *this;
            }
            const T& operator*() const {
                return *ptr;
            }
            const T* operator->() const noexcept {
                return ptr;
            }
            bool operator==(const iterator &rhs) const {
                return (deq == rhs.deq && ptr == rhs.ptr);
            }
            bool operator==(const const_iterator &rhs) const {
                return (deq == rhs.deq && ptr == rhs.ptr);
            }
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        friend class iterator;
        friend class const_iterator;

        deque() : map(nullptr), map_capacity(0), first_block(0), first_pos(0),
                  last_block(0), last_pos(0), total_size(0) {}

        deque(const deque &other) : map(nullptr), map_capacity(0), first_block(0),
                                    first_pos(0), last_block(0), last_pos(0), total_size(0) {
            for (size_t i = 0; i < other.total_size; ++i) {
                push_back(other[i]);
            }
        }

        ~deque() {
            clear();
            if (map){
                for (size_t i = 0; i < map_capacity; i++){
                    if (map[i]){
                        ::operator delete(map[i]);
                    }
                }
                delete [] map;
            }
        }

        deque &operator=(const deque &other) {
            if (this == &other) return *this;
            clear();
            for (size_t i = 0; i < other.total_size; ++i) {
                push_back(other[i]);
            }
            return *this;
        }

        T & at(const size_t &pos) {
            if (pos >= total_size) throw index_out_of_bound();
            size_t global = first_pos + pos;
            size_t block_idx = first_block + global / 16;
            size_t offset = global % 16;
            return map[block_idx][offset];
        }
        const T & at(const size_t &pos) const {
            if (pos >= total_size) throw index_out_of_bound();
            size_t global = first_pos + pos;
            size_t block_idx = first_block + global / 16;
            size_t offset = global % 16;
            return map[block_idx][offset];
        }
        T & operator[](const size_t &pos) { return at(pos); }
        const T & operator[](const size_t &pos) const { return at(pos); }

        const T & front() const {
            if (empty()) throw container_is_empty();
            return map[first_block][first_pos];
        }
        const T & back() const {
            if (empty()) throw container_is_empty();
            size_t b = last_block, p = last_pos;
            if (p == 0) {
                if (b != first_block) b--;
                p = 16;
            }
            return map[b][p - 1];
        }

        iterator begin() {
            if (empty()) return iterator(this, 0, nullptr);
            return iterator(this, first_block, map[first_block] + first_pos);
        }
        const_iterator cbegin() const {
            if (empty()) return const_iterator(this, 0, nullptr);
            return const_iterator(this, first_block, map[first_block] + first_pos);
        }
        iterator end() {
            if (empty()) return iterator(this, 0, nullptr);
            return iterator(this, last_block, map[last_block] + last_pos);
        }
        const_iterator cend() const {
            if (empty()) return const_iterator(this, 0, nullptr);
            return const_iterator(this, last_block, map[last_block] + last_pos);
        }

        bool empty() const { return total_size == 0; }
        size_t size() const { return total_size; }

        void clear() {
            while (!empty()){
                pop_back();
            }
            if (map){
                for (size_t i = 0; i < map_capacity; i++){
                    if (map[i]){
                        ::operator delete(map[i]);
                    }
                }
                delete [] map;
                map_capacity = 0;
                map = nullptr;
            }
        }

        iterator insert(iterator pos, const T &value) {
            if (pos.deq != this) throw invalid_iterator();
            if (pos == end()){
                push_back(value);
                return iterator(this, last_block, map[last_block] + last_pos - 1);
            }
            size_t index = pos - begin();
            T tmp = value;
            push_back(tmp);
            for (size_t i = total_size - 2; i > index; --i) {
                T* src = element_ptr(i - 1);
                T* dst = element_ptr(i);
                new (dst) T(*src);
                src->~T();
            }
            T* target = element_ptr(index);
            new (target) T(tmp);
            size_t global = first_pos + index;
            size_t block_index = first_block + global / 16;
            size_t offset = global % 16;
            return iterator(this, block_index, map[block_index] + offset);
        }

        iterator erase(iterator pos) {
            if (pos.deq != this) throw invalid_iterator();
            if (empty()) throw container_is_empty();
            if (pos == end()) throw invalid_iterator();
            size_t index = pos - begin();
            for (size_t i = index; i < total_size - 1; i++){
                T* src = element_ptr(i + 1);
                T* dst = element_ptr(i);
                new (dst) T(*src);
                src->~T();
            }
            T* last = element_ptr(total_size - 1);
            last->~T();
            total_size--;
            if (last_pos == 0) {
                if(last_block != first_block) last_block--;
                last_pos = 16;
            }
            last_pos--;
            if (empty()) return end();
            if (index == total_size) return end();
            size_t global = first_pos + index;
            size_t block_index = first_block + global / 16;
            size_t offset = global % 16;
            return iterator(this, block_index, map[block_index] + offset);
        }

        void push_back(const T &value) {
            if (total_size == 0){
                map_capacity = 8;
                map = new T*[map_capacity];
                for (size_t i = 0; i < map_capacity; i++)
                    map[i] = nullptr;
                first_block = last_block = map_capacity / 2;
                first_pos = last_pos = 0;
                map[first_block] = static_cast<T*>(::operator new(16 * sizeof(T)));
            }
            if (last_pos == 16){
                if (last_block + 1 < map_capacity && map[last_block + 1] != nullptr){
                    last_block++;
                    last_pos = 0;
                } else {
                    if (last_block + 1 >= map_capacity){
                        extend(0, 1);
                    }
                    map[last_block + 1] = static_cast<T*>(::operator new(16 * sizeof(T)));
                    last_block++;
                    last_pos = 0;
                }
            }
            new (map[last_block] + last_pos) T(value);
            last_pos++;
            total_size++;
        }

        void pop_back() {
            if (empty()) throw container_is_empty();
            if (last_pos == 0){
                last_block--;
                last_pos = 16;
            }
            last_pos--;
            (map[last_block] + last_pos)->~T();
            total_size--;
        }

        void push_front(const T &value) {
            if (total_size == 0){
                map_capacity = 8;
                map = new T*[map_capacity];
                for (size_t i = 0; i < map_capacity; i++)
                    map[i] = nullptr;
                first_block = last_block = map_capacity / 2;
                first_pos = last_pos = 0;
                map[first_block] = static_cast<T*>(::operator new(16 * sizeof(T)));
                new (map[first_block] + first_pos) T(value);
                last_pos = first_pos + 1;  // 关键修正：更新尾部位置
                total_size++;
                return;
            }
            if (first_pos > 0){
                first_pos--;
                new (map[first_block] + first_pos) T(value);
                total_size++;
                return;
            }
            if (first_block > 0 && map[first_block - 1] != nullptr) {
                first_block--;
                first_pos = 15;
                new (map[first_block] + first_pos) T(value);
                total_size++;
                return;
            }
            if (first_block == 0){
                extend(1, 0);
            }
            map[first_block - 1] = static_cast<T*>(::operator new(16 * sizeof(T)));
            first_block--;
            first_pos = 15;
            new (map[first_block] + first_pos) T(value);
            total_size++;
        }

        void pop_front() {
            if (empty()) throw container_is_empty();
            map[first_block][first_pos].~T();
            first_pos++;
            if (first_pos == 16){
                first_block++;
                first_pos = 0;
            }
            total_size--;
            if (total_size == 0){
                first_block = last_block = map_capacity / 2;
                first_pos = last_pos = 0;
            }
        }
    };

    template<class T>
    bool deque<T>::iterator::operator==(const deque<T>::const_iterator &rhs) const {
        return deq == rhs.deq && ptr == rhs.ptr;
    }
    template<class T>
    bool deque<T>::iterator::operator!=(const deque<T>::const_iterator &rhs) const {
        return !(*this == rhs);
    }

} // namespace sjtu

#endif
