// ----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License
// ----------------------------------------------------------------------
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../Strings.h"

using namespace Microsoft::Featurizer;

void ParseTest(std::string const & input, std::vector<std::string> const & label) {
    std::vector<std::string> output;
    Parse<std::string::const_iterator, char>(
        input,
        ' ',
        [&output] (std::string::const_iterator & iter_start, std::string::const_iterator & iter_end) {
            output.emplace_back(std::string(iter_start, iter_end));
        }
    );
    CHECK(output == label);
}

void ParseRegexTest(std::string const & input, std::vector<std::string> const & label, std::string const & token = "[^\\s]+") {
    std::vector<std::string> output;
    std::regex rgx(token);
    ParseRegex<std::string::const_iterator, std::regex>(
        input,
        rgx,
        [&output] (std::string::const_iterator iter_start, std::string::const_iterator iter_end) {
            output.emplace_back(std::string(iter_start, iter_end));
        }
    );
    CHECK(output == label);
}

TEST_CASE("ToLower") {
    std::string input("THIS IS THE FIRST DOCUMENT.");
    std::string label("this is the first document.");
    std::string output(ToLower(input.begin(), input.end()));
    CHECK(output == label);
}

TEST_CASE("ToUpper") {
    std::string input("this is the first document.");
    std::string label("THIS IS THE FIRST DOCUMENT.");
    std::string output(ToUpper(input.begin(), input.end()));
    CHECK(output == label);
}

TEST_CASE("TrimLeft") {
    std::string input("    this is the first document.");
    std::string label("this is the first document.");
    std::string output(TrimLeft(input.begin(), input.end(), ' '));
    CHECK(output == label);
}

TEST_CASE("TrimRight") {
    std::string input("this is the first document.        ");
    std::string label("this is the first document.");
    std::string output(TrimRight(input.begin(), input.end(), ' '));
    CHECK(output == label);
}

TEST_CASE("Trim") {
    std::string input("     this is the first document.     ");
    std::string label("this is the first document.");
    std::string output(Trim(input.begin(), input.end(), ' '));
    CHECK(output == label);
}

TEST_CASE("Parse") {
    ParseTest("this is a document", {"this", "is", "a", "document"});
    ParseTest("this   is a   document ", {"this", "is", "a", "document"});
    ParseTest(" this is   a document", {"this", "is", "a", "document"});
    ParseTest(" this is a document ", {"this", "is", "a", "document"});
    ParseTest(" this   is a   document  ", {"this", "is", "a", "document"});
    ParseTest("this", {"this"});
    ParseTest(" this", {"this"});
    ParseTest("this ", {"this"});
    ParseTest(" this ", {"this"});
    ParseTest("  this  ", {"this"});
    ParseTest("", {});
    ParseTest(" ", {});
    ParseTest("  ", {});
}

TEST_CASE("ParseRegex") {
    ParseRegexTest("this is a document", {"this", "is", "a", "document"});
    ParseRegexTest("this   is a   document ", {"this", "is", "a", "document"});
    ParseRegexTest(" this is   a document", {"this", "is", "a", "document"});
    ParseRegexTest(" this is a document ", {"this", "is", "a", "document"});
    ParseRegexTest(" this   is a   document  ", {"this", "is", "a", "document"});
}


// TEST_CASE("TextPreprocessorTransformer_ngramsword") {
//     std::vector<std::string> stopwords({});
//     NS::Featurizers::Components::TextPreprocessorTransformer tp(true, "\\s+", stopwords, "ascii", 1, 2, "word");
//     std::string input(" Bi-grams    are cool! ");
//     std::vector<std::string> label({"bi", "grams", "are", "cool", "bi grams", "grams are", "are cool"});
//     std::vector<std::string> out = tp.execute(input);
//     CHECK(out == label);
// }
// TEST_CASE("TextPreprocessorTransformer_ngramschar") {
//     std::vector<std::string> stopwords({});
//     NS::Featurizers::Components::TextPreprocessorTransformer tp(true, "\\s+", stopwords, "ascii", 5, 5, "char");
//     std::string input("jumpy fox");
//     std::vector<std::string> label({"jumpy", "umpy ", "mpy f", "py fo", "y fox"});
//     std::vector<std::string> out = tp.execute(input);
//     CHECK(out == label);
// }
// TEST_CASE("TextPreprocessorTransformer_ngramscharwb") {
//     std::vector<std::string> stopwords({});
//     NS::Featurizers::Components::TextPreprocessorTransformer tp(true, "\\s+", stopwords, "ascii", 5, 5, "char_wb");
//     std::string input("jumpy fox");
//     std::vector<std::string> label({" jump", "jumpy", "umpy ", " fox "});
//     std::vector<std::string> out = tp.execute(input);
//     CHECK(out == label);
// }
// TEST_CASE("TextPreprocessorTransformer_lowercase_unicode") {
//     std::vector<std::string> stopwords({});
//     NS::Featurizers::Components::TextPreprocessorTransformer tp(true, "\\s+", stopwords, "unicode", 1, 1, "word");
//     NS::Featurizers::Components::Utf8Converter converter(conv_error, wconv_error);
// #if (defined __clang__)
// #   pragma clang diagnostic push
// #   pragma clang diagnostic ignored "-Wexit-time-destructors"
// #endif
//     std::wstring winput(L"HellO");
//     std::string input = converter.to_bytes(winput);
//     std::wstring prewlabel(L"hello");
//     std::string prelabel = converter.to_bytes(prewlabel);
//     std::vector<std::string> label({prelabel});
// #if (defined __clang__)
// #   pragma clang diagnostic pop
// #endif
//     std::vector<std::string> out = tp.execute(input);
//     CHECK(out == label);
// }
