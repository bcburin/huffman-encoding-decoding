#include <cstdio>
#include <fstream>
#include "huffman_tree.h"

int main(int argc, char** argv) {
  // Check user input
  if(argc != 3) {
    std::cerr << std::endl << "There must be two arguments. Example:" << std::endl
              << std::endl << "encoder source.bin destination.txt" << std::endl;
    return EXIT_FAILURE;
  }

  // Get filenames from user input
  std::string source_filename(argv[1]);
  std::string destination_filename(argv[2]);

  // Open files
  std::ifstream source_file(source_filename, std::ios::in | std::ios::binary);
  std::ofstream destination_file(destination_filename);

  // Verify that files have been opened
  if(!source_file.is_open()) {
    std::cerr << std::endl << "Could not find file " << source_filename << "." << std::endl;
    destination_file.close();
    remove(destination_filename.c_str());
    return EXIT_FAILURE;
  }
  if(!destination_file.is_open()) {
    std::cerr << std::endl << "Could not open nor create file " << destination_filename << "." << std::endl;
    return EXIT_FAILURE;
  }

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