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
    std::transform(result.begin(), result.end(), result.begin(), [](char c) { return std::tolower(c); });
    return result;
}

template <typename IteratorT>
inline std::string ToUpper(IteratorT begin, IteratorT end) {
    std::string result(begin, end);
    std::transform(result.begin(), result.end(), result.begin(), [](char c) { return std::toupper(c); });
    return result;
}


// //trim
// template<typename IteratorT, typename PredicateT>
// inline IteratorT trim_left(const IteratorT& inputbegin, const IteratorT& inputend, PredicateT isspace);
// template<typename IteratorT, typename PredicateT>
// inline IteratorT trim_left(IteratorT& inputbegin, IteratorT& inputend, PredicateT isspace);
// template<typename IteratorT, typename PredicateT>
// inline IteratorT trim_right(const IteratorT& inputbegin, const IteratorT& inputend, PredicateT isspace);
// template<typename IteratorT, typename PredicateT>
// inline IteratorT trim_right(IteratorT& inputbegin, IteratorT& inputend, PredicateT isspace);
// template<typename IteratorT, typename PredicateT>
// inline IteratorT trim(const IteratorT& inputbegin, const IteratorT& inputend, PredicateT isspace);
// template<typename IteratorT, typename PredicateT>
// inline IteratorT trim(IteratorT& inputbegin, IteratorT& inputend, PredicateT isspace);
// //Remove all leading and trailing spaces from the input and compress all other spaces to a single character.
// template<typename IteratorT, typename PredicateT>
// inline IteratorT trim_all(const IteratorT& inputbegin, const IteratorT& inputend, PredicateT isspace);
// template<typename IteratorT, typename PredicateT>
// inline IteratorT trim_all(IteratorT& inputbegin, IteratorT& inputend, PredicateT isspace);

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

// //parse
// template<typename IteratorT, typename PredicateT>
// inline std::vector<std::tuple<IteratorT, IteratorT>> parse(const IteratorT& inputbegin, const IteratorT& inputend, PredicateT ispunct);
// template<typename IteratorT, typename RegexT>
// inline std::vector<std::tuple<IteratorT, IteratorT>> parse(const IteratorT& inputbegin, const IteratorT& inputend, const RegexT& re, "flag");


// //ngram
// //question: tuple<IteratorT*, size_t> or tuple<IteratorT*, IteratorT*>
// template<typename IteratorT, typename RangeT>
// inline std::vector<std::tuple<IteratorT, IteratorT>> ngram_string(const IteratorT& inputbegin, const IteratorT& inputend, RangeT ngram_range);
// template<typename IteratorT, typename RangeT>
// inline std::vector<std::tuple<IteratorT, IteratorT>> ngram_string(IteratorT& inputbegin, IteratorT& inputend, RangeT ngram_range);
// template<typename IteratorT, typename RangeT>
// inline std::vector<std::tuple<IteratorT, IteratorT>> ngram_char(const IteratorT& inputbegin, const IteratorT& inputend, RangeT ngram_range);
// template<typename IteratorT, typename RangeT>
// inline std::vector<std::tuple<IteratorT, IteratorT>> ngram_char(IteratorT& inputbegin, IteratorT& inputend, RangeT ngram_range);
// template<typename IteratorT, typename RangeT>
// inline std::vector<std::tuple<IteratorT, IteratorT>> ngram_charwb(const IteratorT& inputbegin, const IteratorT& inputend, RangeT ngram_range);
// template<typename IteratorT, typename RangeT>
// inline std::vector<std::tuple<IteratorT, IteratorT>> ngram_charwb(IteratorT& inputbegin, IteratorT& inputend, RangeT ngram_range);


} // namespace Featurizer
} // namespace Microsoft
