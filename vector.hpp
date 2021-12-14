#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <iostream>

namespace sjtu {
    
template<typename T>
class vector {
public:
    T ** store;
    size_t capacity;
    size_t tail;
    class const_iterator;
    class iterator {
    private:
        T** iter;
        const vector* v; 
    public:
        iterator():v(nullptr){iter = nullptr;}
        iterator(const iterator &other):v(other.v){iter = other.iter;}
        iterator(const vector* vect,const size_t pos):v(vect),iter(&((*vect).store[pos])){}
        const_iterator operator=(iterator rhs){return {rhs.iter, rhs.v};}
        const vector* getv(){return v;}
        T** getiter(){return iter;}
        iterator operator+(const int &n) const {
            iterator res(*this);
            res.iter += n;
            return res;
        }
        iterator operator-(const int &n) const {
            iterator res(*this);
            res.iter -= n;
            return res;
        }
        int operator-(const iterator &rhs) const {
            if (v != rhs.v) throw invalid_iterator();
            else {return (iter - rhs.iter);}
        }
        iterator& operator+=(const int &n) {
            (*this).iter += n;
            return *this;
        }
        iterator& operator-=(const int &n) {
            (*this).iter -= n;
            return *this;
        }
        iterator operator++(int) {
            iterator res(*this);
            ++(*this).iter;
            return res;
        }
        iterator& operator++() {
            ++(*this).iter;
            return *this;
        }
        iterator operator--(int) {
            iterator res(*this);
            --(*this).iter;
            return res;
        }
        iterator& operator--() {
            --(*this).iter;
            return *this;
        }
        T& operator*() const{return **iter;}
        bool operator==(const iterator &rhs) const {return (iter == rhs.iter);}
        bool operator==(const const_iterator &rhs) const {return (iter == rhs.iter);}
        bool operator!=(const iterator &rhs) const {return (iter != rhs.iter);}
        bool operator!=(const const_iterator &rhs) const {return (iter != rhs.iter);}
    };
    class const_iterator {
        T** iter;
        const vector* v;
        public:
        const_iterator():v(nullptr),iter(nullptr){}
        const_iterator(const iterator &other):v(other.getv()),iter(other.getiter()){}
        const_iterator(const const_iterator &other):v(other.v),iter(other.iter){}
        const_iterator(const vector* vect,const size_t pos):v(vect),iter(&((*vect).store[pos])){}
        const vector* getv(){return v;}
        const T** getiter(){return iter;}
        iterator operator=(const_iterator rhs){return {rhs.iter, rhs.v};}

        const_iterator operator+(const int &n) const {
            const_iterator res(*this);
            res.iter += n;
            return res;
        }
        const_iterator operator-(const int &n) const {
            const_iterator res(*this);
            res.iter -= n;
            return res;
        }
        int operator-(const const_iterator &rhs) const {
            if (v != rhs.v) throw invalid_iterator();
            else {return (iter - rhs.iter);}
        }
        const_iterator& operator+=(const int &n) {
            (*this).iter += n;
            return *this;
        }
        const_iterator& operator-=(const int &n) {
            (*this).iter -= n;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator res(*this);
            ++(*this).iter;
            return res;
        }
        const_iterator& operator++() {
            ++(*this).iter;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator res(*this);
            --(*this).iter;
            return res;
        }
        const_iterator& operator--() {
            --(*this).iter;
            return *this;
        }
        const T& operator*() const{return **iter;}
        bool operator==(const iterator &rhs) const {return (iter == rhs.iter);}
        bool operator==(const const_iterator &rhs) const {return (iter == rhs.iter);}
        bool operator!=(const iterator &rhs) const {return (iter != rhs.iter);}
        bool operator!=(const const_iterator &rhs) const {return (iter != rhs.iter);}
    };
    vector(){capacity = 10;store = new T*[capacity];tail = 0;}
    vector(const vector &other) {
        capacity = other.capacity;
        tail = other.tail;
        store = new T*[capacity];
        for (int i = 0;i < tail;++i) store[i] = new T {other[i]};
    }
    ~vector() {clear();delete []store;}
    vector &operator=(const vector &other) {
        if (this == &other) return *this;
        for (int i = 0;i < tail;++i) delete store[i];
        delete []store;
        capacity = other.capacity;tail = other.tail;store = new T*[capacity];
        for (int i = 0;i < tail;++i) store[i] = new T {other[i]};
        return *this;
    }
    T & operator[](const size_t &pos) {if (pos < 0 || pos >= tail) throw index_out_of_bound();else return **(store + pos);}
    const T & operator[](const size_t &pos) const {if (pos < 0 || pos >= tail) throw index_out_of_bound();else return **(store + pos);}
    T & at(const size_t &pos) {if (pos < 0 || pos >= tail) throw index_out_of_bound(); else return (*this)[pos];}
    const T & at(const size_t &pos) const {if (pos < 0 || pos >= tail) throw index_out_of_bound(); else return (*this)[pos];}
    const T & front() const {if (tail == 0) throw container_is_empty();else return (*this)[0];}
    const T & back() const {if (tail == 0) throw container_is_empty();else return (*this)[tail - 1];}
    iterator begin() {return {this, 0};}
    const_iterator cbegin() const {return {this, 0};}
    iterator end() {return {this,tail};}
    const_iterator cend() const {return {this,tail};}
    bool empty() const {return (!tail);}
    size_t size() const {return tail;}
    void clear() {
        for (int i = 0;i < tail;++i) delete store[i];
        tail = 0;
    }
    iterator insert(iterator pos, const T &value) {
        if (pos.getv() != this) return pos;
        T** store_ = new T*[capacity + 1];
        int iter = 0;
        for (;iter < pos - (*this).begin();++iter) store_[iter] = new T {(*this)[iter]};
        size_t res = iter;
        store_[iter++] = new T {value};
        for (;iter <= tail;++iter) store_[iter] = new T {(*this)[iter - 1]};
        for (int i = 0;i < tail;++i) delete store[i];
        delete []store;
        store = store_;
        ++tail;
        ++capacity;
        return {this,res};
    }
    iterator insert(const size_t &ind, const T &value) {
        if (ind > tail) {throw index_out_of_bound();return (*this).end();}
        T** store_ = new T* [capacity + 1];
        int iter = 0;
        for (;iter < ind;++iter) store_[iter] = new T {(*this)[iter]};
        store_[iter++] = value;
        for (;iter <= tail;++iter) store_[iter] = new T {(*this)[iter - 1]};
        for (int i = 0;i < tail;++i) delete store[i];
        delete []store;
        store = store_;
        ++tail;
        ++capacity;
        return {this,ind};
    }
    iterator erase(iterator pos) {
        if (pos.getv() != this) return pos;
        size_t ind = pos - (*this).begin();
        if (ind >= tail) return pos;
        T** store_ = new T* [capacity - 1];
        int iter = 0;
        for (;iter < ind;++iter) store_[iter] = new T {(*this)[iter]};
        for (;iter < tail - 1;++iter) store_[iter] = new T {(*this)[iter + 1]};
        for (int i = 0;i < tail;++i) delete store[i];
        delete []store;
        store = store_;
        --tail;
        --capacity;
        return {this,ind};
    }
    iterator erase(const size_t &ind) {
        if (ind > tail) {throw index_out_of_bound();return (*this).end();}
        if (ind == tail) return {this,tail};
        T** store_ = new T*[capacity - 1];
        int iter = 0;
        for (;iter < ind;++iter) store_[iter] = new T {(*this)[iter]};
        for (;iter < tail - 1;++iter) store_[iter] = new T {(*this)[iter + 1]};
        for (int i = 0;i < tail;++i) delete store[i];
        delete []store;
        store = store_;
        --tail;
        --capacity;
        return {this,ind};
    }
    void push_back(const T &value) {
        if (capacity == tail) expand();
        (*this).store[tail] = new T {value};
        ++tail;
        }
    void pop_back() {
        if (tail == 0) throw container_is_empty();
        else {
            delete store[tail - 1];
            --tail;
            shrink();
        }
    }

    private: 
    void expand(){//扩大容器容量（默认double）
        capacity *= 2;
        T** store_ = new T*[capacity];
        for (int i = 0;i < tail;++i) {
            store_[i] = new T {(*this)[i]};
            delete store[i];
        }
        delete []store;
        store = store_;
    }
    void shrink(){//缩小容器容量（默认half）
        if (tail == 0) capacity = 10;
        else if (capacity / tail <= 4) return;
        capacity /= 2;
        T** store_ = new T*[capacity];
        for (int i = 0;i < tail;++i) {
            store_[i] = new T {(*this)[i]};
            delete store[i];
        }
        delete []store;
        store = store_;
    }
    public:
    void display(){
        if (empty()) {
            std::cout << "*\nvector is EMPTY";
        }
        else {
            std::cout << "*\nstorage = " ;
            for (int i = 0;i < tail;++i)  std::cout << store[i] << ",";
            std::cout << "\nnonempty element size is " << tail;  
        }
        std::cout <<"\ncapacity is " << capacity << "\n*\n";
    }
};


}

#endif