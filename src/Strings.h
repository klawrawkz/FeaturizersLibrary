// ----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License
// ----------------------------------------------------------------------
#pragma once

namespace Microsoft {
namespace Featurizer {





// //casing. todo: locale
// template<typename IteratorT>
// inline IteratorT to_lower_copy(const IteratorT& inputbegin, const IteratorT& inputend);
// template<typename IteratorT>
// inline IteratorT to_lower(IteratorT& inputbegin, IteratorT& inputend);
// template<typename IteratorT>
// inline IteratorT to_upper(const IteratorT& inputbegin, const IteratorT& inputend);
// template<typename IteratorT>
// inline IteratorT to_upper(IteratorT& inputbegin, IteratorT& inputend);

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
