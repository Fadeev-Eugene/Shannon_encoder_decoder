#include "shannon_encoder.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <climits>
#include <numeric>

using namespace std;

void ShannonEncoder::build_code_dictionary(const string& data) {
    map<char, int> freq;
    for (char c : data) freq[c]++;
    
    vector<pair<char, int>> sorted(freq.begin(), freq.end());
    sort(sorted.begin(), sorted.end(), 
        [](auto& a, auto& b) { return a.second > b.second; });

    code_dict.clear();

    function<void(vector<pair<char, int>>&, int, int, string)> build_codes;
    build_codes = [&](vector<pair<char, int>>& symbols, int start, int end, string code) {
        if (start >= end) {
            if (code.empty()) code = "0"; 
            code_dict[symbols[start].first] = code;
            return;
        }

        int total = accumulate(symbols.begin() + start, symbols.begin() + end + 1, 0,
            [](int sum, auto& p) { return sum + p.second; });
        
        int sum = 0, split = start, min_diff = INT_MAX;
        for (int i = start; i < end; i++) {
            sum += symbols[i].second;
            int diff = abs(2 * sum - total);
            if (diff < min_diff) {
                min_diff = diff;
                split = i;
            }
        }

        build_codes(symbols, start, split, code + "0");
        build_codes(symbols, split + 1, end, code + "1");
    };

    if (sorted.size() == 1) {
        code_dict[sorted[0].first] = "0";
    } else {
        build_codes(sorted, 0, sorted.size()-1, "");
    }
}

void ShannonEncoder::save_dictionary(const string& filename) {
    ofstream out(filename);
    for (const auto& [c, code] : code_dict) {
        out << static_cast<int>(c) << ':' << code << '\n';
    }
}

void ShannonEncoder::encode_file(const string& input_file, 
                               const string& output_file,
                               const string& dict_file) {

    ifstream in(input_file);
    if (!in) throw runtime_error("Can't open input file");    
    
    string data(istreambuf_iterator<char>(in), {});
    
    build_code_dictionary(data);
    save_dictionary(dict_file);
    
    string bitstring;
    for (char c : data) bitstring += code_dict[c];
    
    int padding = (8 - (bitstring.length() % 8)) % 8;
    bitstring += string(padding, '0');
    
    ofstream out(output_file, ios::binary);
    out.put(static_cast<char>(padding));
    
    for (size_t i = 0; i < bitstring.size(); i += 8) {
        bitset<8> bits(bitstring.substr(i, 8));
        out.put(static_cast<char>(bits.to_ulong()));
    }
    out.close(); 
}
