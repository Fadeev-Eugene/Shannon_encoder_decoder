#pragma once
#include <string>
#include <map>

class ShannonEncoder {
public:
    void encode_file(const std::string& input_file, 
                    const std::string& output_file,
                    const std::string& dict_file = "dict.shannon");
    
private:
    std::map<char, std::string> code_dict;
    
    void build_code_dictionary(const std::string& data);
    void save_dictionary(const std::string& filename);
};
