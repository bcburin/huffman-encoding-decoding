#ifndef BITARRAY_H
#define BITARRAY_H

#include <ostream>
#include <climits>
#include <stdexcept>

#include <iostream>

using std::size_t;

template <typename W = unsigned long>
class bitarray {
  protected:
    static const size_t word_size = CHAR_BIT * sizeof(W);  // number of bits a word holds
    size_t bit_capacity_;  // maximum number of bits
    W* words_;
    size_t words_necessary_for(size_t size) const { return (size-1)/word_size + 1; }
    static W bitmask_at(std::size_t index) { return 1U << index; }
  public:
    bitarray(size_t capacity = 8);
    bitarray(const bitarray<W>& other);
    bitarray(bitarray<W>&& other);
    bitarray<W>& operator= (const bitarray<W>& other); 
    bitarray<W>& operator= (bitarray<W>&& other); 
    void resize(size_t new_capacity);
    void set();
    void reset();
    void set(size_t index);
    void reset(size_t index);
    bool get(size_t index) const;
    W word(size_t index) const { return words_[index]; }
    size_t capacity() const { return bit_capacity_; }
    size_t word_capacity() const { return words_necessary_for(bit_capacity_); }
    virtual ~bitarray();
};

template <typename W>
bitarray<W>::bitarray(size_t capacity)
: bit_capacity_(words_necessary_for(capacity)*word_size), words_(new W[bit_capacity_/word_size]) { reset(); }

template <typename W>
bitarray<W>::bitarray(const bitarray<W>& other)
: bit_capacity_(other.bit_capacity_), words_(new W[bit_capacity_]) { 
  for(size_t w=0; w != word_capacity(); ++w) words_[w] = other.words_[w];
}

template <typename W>
bitarray<W>::bitarray(bitarray<W>&& other)
: bit_capacity_(other.bit_capacity_), words_(other.words_) { other.words_ = nullptr; }

template <typename W>
bitarray<W>::~bitarray() { delete words_; }

template <typename W>
bitarray<W>& bitarray<W>::operator= (const bitarray<W>& other) {
  bit_capacity_ = other.bit_capacity_;
  delete words_;
  words_ = new W[bit_capacity_];
  for(size_t w=0; w != word_capacity(); ++w) words_[w] = other.words_[w];
  return *this;
}

template <typename W>
bitarray<W>& bitarray<W>::operator= (bitarray<W>&& other) {
  bit_capacity_ = other.bit_capacity_;
  words_ = other.words_;
  other.words_ = nullptr;
  return *this;
}

template <typename W>
void bitarray<W>::resize(size_t new_capacity) {
  // Calculate number of words necessary to hold new capacity of bits
  size_t word_capacity = words_necessary_for(new_capacity);
  new_capacity = word_capacity * word_size;
  // Create new array to hold data
  W* new_words = new W[word_capacity];
  // Copy data into new array; fill with zero remaining spaces; crop data if shrunk
  for(size_t w = 0; w != word_capacity; ++w) {
    if(w < bit_capacity_) new_words[w] = words_[w];
    else new_words[w] = 0;
  }
  // Deallocate memory for former array
  delete words_;
  // Set new attributes
  bit_capacity_ = new_capacity;
  words_ = new_words;
}

template <typename W>
void bitarray<W>::set() { for(size_t w = 0; w != word_capacity(); ++w) words_[w] = ~((W)0); }

template <typename W>
void bitarray<W>::reset() { for(size_t w = 0; w != word_capacity(); ++w) words_[w] = (W)0; }

template <typename W>
void bitarray<W>::set(size_t index) {
  // Verify size and expand if necessary
  if(index > bit_capacity_) resize(index * 2);
  // Set bit
  words_[index/word_size] |= bitmask_at(index%word_size);
}

template <typename W>
void bitarray<W>::reset(size_t index) {
  // Verify size and expand if necessary
  if(index > bit_capacity_) resize(index * 2);
  // Reset bit
  words_[index/word_size] &= ~bitmask_at(index%word_size);
}

template <typename W>
bool bitarray<W>::get(size_t index) const {
  if(index < 0 || index > bit_capacity_) throw std::out_of_range("Invalid bit index.");
  return words_[index/word_size] & bitmask_at(index%word_size);
}

#endif