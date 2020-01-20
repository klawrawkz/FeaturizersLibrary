// ----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License
// ----------------------------------------------------------------------
#pragma once

#include <algorithm>
#include <iterator>

namespace Microsoft {
namespace Featurizer {

template <typename IteratorT>
inline std::string ToLower(IteratorT begin, IteratorT end) {
    std::string result(begin, end);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

template <typename IteratorT>
inline std::string ToUpper(IteratorT begin, IteratorT end) {
    std::string result(begin, end);
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

template <typename IteratorT, typename PredicateT>
inline std::string TrimLeft(IteratorT begin, IteratorT end, PredicateT isPredicate) {
    while (begin != end) {
        if (*begin == isPredicate)
            ++begin;
        else
            break;
    }
    return std::string(begin, end);
}

template <typename IteratorT, typename PredicateT>
inline std::string TrimRight(IteratorT begin, IteratorT end, PredicateT isPredicate) {
    IteratorT offset = begin;
    IteratorT iter = begin;
    while (iter != end) {
        if (*iter != isPredicate)
            offset = iter;
        ++iter;
    }
    return std::string(begin, offset + 1);
}

template <typename IteratorT, typename PredicateT>
inline std::string Trim(IteratorT begin, IteratorT end, PredicateT isPredicate) {
    std::string stringAfterTrimedLeft(TrimLeft(begin, end, isPredicate));
    return TrimRight(stringAfterTrimedLeft.begin(), stringAfterTrimedLeft.end(), isPredicate);
}

template <typename IteratorT, typename IsDelimiterT>
inline void Parse(std::string const &input, IsDelimiterT const &isDelimiter, std::function<void (IteratorT&, IteratorT&)> const &callback) {
    IteratorT left = input.begin();
    IteratorT right = left;
    while (left != input.end() && *left == isDelimiter)
        ++left;

    right = left;
    while (right != input.end()) {
        if (*right == isDelimiter) {
            callback(left, right);
            left = right;
            while (left != input.end() && *left == isDelimiter)
                ++left;
            right = left;
        } else {
            ++right;
        }
    }
    if (left != right)
        callback(left, right);
}

template <typename IteratorT, typename RegexT>
inline void ParseRegex(std::string const &input, RegexT const &regexToken, std::function<void (IteratorT, IteratorT)> const &callback) {
    std::sregex_iterator iter(input.begin(), input.end(), regexToken);
    std::sregex_iterator end;

    while (iter != end) {
        std::string const iterStringConst = iter->str();
        callback(iterStringConst.begin(), iterStringConst.end());
        ++iter;
    }
}

} // namespace Featurizer
} // namespace Microsoft


// inline void Trim(std::string& input) {
//     //replace all punctuations with spaces
//     std::transform(input.begin(), input.end(), input.begin(), [](char c) {
//         if (std::ispunct(c) )
//             return ' ';
//         return c;
//     });
//     //remove duplicate whitespace
//     bool prev_is_space = true;
//     input.erase(std::remove_if(input.begin(), input.end(), [&prev_is_space](unsigned char curr) {
//         bool r = std::isspace(curr) && prev_is_space;
//         prev_is_space = std::isspace(curr);
//         return r;
//     }), input.end());
// }
// #if (defined _MSC_VER)
// #   pragma warning(pop)
// #endif
// inline void RemoveWhiteSpace(std::string& input) {
//     input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
// }

// inline std::vector<std::string> NgramParsingWord(std::string const &input, std::uint8_t ngram_range_min, std::uint8_t ngram_range_max) {
//     if (ngram_range_min < 1 || ngram_range_max < ngram_range_min)
//         throw std::invalid_argument("invalid ngram range");
//     std::vector<std::string> ngramDocument;
//     std::vector<size_t> spaceIdx;
//     spaceIdx.emplace_back(0);
//     size_t idx = 0;
//     for (auto it = input.begin(); it != input.end(); ++it, ++idx) {
//         if (std::isspace(*it))
//             spaceIdx.emplace_back(idx + 1);
//     }

//     //need optimize
//     for (auto ngramVal = ngram_range_min; ngramVal <= ngram_range_max; ++ngramVal) {
//         for (size_t wordIdx = 0; wordIdx < spaceIdx.size() - ngramVal; ++wordIdx) {
//             ngramDocument.emplace_back(input.substr(spaceIdx[wordIdx], spaceIdx[wordIdx + ngramVal] - spaceIdx[wordIdx] - 1));
//         }
//     }
//     return ngramDocument;
// }
// inline std::vector<std::string> NgramParsingChar(std::string const &input, std::uint8_t ngram_range_min, std::uint8_t ngram_range_max) {
//     if (ngram_range_min < 1 || ngram_range_max < ngram_range_min)
//         throw std::invalid_argument("invalid ngram range");
//     std::vector<std::string> ngramDocument;
//     for (auto ngramVal = ngram_range_min; ngramVal <= ngram_range_max; ++ngramVal) {
//         for (size_t segIdx = 0; segIdx < input.size() - ngramVal + 1; ++segIdx) {
//             ngramDocument.emplace_back(input.substr(segIdx, ngramVal));
//         }
//     }
//     if (ngramDocument.size() == 0)
//         std::runtime_error("empty vocabulary");
//     return ngramDocument;
// }
// inline std::vector<std::string> NgramParsingCharwb(std::vector<std::string> const &input, std::uint8_t ngram_range_min, std::uint8_t ngram_range_max) {
//     if (ngram_range_min < 1 || ngram_range_max < ngram_range_min)
//         throw std::invalid_argument("invalid ngram range");
//     std::vector<std::string> ngramDocument;
//     for (std::string word : input) {
//         std::string paddingWord = " " + word + " ";
//         size_t paddingWordSize = paddingWord.size();
//         for (auto ngramVal = ngram_range_min; ngramVal <= ngram_range_max; ++ngramVal) {
//             if (ngramVal >= paddingWordSize) {
//                 ngramDocument.emplace_back(paddingWord);
//                 break;
//             }
//             for (size_t segIdx = 0; segIdx < paddingWordSize - ngramVal + 1; ++segIdx) {
//                 ngramDocument.emplace_back(paddingWord.substr(segIdx, ngramVal));
//             }

//         }
//     }
//     if (ngramDocument.size() == 0)
//         std::runtime_error("empty vocabulary");
//     return ngramDocument;
// }
