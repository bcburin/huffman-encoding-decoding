#ifndef BITDEQUEUE_H
#define BITDEQUEUE_H

#include "bitarray.h"
#include <utility>


template <typename W = unsigned long>
class bitdequeue: public bitarray<W> {
  private:
    int start_;
    int end_;
    inline size_t mod(size_t index) const;
  public:
    bitdequeue(size_t capacity = 1);
    bitdequeue(std::initializer_list<bool> data);
    bitdequeue(const bitdequeue<W>& other);
    bitdequeue(bitdequeue<W>&& other);
    bitdequeue<W>& operator= (const bitdequeue<W>& other);
    bitdequeue<W>& operator= (bitdequeue<W>&& other);
    bool operator== (const bitdequeue<W>& other);
    bool operator!= (const bitdequeue<W>& other);
    void resize(size_t new_capacity);
    void fit();
    bitdequeue<W>& push_back(bool bit);
    bitdequeue<W>& push_front(bool bit);
    bool pop_back();
    bool pop_front();
    bool back() const;
    bool front() const;
    bitdequeue<W>& push_back(bitdequeue<W> other);
    bitdequeue<W>& push_front(bitdequeue<W> other);
    size_t size() const { return start_ < 0 ? 0 : mod(end_-start_)+1; }
    bool empty() const { return size() == 0; }
    bool full() const { return size() == bitarray<W>::capacity(); }
    bool operator[] (size_t index) const { return bitarray<W>::get(mod(start_+index)); }
    template <typename WW>
    friend std::ostream& operator<< (std::ostream& os, bitdequeue<WW>& bits);
    template <typename WW>
    friend std::istream& operator>> (std::istream& is, bitdequeue<WW>& bits);
};

template <typename W = unsigned long>
std::ostream& operator<< (std::ostream& os, bitdequeue<W>& bits) {
  bits.fit();
  size_t word_capacity = bits.word_capacity();
  os.write((char*) &word_capacity, sizeof(size_t));
  for(int w = 0; w != bits.word_capacity(); ++w)
    os.write((char*) &bits.words_[w], sizeof(W));
  return os;
}


template <typename W = unsigned long>
std::istream& operator>> (std::istream& is, bitdequeue<W>& bits) {
  size_t word_capacity;
  is.read((char*) &word_capacity, sizeof(size_t));
  bits.resize(word_capacity * bits.word_size);
  for(int w = 0; w != bits.word_capacity(); ++w)
    is.read((char*) &bits.words_[w], sizeof(W));
  bits.start_ = 0;
  bits.end_ = bits.word_capacity() * bits.word_size - 1;
  return is;
}


template <typename W>
bitdequeue<W>::bitdequeue(size_t capacity)
: bitarray<W>(capacity), start_(-1), end_(-1) {}


template <typename W>
bitdequeue<W>::bitdequeue(std::initializer_list<bool> data)
: start_(-1), end_(-1) { for(const auto& x : data) push_back(x); }


template <typename W>
bitdequeue<W>::bitdequeue(const bitdequeue<W>& other)
: bitarray<W>(other), start_(other.start_), end_(other.end_) {}


template <typename W>
bitdequeue<W>::bitdequeue(bitdequeue<W>&& other)
: bitarray<W>(std::move(other)), start_(other.start_), end_(other.end_) {}


template <typename W>
bitdequeue<W>& bitdequeue<W>::operator= (const bitdequeue<W>& other) {
  bitarray<W>::operator= (other);
  start_ = other.start_;
  end_ = other.end_;
  return *this;
}


template <typename W>
bitdequeue<W>& bitdequeue<W>::operator= (bitdequeue<W>&& other) {
  bitarray<W>::operator= (std::move(other));
  start_ = other.start_;
  end_ = other.end_;
  return *this;
}


template <typename W>
bool bitdequeue<W>::operator== (const bitdequeue<W>& other) {
  if(size() != other.size()) return false;
  for(int i = 0; i != size(); ++i) {
    if((*this)[i] != other[i]) return false;
  }
  return true;
}


template <typename W>
bool bitdequeue<W>::operator!= (const bitdequeue<W>& other) { return !(*this == other); }


template <typename W>
void bitdequeue<W>::resize(size_t new_capacity) {
  bitdequeue<W> temp(*this);
  bitarray<W>::resize(new_capacity);
  start_ = end_ = -1;
  while(!temp.empty()) push_back(temp.pop_front());
}


template <typename W>
void bitdequeue<W>::fit() { resize(size()); }


template <typename W>
bitdequeue<W>& bitdequeue<W>::push_back(bool bit) {
  if(full()) resize(bitarray<W>::capacity()*2);
  if(empty()) start_ = end_ = 0;
  else end_ = mod(end_ + 1);
  bit ? bitarray<W>::set(end_) : bitarray<W>::reset(end_);
  return *this;
}


template <typename W>
bitdequeue<W>& bitdequeue<W>::push_front(bool bit) {
  if(full()) resize(bitarray<W>::capacity()*2);
  if(empty()) start_ = end_ = 0;
  else start_ = mod(start_ - 1);
  bit ? bitarray<W>::set(start_) : bitarray<W>::reset(start_);
  return *this;
}


template <typename W>
bool bitdequeue<W>::pop_back() {
  if(empty()) throw std::underflow_error("Empty bit queue");
  int index = end_;
  if(size() == 1) start_ = end_ = -1;
  else end_ = mod(end_ - 1);
  return bitarray<W>::get(index);
}


template <typename W>
bool bitdequeue<W>::pop_front() {
  if(empty()) throw std::underflow_error("Empty bit queue");
  int index = start_;
  if(size() == 1) start_ = end_ = -1;
  else start_ = mod(start_ + 1);
  return bitarray<W>::get(index);
}


template <typename W>
bool bitdequeue<W>::back() const { return bitarray<W>::get(end_); }


template <typename W>
bool bitdequeue<W>::front() const { return bitarray<W>::get(start_); }


template <typename W>
bitdequeue<W>& bitdequeue<W>::push_back(bitdequeue<W> other) {
  while(!other.empty()) push_back(other.pop_front());
  return *this;
}


template <typename W>
bitdequeue<W>& bitdequeue<W>::push_front(bitdequeue<W> other) {
  while(!other.empty()) push_front(other.pop_back());
  return *this;
}


template <typename W>
size_t bitdequeue<W>::mod(size_t index) const { 
  int md = index%bitarray<W>::capacity();
  return md < 0 ? md + bitarray<W>::capacity() : md;
}

#endif