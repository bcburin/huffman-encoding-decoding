#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include "huffman_tree.h"
#include "encoder.h"


int main(int argc, char** argv) {
  // Check user input
  if(argc != 3) {
    std::cout << std::endl << "There must be two arguments. Example:" << std::endl
              << std::endl << "encoder source.txt destination.bin" << std::endl;
    return EXIT_FAILURE;
  }

  // Get filenames from user input
  std::string source_filename(argv[1]);
  std::string destination_filename(argv[2]);

  
  // Open files
  std::ifstream source_file(source_filename);
  std::ofstream destination_file(destination_filename, std::ios::out | std::ios::binary);

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

  // Get character frequencies from file
  auto frequencies = encoder::get_char_frequencies<char>(source_file);

  // Create huffman tree from character frequencies
  auto huff_tree = huffman_tree<char>::from_frequencies(frequencies);

  // Read whole file
  std::stringstream buffer;
  buffer << source_file.rdbuf();

  // Encode file content
  auto encoded_data = huff_tree->encode(buffer.str());

  // Write encoded data to binary file
  destination_file << *huff_tree << encoded_data;

  // Free resources
  source_file.close();
  destination_file.close();
  delete huff_tree;

  return EXIT_SUCCESS;
}