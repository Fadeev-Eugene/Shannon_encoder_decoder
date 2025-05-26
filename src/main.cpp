#include <iostream>
#include "shannon_encoder.h"
#include "shannon_decoder.h"

using namespace std;

void print_help() {
    cout << "Shannon-Fano codec\n"
         << "Usage:\n"
         << "  shannon encode <input> <output> [dict]\n"
         << "  shannon decode <input> <output> <dict>\n"
         << "  shannon --help\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    string command(argv[1]);
    
    try {
        if (command == "encode") {
            if (argc < 4) throw invalid_argument("Not enough arguments");
            
            ShannonEncoder encoder;
            encoder.encode_file(
                argv[2], 
                argv[3], 
                argc >= 5 ? argv[4] : "dict.shannon"
            );
            
        } else if (command == "decode") {
            if (argc < 5) throw invalid_argument("Not enough arguments");
            
            ShannonDecoder decoder;
            decoder.load_dictionary(argv[4]);
            decoder.decode_file(argv[2], argv[3]);
            
        } else if (command == "--help") {
            print_help();
            
        } else {
            throw invalid_argument("Unknown command");
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n\n";
        print_help();
        return 1;
    }
    
    return 0;
}
