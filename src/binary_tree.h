#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <ostream>
#include <sstream>
#include <utility>
#include <vector>

template <typename T>
class binary_tree {
  protected:
    T data_;
    binary_tree<T>* left_;
    binary_tree<T>* right_;
  public:
    enum class traversal {inorder, preorder, postorder};
    binary_tree(T data);
    binary_tree(const binary_tree<T>& other);
    binary_tree(binary_tree<T>&& other);
    ~binary_tree();
    binary_tree<T>& operator= (const binary_tree<T>& other);
    binary_tree<T>& operator= (binary_tree<T>&& other);
    std::string str(traversal type = traversal::postorder) const;
    static binary_tree<T>* from_inorder_postorder(const std::vector<T>& in, const std::vector<T>& post);
    static binary_tree<T>* from_inorder_postorder(
      const std::vector<T>& in, const std::vector<T>& post, 
      int in_start, int in_end, int post_start, int post_end
    );
    std::vector<T> vector(traversal type = traversal::postorder) const;
    std::vector<T> vector(traversal type, std::vector<T>& acc) const;
    template <typename TT>
    friend std::ostream& operator<< (std::ostream& os, const binary_tree<TT>& tree);
    template <typename TT>
    friend std::istream& operator>> (std::istream& is, binary_tree<TT>& tree);
};

template <typename T>
std::ostream& operator<< (std::ostream& os, const binary_tree<T>& tree);

template <typename T>
std::istream& operator>> (std::istream& is, binary_tree<T>& tree);


template <typename T>
binary_tree<T>::binary_tree(T data): data_(data), left_(nullptr), right_(nullptr) {}

template <typename T>
binary_tree<T>::binary_tree(const binary_tree<T>& other)
: data_(other.data_), left_(nullptr), right_(nullptr) {
  if (other.left_) left_ = new binary_tree<T>(*other.left_);
  if(other.right_) right_ = new binary_tree<T>(*other.right_);
}

template <typename T>
binary_tree<T>::binary_tree(binary_tree<T>&& other)
: data_(std::move(data_)), left_(other.left_), right_(other.right_) {
  other;left_ = other.right_ = nullptr;
}

template <typename T>
binary_tree<T>::~binary_tree() {
  if(left_) delete left_;
  if(right_) delete right_;
}

template <typename T>
binary_tree<T>& binary_tree<T>::operator= (const binary_tree<T>& other) {
  data_ = other.data_;
  if(left_) delete left_;
  if(right_) delete right_;
  if(other.left_) left_ = new binary_tree<T>(*other.left_);
  if(other.right_) right_ = new binary_tree<T>(*other.right_);
  return *this;
}

template <typename T>
binary_tree<T>& binary_tree<T>::operator= (binary_tree<T>&& other) {
  data_ = other.data_;
  if(left_) delete left_;
  if(right_) delete right_;
  left_ = other.left_;
  right_ = other.right_;
  other.left_ = nullptr;
  other.right_ = nullptr;
  return *this;
}

template <typename T>
std::string binary_tree<T>::str(traversal type) const {
  std::stringstream ss;
  switch(type) {
    case traversal::preorder:
      ss << data_;
      if(left_) ss << left_->str(type);
      if(right_) ss << right_->str(type);
      break;
    case traversal::inorder:
      if(left_) ss << left_->str(type);
      ss << data_;
      if(right_) ss << right_->str(type);
      break;
    case traversal::postorder:
      if(left_) ss << left_->str(type);
      if(right_) ss << right_->str(type);
      ss << data_;
      break;
  }
  return ss.str();
}

template <typename T>
binary_tree<T>* binary_tree<T>::from_inorder_postorder(
      const std::vector<T>& in, const std::vector<T>& post, 
      int in_start, int in_end, int post_start, int post_end
    ) {
  // Base case
  if(in_start == in_end) return nullptr;
  // Find current node (last element in range of post vector)
  binary_tree<T>* curr = new binary_tree<T>(post[post_end-1]);
  // Find index of current node in range of in vector
  int index = -1;
  for(int i = in_start; i != in_end; ++i) {
    if(in[i] == curr->data_) {
      index = i;
      break;
    }
  }
  // Generate left and right subtrees recursively
  curr->left_ = binary_tree<T>::from_inorder_postorder(in, post, in_start, index, post_start, post_start-in_start+index);
  curr->right_ = binary_tree<T>::from_inorder_postorder(in, post, index+1, in_end, post_start-in_start+index, post_end-1);
  // Return generated node
  return curr;
}

template <typename T>
binary_tree<T>* binary_tree<T>::from_inorder_postorder(const std::vector<T>& in, const std::vector<T>& post) {
  if(in.size() != post.size()) return nullptr;
  return binary_tree<T>::from_inorder_postorder(in, post, 0, in.size(), 0, post.size());
}

template <typename T>
std::vector<T> binary_tree<T>::vector(traversal type) const {
  std::vector<T> acc;
  return vector(type, acc);
}

template <typename T>
std::vector<T> binary_tree<T>::vector(traversal type, std::vector<T>& acc) const {
  switch(type) {
    case traversal::preorder:
      acc.push_back(data_);
      if(left_) left_->vector(type, acc);
      if(right_) right_->vector(type, acc);
      break;
    case traversal::inorder:
      if(left_) left_->vector(type, acc);
      acc.push_back(data_);
      if(right_) right_->vector(type, acc);
      break;
    case traversal::postorder:
      if(left_) left_->vector(type, acc);
      if(right_) right_->vector(type, acc);
      acc.push_back(data_);
      break;
  }
  return acc;
}

template <typename T>
std::ostream& operator<< (std::ostream& os, const binary_tree<T>& tree) {
  // Get inorder and postorder traversals of tree
  std::vector<T> post = tree.vector(binary_tree<T>::traversal::postorder);
  std::vector<T> in = tree.vector(binary_tree<T>::traversal::inorder);
  // Store size of tree
  size_t size = in.size();
  os.write((char*) &size, sizeof(size_t));
  // Store traversals
  for(int i = 0; i != size; ++i) os.write((char*) &in[i], sizeof(T));
  for(int i = 0; i != size; ++i) os.write((char*) &post[i], sizeof(T));
  return os;
}

template <typename T>
std::istream& operator>> (std::istream& is, binary_tree<T>& tree) {
  // Build inorder and postorder traversals of tree
  std::vector<T> post;
  std::vector<T> in;
  // Get size
  size_t size;
  is.read((char*) &size, sizeof(size_t));
  // Get traversals
  T temp;
  for(int i = 0; i != size; ++i) { is.read((char*) &temp, sizeof(T)); in.push_back(temp); }
  for(int i = 0; i != size; ++i) { is.read((char*) &temp, sizeof(T)); post.push_back(temp); }
  // Build tree from traversals
  tree = *binary_tree<T>::from_inorder_postorder(in, post);
  return is;
}

#endif