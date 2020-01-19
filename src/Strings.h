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
    while (begin != end) {
        if (*end == isPredicate)
            --end;
        else
            break;
    }
    return std::string(begin, end);
}

template <typename IteratorT, typename PredicateT>
inline std::string Trim(IteratorT begin, IteratorT end, PredicateT isPredicate) {
    while (begin != end) {
        if (*begin == isPredicate)
            ++begin;
        else if (*end == isPredicate)
            --end;
        else
            break;
    }
    return std::string(begin, end);
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
