#include "shannon_decoder.h"
#include <fstream>
#include <sstream>
#include <bitset>
#include <climits>

using namespace std;

void ShannonDecoder::load_dictionary(const string& dict_file) {
    ifstream in(dict_file);
    decode_tree = make_shared<DecodeNode>();
    string line;
    
    while (getline(in, line)) {
        size_t sep = line.find(':');
        if (sep == string::npos) continue;
        
        char c = static_cast<char>(stoi(line.substr(0, sep)));
        string code = line.substr(sep + 1);
        
        shared_ptr<DecodeNode> current = decode_tree;
        for (char bit : code) {
            if (bit == '0') {
                if (!current->zero) 
                    current->zero = make_shared<DecodeNode>();
                current = current->zero;
            } else {
                if (!current->one) 
                    current->one = make_shared<DecodeNode>();
                current = current->one;
            }
        }
        current->symbol = c;
    }
}

void ShannonDecoder::decode_file(const string& input_file, 
                               const string& output_file) {
    ifstream in(input_file, ios::binary);
    int padding = in.get();
    
    string bitstring;
    char byte;
    while (in.get(byte)) {
        bitset<8> bits(byte);
        bitstring += bits.to_string();
    }
    if (padding > 0) bitstring.resize(bitstring.size() - padding);
    
    ofstream out(output_file);
    shared_ptr<DecodeNode> current = decode_tree;
    
    for (char bit : bitstring) {
        current = (bit == '0') ? current->zero : current->one;
        
        if (!current) throw runtime_error("Invalid code sequence");
        
        if (current->symbol != '\0') {
            out.put(current->symbol);
            current = decode_tree;
        }
    }
}
