#include <fstream>
#include "huffman_tree.h"

int main(int argc, char** argv) {
  // Check user input
  if(argc != 3) {
    std::cout << "There must be two arguments. Example:" << std::endl
              << "encoder source.txt destination.bin" << std::endl;
    return EXIT_FAILURE;
  }

  // Get filenames from user input
  std::string source_filename(argv[1]);
  std::string destination_filename(argv[2]);

  // Open files
  std::ifstream source_file(source_filename, std::ios::in | std::ios::binary);
  std::ofstream destination_file(destination_filename);

  // Verify that files have been opened
  if(!source_file.is_open()) throw std::invalid_argument("Could not open file " + source_filename + ".");
  if(!destination_file.is_open()) throw std::invalid_argument("Could not open file " + destination_filename + ".");

  // Get huffman tree and bitstream from file
  huffman_tree<char> huff_tree;
  bitstream encoded_data;
  source_file >> huff_tree >> encoded_data;

  // Decode data
  auto decoded_data = huff_tree.decode(encoded_data);

  // Store decoded data
  destination_file << decoded_data;

  // Free resources
  source_file.close();
  destination_file.close();

  return EXIT_SUCCESS;
}