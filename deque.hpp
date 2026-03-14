#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {

    template<class T>
    class deque {
    public:
        struct Block{
            T* data;
            size_t first;
            size_t last;
            Block() : data(nullptr), first(0), last(0) {
                data = static_cast<T*>(::operator new(sizeof(T) * 16));
            }
            ~Block(){
                for (size_t i = first; i < last; i++){
                    data[i].~T();
                }
                ::operator delete(data);
            }
        };
        class const_iterator;
        class iterator {
        public:
            deque* deq;
            size_t block_index;
            size_t pos;
            /**
             * TODO add data members
             *   just add whatever you want.
             */
        public:
            /**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, the behaviour is **undefined**.
             * as well as operator-
             */
            iterator() : deq(nullptr), block_index(0), pos(0) {}
            iterator(deque* d, size_t b, size_t p) : deq(d), block_index(b), pos(p) {}

            iterator operator+(const int &n) const {
                int global_pos = (block_index - deq -> map_begin) * 16 + pos + n;
                size_t  new_block = deq -> map_begin + global_pos / 16;
                size_t new_pos = global_pos % 16;
                return iterator(deq, new_block, new_pos);
                //TODO
            }
            iterator operator-(const int &n) const {
                return *this + (-n);
                //TODO
            }
            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                if (deq != rhs.deq) throw invalid_iterator();
                int lhs_global = (block_index - deq -> map_begin) * 16 + pos;
                int rhs_global = (rhs.block_index - deq -> map_begin) * 16 + rhs.pos;
                return lhs_global - rhs_global;
                //TODO
            }
            iterator operator+=(const int &n) {
                *this = *this + n;
                return *this;
                //TODO
            }
            iterator operator-=(const int &n) {
                *this = *this - n;
                return *this;
                //TODO
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator tmp = *this;
                if (pos < deq -> map[block_index] -> last - 1){
                    pos++;
                } else {
                    block_index++;
                    pos = deq -> map[block_index] -> first;
                }
                return tmp;
            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                if (pos < deq -> map[block_index] -> last - 1){
                    pos++;
                } else {
                    block_index++;
                    pos = deq -> map[block_index] -> first;
                }
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp = *this;
                if (pos > deq -> map[block_index] -> first){
                    pos--;
                } else {
                    block_index--;
                    pos = deq -> map[block_index] -> last - 1;
                }
                return tmp;
            }
            /**
             * TODO --iter
             */
            iterator& operator--() {
                if (pos > deq -> map[block_index] -> first){
                    pos--;
                } else {
                    block_index--;
                    pos = deq -> map[block_index] -> last - 1;
                }
                return *this;
            }
            /**
             * TODO *it
             */
            T& operator*() const {
                return deq -> map[block_index] -> data[pos];
            }
            /**
             * TODO it->field
             */
            T* operator->() const noexcept {
                return &(deq -> map[block_index] -> data[pos]);
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const {
                if (deq == rhs.deq && block_index == rhs.block_index && pos == rhs.pos){
                    return true;
                }
                return false;
            }
            bool operator==(const const_iterator &rhs) const {
                if (deq == rhs.deq && block_index == rhs.block_index && pos == rhs.pos){
                    return true;
                }
                return false;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };
        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        public:
            const deque* deq;
            size_t block_index;
            size_t pos;
            // data members.
        public:
            const_iterator() : deq(nullptr), block_index(0), pos(0) {
                // TODO
            }
            const_iterator(const deque* d, size_t b, size_t p) : deq(d), block_index(b), pos(p) {}
            const_iterator(const iterator &other) : deq(other.deq), block_index(other.block_index), pos(other.pos) {
                // TODO
            }
            const_iterator operator+(const int &n) const {
                int global_pos = (block_index - deq -> map_begin) * 16 + pos + n;
                size_t new_block = deq -> map_begin + global_pos / 16;
                size_t new_pos = global_pos % 16;
                return const_iterator(deq, new_block, new_pos);
                //TODO
            }
            const_iterator operator-(const int &n) const {
                return *this + (-n);
                //TODO
            }
            int operator-(const const_iterator &rhs) const {
                if (deq != rhs.deq) throw invalid_iterator();
                int lhs_global = (block_index - deq -> map_begin) * 16 + pos;
                int rhs_global = (rhs.block_index - deq -> map_begin) * 16 + rhs.pos;
                return lhs_global - rhs_global;
                //TODO
            }
            const_iterator operator+=(const int &n) {
                *this = *this + n;
                return *this;
                //TODO
            }
            const_iterator operator-=(const int &n) {
                *this = *this - n;
                return *this;
                //TODO
            }
            const_iterator operator++(int) {
                const_iterator tmp = *this;
                if (pos < deq -> map[block_index] -> last - 1){
                    pos++;
                } else {
                    block_index++;
                    pos = deq -> map[block_index] -> first;
                }
                return tmp;
            }
            const_iterator& operator++() {
                if (pos < deq -> map[block_index] -> last - 1){
                    pos++;
                } else {
                    block_index++;
                    pos = deq -> map[block_index] -> first;
                }
                return *this;
            }
            const_iterator operator--(int) {
                const_iterator tmp = *this;
                if (pos > deq -> map[block_index] -> first){
                    pos--;
                } else {
                    block_index--;
                    pos = deq -> map[block_index] -> last - 1;
                }
                return tmp;
            }
            const_iterator& operator--() {
                if (pos > deq -> map[block_index] -> first){
                    pos--;
                } else {
                    block_index--;
                    pos = deq -> map[block_index] -> last - 1;
                }
                return *this;
            }
            const T& operator*() const {
                return deq -> map[block_index] -> data[pos];
            }
            const T* operator->() const noexcept {
                return &(deq -> map[block_index] -> data[pos]);
            }
            bool operator==(const iterator &rhs) const {
                if (deq == rhs.deq && block_index == rhs.block_index && pos == rhs.pos){
                    return true;
                }
                return false;
            }
            bool operator==(const const_iterator &rhs) const {
                if (deq == rhs.deq && block_index == rhs.block_index && pos == rhs.pos){
                    return true;
                }
                return false;
            }
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };
        /**
         * TODO Constructors
         */
    public:
        Block** map;
        size_t map_capacity;  //map的总容量
        size_t map_begin;  //第一个使用的块在map中的下标
        size_t map_end;  //最后一个使用的块的下一个下标
        size_t total_size;   //元素总数
        size_t start_pos;   //第一个元素在第一个块中的下标

    public:
        deque() : map(nullptr), map_capacity(0), map_begin(0), map_end(0), total_size(0), start_pos(0) {
            map = new Block*[1];
            map[0] = new Block();
            map_capacity = 1;
            map_begin = 0;
            map_end = 0;
        }
        deque(const deque &other) {
            map = new Block*[other.map_capacity];
            map_capacity = other.map_capacity;
            map_begin = other.map_begin;
            map_end = map_begin;
            total_size = other.total_size;
            start_pos = other.start_pos;
            for (size_t i = other.map_begin; i < other.map_end; i++){
                Block* src = other.map[i];
                Block* dst = new Block();
                dst -> first = src -> first;
                dst -> last = src -> last;
                for (size_t j = src -> first; j < src -> last; j++)
                    new (dst -> data + j) T(src -> data[j]);
                map[map_end++] = dst;
            }
        }
        /**
         * TODO Deconstructor
         */
        ~deque() {
            clear();
            delete [] map;
        }
        /**
         * TODO assignment operator
         */
        deque &operator=(const deque &other) {
            if (this == &other) return *this;
            clear();
            if (map_capacity < other.map_capacity){
                delete [] map;
                map = new Block*[other.map_capacity];
                map_capacity = other.map_capacity;
            }
            map_begin = other.map_begin;
            map_end = map_begin;
            total_size = other.total_size;
            start_pos = other.start_pos;
            for (size_t i = other.map_begin; i < other.map_end; i++){
                Block* src = other.map[i];
                Block* dst = new Block();
                dst -> first = src -> first;
                dst -> last = src -> last;
                for (size_t j = src -> first; j < src -> last; j++)
                    new (dst -> data + j) T(src -> data[j]);
                map[map_end++] = dst;
            }
            return *this;
        }
        /**
         * access specified element with bounds checking
         * throw index_out_of_bound if out of bound.
         */
        T & at(const size_t &pos) {
            if (pos >= total_size) throw index_out_of_bound();
            size_t global_pos = pos + start_pos;
            size_t block_num = map_begin + global_pos / 16;
            size_t p = global_pos % 16;
            return map[block_num] -> data[p];
        }
        const T & at(const size_t &pos) const {
            if (pos >= total_size) throw index_out_of_bound();
            size_t global_pos = pos + start_pos;
            size_t block_num = map_begin + global_pos / 16;
            size_t p = global_pos % 16;
            return map[block_num] -> data[p];
        }
        T & operator[](const size_t &pos) {
            return at(pos);
        }
        const T & operator[](const size_t &pos) const {
            return at(pos);
        }
        /**
         * access the first element
         * throw container_is_empty when the container is empty.
         */
        const T & front() const {
            if (empty()) throw container_is_empty();
            return map[map_begin] -> data[start_pos];
        }
        /**
         * access the last element
         * throw container_is_empty when the container is empty.
         */
        const T & back() const {
            if (empty()) throw container_is_empty();
            size_t last_block = map_end - 1;
            return map[last_block] -> data[map[last_block] -> last - 1];
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(this, map_begin, start_pos);
        }
        const_iterator cbegin() const {
            return const_iterator(this, map_begin, start_pos);
        }
        /**
         * returns an iterator to the end.
         */
        iterator end() {
            size_t global_pos = total_size + start_pos;
            size_t block_num = map_begin + global_pos / 16;
            size_t p = global_pos % 16;
            return iterator(this, block_num, p);
        }
        const_iterator cend() const {
            size_t global_pos = total_size + start_pos;
            size_t block_num = map_begin + global_pos / 16;
            size_t p = global_pos % 16;
            return const_iterator(this, block_num, p);
        }
        /**
         * checks whether the container is empty.
         */
        bool empty() const {
            if (total_size == 0) return true;
            return false;
        }
        /**
         * returns the number of elements
         */
        size_t size() const {
            return total_size;
        }
        /**
         * clears the contents
         */
        void clear() {
            for (size_t i = map_begin; i < map_end; i++){
                delete map[i];
            }
            map_begin = 0;
            map_end = 0;
            total_size = 0;
            start_pos = 0;
            if (map_capacity == 0){
                map = new Block*[1];
                map_capacity = 1;
            }
            map[0] = new Block();
        }

        void extend(){
            if (map_begin == 0 || map_end == map_capacity){
                size_t new_capacity = map_capacity * 2;
                Block** new_map = new Block*[new_capacity];
                size_t new_begin = (new_capacity - map_end + map_begin) / 2;
                for (size_t i = 0; i < map_end - map_begin; i++){
                    new_map[new_begin + i] = map[map_begin + i];
                }
                delete [] map;
                map = new_map;
                map_capacity = new_capacity;
                map_begin = new_begin;
                map_end = new_begin + map_end - map_begin;
            }
        }

        //用于在插入或删除时更新block的first/last
        void build(){
            if (total_size == 0){
                for (size_t i = map_begin; i < map_end; i++){
                    delete map[i];
                }
                map_begin = 0;
                map_end = 0;
                map[0] = new Block();
                start_pos = 0;
                return;
            }
            size_t curr = 0;
            for (size_t i = map_begin; i < map_end; i++){
                Block* block = map[i];
                size_t block_start = curr;
                size_t block_end = block_start + 16;
                if (block_end > total_size) block_end = total_size;
                block -> first = (block_start + start_pos) % 16;
                block -> last = block -> first + block_end - block_start;
                curr = block_end;
            }
        }

        /**
         * inserts elements at the specified locat on in the container.
         * inserts value before pos
         * returns an iterator pointing to the inserted value
         *     throw if the iterator is invalid or it point to a wrong place.
         */
        iterator insert(iterator position, const T &value) {
            if (this != position.deq) throw invalid_iterator();
            size_t global_pos = (position.block_index - map_begin) * 16 + position.pos - start_pos;
            if (global_pos > total_size) throw invalid_iterator();
            if (total_size == 0){
                Block* block = map[map_begin];
                new (block -> data) T(value);
                block -> first = 0;
                block -> last = 1;
                total_size = 1;
                map_end = map_begin + 1;
                start_pos = 0;
                return begin();
            }
            for (size_t i = total_size; i > global_pos; i--){
                size_t curr_block = map_begin + (i - 1) / 16;
                size_t curr_pos = (i - 1) % 16;
                size_t next_block = map_begin + i / 16;
                size_t next_pos = i % 16;
                if (next_block >= map_end){
                    extend();
                    Block* new_block = new Block();
                    new_block -> first = 0;
                    new_block -> last = 1;
                    map[map_end++] = new_block;
                }
                T* src = &map[curr_block] -> data[curr_pos];
                T* dst = &map[next_block] -> data[next_pos];
                new (dst) T(*src);
                src -> ~T();
            }
            size_t block_num = map_begin + global_pos / 16;
            size_t p = global_pos % 16;
            new (map[block_num] -> data + p) T(value);
            total_size++;
            build();
            return iterator(this, block_num, p);
        }
        /**
         * removes specified element at pos.
         * removes the element at pos.
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
         * throw if the container is empty, the iterator is invalid or it points to a wrong place.
         */
        iterator erase(iterator position) {
            if (this != position.deq) throw invalid_iterator();
            size_t global_pos = (position.block_index - map_begin) * 16 + position.pos - start_pos;
            if (global_pos > total_size) throw invalid_iterator();
            position -> ~T();
            for (size_t i = global_pos + 1; i < total_size; i++){
                size_t curr_block = map_begin + i / 16;
                size_t curr_pos = i % 16;
                size_t prev_block = map_begin + (i - 1) / 16;
                size_t prev_pos = (i - 1) % 16;
                T* src = &map[curr_block] -> data[curr_pos];
                T* dst = &map[prev_block] -> data[prev_pos];
                new (dst) T(*src);
                src -> ~T();
            }
            total_size--;
            if (map_end > map_begin){
                Block* last_block = map[map_end - 1];
                if (last_block -> first == last_block -> last){
                    delete last_block;
                    map_end--;
                }
            }
            build();
            if (global_pos == total_size) return end();
            size_t new_block = map_begin + global_pos / 16;
            size_t new_pos = global_pos % 16;
            return iterator(this, new_block, new_pos);
        }
        /**
         * adds an element to the end
         */
        void push_back(const T &value) {
            if (map_begin == map_end){
                Block* block = map[map_begin];
                new (block -> data) T(value);
                block -> first = 0;
                block -> last = 1;
                start_pos = 0;
                total_size = 1;
                map_end = map_begin + 1;
                return;
            }
            Block* last_block = map[map_end - 1];
            if (last_block -> last < 16){
                new (last_block -> data + last_block -> last) T(value);
                last_block -> last++;
                total_size++;
            } else {
                extend();
                Block* new_block = new Block();
                new_block -> first = 0;
                new_block -> last = 1;
                new (new_block -> data) T(value);
                map[map_end++] = new_block;
                total_size++;
            }
        }
        /**
         * removes the last element
         *     throw when the container is empty.
         */
        void pop_back() {
            if (empty()) throw container_is_empty();
            Block* last_block = map[map_end - 1];
            last_block -> last--;
            (last_block -> data + last_block -> last) -> ~T();
            total_size--;
            if (last_block -> first == last_block -> last){
                delete last_block;
                map_end--;
                if (map_begin == map_end){
                    map[0] = new Block();
                    map_begin = 0;
                    map_end = 0;
                    start_pos = 0;
                }
            }
        }
        /**
         * inserts an element to the beginning.
         */
        void push_front(const T &value) {
            if (map_begin == map_end){
                Block* block = map[map_begin];
                new (block -> data) T(value);
                block -> first = 0;
                block -> last = 1;
                start_pos = 0;
                total_size = 1;
                map_end = map_begin + 1;
                return;
            }
            Block* first_block = map[map_begin];
            if (first_block -> first > 0){
                first_block -> first--;
                new (first_block -> data + first_block -> first) T(value);
                start_pos--;
                total_size++;
            } else {
                extend();
                map_begin--;
                Block* new_block = new Block();
                new_block -> first = 15;
                new_block -> last = 16;
                new (new_block -> data + new_block ->first) T(value);
                map[map_begin] = new_block;
                start_pos = 15;
                total_size++;
            }
        }
        /**
         * removes the first element.
         *     throw when the container is empty.
         */
        void pop_front() {
            if (empty()) throw container_is_empty();
            Block* first_block = map[map_begin];
            (first_block -> data + start_pos) -> ~T();
            start_pos++;
            first_block -> first++;
            total_size--;
            if (first_block -> first == first_block -> last){
                delete first_block;
                map_begin++;
                if (map_begin < map_end){
                    start_pos = map[map_begin] -> first;
                } else {
                    map[0] = new Block();
                    map_begin = 0;
                    map_end = 0;
                    start_pos = 0;
                }
            }
        }
    };

}

#endif
