#include <gtest/gtest.h>
#include "shannon_encoder.h"
#include "shannon_decoder.h"
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

class ShannonCodingTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::remove("test.txt");
        fs::remove("test.bin");
        fs::remove("test_decoded.txt");
        fs::remove("dict.shannon");
    }

    void TearDown() override {
        fs::remove("test.txt");
        fs::remove("test.bin");
        fs::remove("test_decoded.txt");
        fs::remove("dict.shannon");
    }
};

TEST_F(ShannonCodingTest, BasicEncodingDecoding) {
    ofstream("test.txt") << "AAABBC";
    
    ShannonEncoder encoder;
    encoder.encode_file("test.txt", "test.bin");
    
    ShannonDecoder decoder;
    decoder.load_dictionary("dict.shannon");
    decoder.decode_file("test.bin", "test_decoded.txt");
    
    ifstream in("test_decoded.txt");
    string result(istreambuf_iterator<char>(in), {});
    ASSERT_EQ(result, "AAABBC");
}

TEST_F(ShannonCodingTest, SingleSymbolEncoding) {
    {
        ofstream out("test.txt");
        out << "XXXXX";
    }

    ShannonEncoder encoder;
    encoder.encode_file("test.txt", "test.bin");
    
    ShannonDecoder decoder;
    decoder.load_dictionary("dict.shannon");
    decoder.decode_file("test.bin", "test_decoded.txt");

    ifstream in("test_decoded.txt");
    string result(istreambuf_iterator<char>(in), {});
    
    ASSERT_EQ(result.size(), 5);
    ASSERT_EQ(result, "XXXXX");
}

TEST_F(ShannonCodingTest, EmptyFileThrows) {
    ShannonEncoder encoder;
    ASSERT_THROW(
        encoder.encode_file("nonexistent.txt", "out.bin"), 
        std::runtime_error
    );
}
