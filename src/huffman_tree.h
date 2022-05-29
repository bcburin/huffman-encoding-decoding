#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "binary_tree.h"
#include "bitdequeue.h"
#include <unordered_map>
#include <queue>


using bitstream = bitdequeue<unsigned long>;

template <typename char_t>
struct identified_char {
  char_t ch;
  int id;
  identified_char(char_t ch = 0);
  bool operator== (const identified_char<char_t>& other) const { return id == other.id; }
  private:
    static int id_;
};


template <typename char_t>
class huffman_tree: public binary_tree<identified_char<char_t>> {
  private:
    int frequency_;
    std::unordered_map<char_t, bitstream> codes_;
    void generate_codes_util(std::unordered_map<char_t, bitstream>& codes, bitstream& code);
    huffman_tree<char_t>* left() { return static_cast<huffman_tree<char_t>*>(binary_tree<identified_char<char_t>>::left_); }
    huffman_tree<char_t>* right() { return static_cast<huffman_tree<char_t>*>(binary_tree<identified_char<char_t>>::right_); }
    static int id_;
  public:
    huffman_tree(char_t ch, int frequency = 0);
    int frequency() const { return frequency_; }
    char_t ch() const { return binary_tree<identified_char<char_t>>::data_.ch; }
    bitstream encode(std::basic_string<char_t> str);
    std::basic_string<char_t> decode(bitstream& coded);
    struct greater { bool operator() (const huffman_tree<char_t>*, const  huffman_tree<char_t>*); };
    static huffman_tree<char_t>* from_frequencies(std::unordered_map<char_t, int> frequencies);
};

template <typename char_t>
int identified_char<char_t>::id_ = 0;


template <typename char_t>
identified_char<char_t>::identified_char(char_t ch): ch(ch), id(id_++) {}


template <typename char_t>
bool huffman_tree<char_t>::greater::operator() (const huffman_tree<char_t>* ch1, const  huffman_tree<char_t>* ch2) {
  return ch1->frequency() > ch2->frequency();
}


template <typename char_t>
huffman_tree<char_t>::huffman_tree(char_t ch, int frequency)
: binary_tree<identified_char<char_t>>(identified_char<char_t>(ch)), frequency_(frequency) {}


template <typename char_t>
huffman_tree<char_t>* huffman_tree<char_t>::from_frequencies(std::unordered_map<char_t, int> frequencies) {
  // Check for empty input
  if(frequencies.empty()) return nullptr;
  // Create priority queue (min heap) to hold nodes of huffman tree
  std::priority_queue<huffman_tree<char_t>*, std::vector<huffman_tree<char_t>*>, greater> node_queue;
  // Insert characters (leaf nodes) in the min heap
  for(const auto& pair : frequencies) node_queue.push(new huffman_tree<char_t>(pair.first, pair.second));
  // Build tree
  while (node_queue.size() != 1) {
    // Retrieve two nodes on the top of the queue
    auto left = node_queue.top(); node_queue.pop();
    auto right = node_queue.top(); node_queue.pop();
    // Create internal node with frequency equal to the sum of left and right subtrees
    auto internal_node = new huffman_tree<char_t>(0, left->frequency() + right->frequency());
    internal_node->left_ = left;
    internal_node->right_ = right;
    // Put internal node in the queue
    node_queue.push(internal_node);
  }
  // Retrieve last node
  auto tree = node_queue.top();
  // Generate codes
  bitstream code;
  tree->generate_codes_util(tree->codes_, code);

  return tree;
}


template <typename char_t>
void huffman_tree<char_t>::generate_codes_util(std::unordered_map<char_t, bitstream>& codes, bitstream& code) {
  // Base case (leaf node): insert code into given map
  if(!left() && !right() && !code.empty()) codes[ch()] = code;
  // Apply recursively to left and right subtrees
  if(left()) { left()->generate_codes_util(codes, code.push_back(0)); code.pop_back(); }
  if(right()) { right()->generate_codes_util(codes, code.push_back(1)); code.pop_back(); }
}


template <typename char_t>
bitstream huffman_tree<char_t>::encode(std::basic_string<char_t> str) {
  bitstream coded(str.size() * CHAR_BIT);

  for(const auto& ch : str) { coded.push_back(codes_[ch]); }
  coded.push_back(codes_[0]);

  return coded;
}


template <typename char_t>
std::basic_string<char_t> huffman_tree<char_t>::decode(bitstream& coded) {
  std::basic_string<char_t> decoded;
  while(true) {
    huffman_tree<char_t>* node = this;
    while(node->left() && node->right()) node = coded.pop_front() ? node->right() : node->left();
    decoded += std::basic_string<char_t>(1, node->ch());
    if(node->ch() == (char_t)0) break;
  }
  return decoded;
}



#endif