#pragma once
#include <string>
#include <memory>

struct DecodeNode {
    char symbol = '\0';
    std::shared_ptr<DecodeNode> zero;
    std::shared_ptr<DecodeNode> one;
};

class ShannonDecoder {
public:
    void load_dictionary(const std::string& dict_file);
    void decode_file(const std::string& input_file, 
                   const std::string& output_file);
    
private:
    std::shared_ptr<DecodeNode> decode_tree;
};
