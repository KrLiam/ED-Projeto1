// Copyright [2023] <William Kraus>
#ifndef STRUCTURES_ARRAY_LIST_H
#define STRUCTURES_ARRAY_LIST_H

#include <cstdint>
#include <stdexcept>


namespace structures {

template<typename T>
class ArrayList {
 public:
    ArrayList();
    explicit ArrayList(std::size_t max_size);
    ~ArrayList();

    void clear();
    void push_back(const T& data);
    void push_front(const T& data);
    void insert(const T& data, std::size_t index);
    void insert_sorted(const T& data);
    T pop(std::size_t index);
    T pop_back();
    T pop_front();
    void remove(const T& data);
    bool full() const;
    bool empty() const;
    bool contains(const T& data) const;
    std::size_t find(const T& data) const;
    std::size_t size() const;
    std::size_t max_size() const;
    T& at(std::size_t index);
    T& operator[](std::size_t index);
    const T& at(std::size_t index) const;
    const T& operator[](std::size_t index) const;
    // descricao do 'operator []' na FAQ da disciplina

 private:
    T* contents;
    std::size_t size_;
    std::size_t max_size_;

    static const auto DEFAULT_MAX = 10u;
};

}  // namespace structures

#endif
template<typename T>
structures::ArrayList<T>::ArrayList() {
    max_size_ = DEFAULT_MAX;
    contents = new T[max_size_];
    size_ = 0;
}

template<typename T>
structures::ArrayList<T>::ArrayList(std::size_t max) {
    max_size_ = max;
    contents = new T[max_size_];
    size_ = 0;
}

template<typename T>
structures::ArrayList<T>::~ArrayList() {
    delete [] contents;
}

template<typename T>
void structures::ArrayList<T>::clear() {
    size_ = 0;
}

template<typename T>
void structures::ArrayList<T>::push_back(const T& data) {
    insert(data, size_);
}

template<typename T>
void structures::ArrayList<T>::push_front(const T& data) {
    insert(data, 0);
}

template<typename T>
void structures::ArrayList<T>:: insert(const T& data, std::size_t index) {
    if (full())
        throw std::out_of_range("lista cheia");

    if (index > size_)
        throw std::out_of_range("index fora do limite da lista");

    int size_int = size_;
    int index_int = index;
    for (int i = size_int - 1; i >= index_int; i--) {
        contents[i+1] = contents[i];
    }

    contents[index] = data;
    size_++;
}

template<typename T>
void structures::ArrayList<T>:: insert_sorted(const T& data) {
    for (std::size_t i = 0; i < size_; i++) {
        if (data < contents[i]) {
            insert(data, i);
            return;
        }
    }
    push_back(data);
}

template<typename T>
T structures::ArrayList<T>:: pop(std::size_t index) {
    if (empty())
        throw std::out_of_range("lista vazia");

    if (index >= size_)
        throw std::out_of_range("index fora do limite da lista");

    T tmp = contents[index];

    for (std::size_t i = index + 1; i < size_; i++) {
        contents[i-1] = contents[i];
    }

    size_--;

    return tmp;
}

template<typename T>
T structures::ArrayList<T>:: pop_back() {
    return pop(size_ - 1);
}

template<typename T>
T structures::ArrayList<T>:: pop_front() {
    return pop(0);
}

template<typename T>
void structures::ArrayList<T>:: remove(const T& data) {
    for (std::size_t i = 0; i < size_; i++) {
        if (contents[i] == data) {
            pop(i);
            break;
        }
    }
}

template<typename T>
bool structures::ArrayList<T>::full() const {
    return size() == max_size_;
}

template<typename T>
bool structures::ArrayList<T>::empty() const {
    return size() == 0;
}

template<typename T>
bool structures::ArrayList<T>::contains(const T& data) const {
    return find(data) != size_;
}

template<typename T>
std::size_t structures::ArrayList<T>::find(const T& data) const {
    for (std::size_t i = 0; i < size_; i++) {
        if (contents[i] == data) {
            return i;
        }
    }
    return size_;
}

template<typename T>
std::size_t structures::ArrayList<T>::size() const {
    return size_;
}

template<typename T>
std::size_t structures::ArrayList<T>::max_size() const {
    return max_size_;
}

template<typename T>
const T& structures::ArrayList<T>::at(std::size_t index) const {
    if (0 > index || index >= size_)
        throw std::out_of_range("index fora do limite da lista");

    return contents[index];
}
template<typename T>
T& structures::ArrayList<T>::at(std::size_t index) {
    if (0 > index || index >= size_)
        throw std::out_of_range("index fora do limite da lista");

    return contents[index];
}

template<typename T>
const T& structures::ArrayList<T>::operator[](std::size_t index) const {
    return at(index);
}
template<typename T>
T& structures::ArrayList<T>::operator[](std::size_t index) {
    return at(index);
}
