
#include <unordered_map>
#include <string>
#include <fstream>
#include <stdexcept>

namespace encoder {

  /* Counts frequency of characters inside the provided file.
   * If max_chars is negative, then it will count every char in file.
   * Else, it will count up to the provided number only.
   * By default, max_chars = -1.
   */
  template <typename char_t>
  std::unordered_map<char_t, int> get_char_frequencies(std::string filename, size_t max_chars = -1);

  /* Counts frequency of characters inside the provided file.
   * If max_chars is negative, then it will count every char in file.
   * Else, it will count up to the provided number only.
   * By default, max_chars = -1.
   */
  template <typename char_t>
  std::unordered_map<char_t, int> get_char_frequencies(std::ifstream& file, size_t max_chars = -1);

}

template <typename char_t>
std::unordered_map<char_t, int> encoder::get_char_frequencies(std::string filename, size_t max_chars) {
  std::ifstream file(filename);
  // Verify that file has been opened
  if(!file.is_open()) throw std::invalid_argument("Could not open file " + filename + ".");
  // Get frequencies from file
  auto freq = get_char_frequencies<char_t>(file, max_chars);
  // Free resources and return map
  file.close();
  return freq;
}


template <typename char_t>
std::unordered_map<char_t, int> encoder::get_char_frequencies(std::ifstream& file, size_t max_chars) {
  // Create map to hold char_t frequencies
  std::unordered_map<char_t, int> freq;
  // Initialize values to zero
  for(int ch = 0; ch != (1U << sizeof(char_t)*CHAR_BIT); ++ch) freq[ch] = 0;
  // Traverse characters inside file
  char_t ch = 0;
  for(int i = 0; file.get(ch) && i != max_chars; ++i) freq[ch] += 1;
  // Delete unused characters
  for(auto it = freq.begin(); it != freq.end(); ) {
    if(it->second == 0 && it->first != 0) it = freq.erase(it);
    else ++it;
  }
  // Reset state of file and return map
  file.clear();
  file.seekg(0, std::ios::beg);
  return freq;
}