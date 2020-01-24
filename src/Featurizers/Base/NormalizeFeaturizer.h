// ----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License
// ----------------------------------------------------------------------
#pragma once

#include "../Components/VectorNormsEstimator.h"
#include "../Components/PipelineExecutionEstimatorImpl.h"
#include "../Components/NormUpdaters.h"
#include "../../Archive.h"
#include "../Structs.h"
namespace Microsoft {
namespace Featurizer {
namespace Featurizers {
namespace Base {

template <typename T>
struct GetNullableType {
    using type = T;
};

template <typename U>
struct GetNullableType<nonstd::optional<U>> {
    using type = U;
};

namespace {
    // tag dispatching functions for potential nullable values within iterators

    template <typename IteratorT>
    bool is_null(IteratorT const &input, std::true_type) {
        return Traits<typename std::iterator_traits<IteratorT>::value_type>::IsNull(*input);
    }

    template <typename IteratorT>
    bool is_null(IteratorT const &, std::false_type) {
        return false;
    }

    template <typename IteratorT>
    typename GetNullableType<typename std::iterator_traits<IteratorT>::value_type>::type const& get_value(IteratorT const &input, std::true_type) {
        return Traits<typename std::iterator_traits<IteratorT>::value_type>::GetNullableValue(*input);
    }

    template <typename IteratorT>
    typename std::iterator_traits<IteratorT>::value_type const& get_value(IteratorT const &input, std::false_type) {
        return *input;
    }
} // anonymous namespace

/////////////////////////////////////////////////////////////////////////
///  \class         NormalizerInput
///  \brief         Supplies necessary information in input/output
///
template <typename ValueType, typename InputIter, typename OutputIter>
struct NormalizerInput {
    using value_type = ValueType;
    using iterator_type = InputIter;
    using output_iterator_type = OutputIter;
    using iterator_range = std::pair<iterator_type, iterator_type>;
    // Space allocation callback. Takes a space required
    // returns output iterator to the newly allocated buffer
    using alloc_callback = std::function<OutputIter(size_t)>;

    iterator_type begin_;
    size_t        input_size_;
    alloc_callback callback_;
};

template <typename ValueType, typename OutputIter, typename InputIter, typename Callback>
NormalizerInput<ValueType, InputIter, OutputIter> make_normalizer_input(InputIter begin, size_t input_size, const Callback& cb) {
    return NormalizerInput<ValueType, InputIter, OutputIter>{ begin, input_size, cb };
}

/////////////////////////////////////////////////////////////////////////
///  \class         NormalizeTransformer
///  \brief         Normalize based on l1, l2 or max norm
///
// TODO: We may further restrict InputData to comply with NormalizerInput interface
template <typename InputData>
class NormalizeTransformer : public StandardTransformer<InputData, size_t> {
public:
    // ----------------------------------------------------------------------
    // |
    // |  Public Types
    // |
    // ----------------------------------------------------------------------

    // static_assert(Components::IsIteratorPair<IteratorRangeT>::value, "VectorNormsEstimator input type need to a pair of iterators of same type!");


    using BaseType     = StandardTransformer<InputData, size_t>;

    // ----------------------------------------------------------------------
    // |
    // |  Public Methods
    // |
    // ----------------------------------------------------------------------
    NormalizeTransformer(std::vector<std::double_t> norms);
    NormalizeTransformer(Archive &ar);

    ~NormalizeTransformer(void) override = default;

    FEATURIZER_MOVE_CONSTRUCTOR_ONLY(NormalizeTransformer);

    bool operator==(NormalizeTransformer const &other) const;

    void save(Archive &ar) const override;

private:
    // ----------------------------------------------------------------------
    // |
    // |  Private Types
    // |
    // ----------------------------------------------------------------------
    using IteratorType = typename InputData::interator_type;
    using OutputIteratorType = typename InputData::output_iterator_type;
    using ValueType    = typename InputData::value_type;

    // ----------------------------------------------------------------------
    // |
    // |  Private Data
    // |
    // ----------------------------------------------------------------------
    std::vector<std::double_t> const                   _norms;
    size_t                                             _row;

    // ----------------------------------------------------------------------
    // |
    // |  Private Methods
    // |
    // ----------------------------------------------------------------------
    void execute_impl(InputData const &input, typename BaseType::CallbackFunction const &callback) override;

    //void execute_impl(IteratorType &begin, IteratorType const &end, typename BaseType::CallbackFunction const &callback, std::true_type);
    //void execute_impl(IteratorType &begin, IteratorType const &end, typename BaseType::CallbackFunction const &callback, std::false_type);
};

namespace Details {

/////////////////////////////////////////////////////////////////////////
///  \class         NormalizeEstimatorImpl
///  \brief         Estimator that reads an annotation created by the `VectorNormsEstimator`
///                 to create a `NormalizeTransformer` object.
///
template <
    typename InputData,
    typename UpdaterT,
    size_t MaxNumTrainingItemsV=std::numeric_limits<size_t>::max()
>
class NormalizeEstimatorImpl : public TransformerEstimator<InputData, size_t> {
public:
    // ----------------------------------------------------------------------
    // |
    // |  Public Types
    // |
    // ----------------------------------------------------------------------
    using TransformedType                   = size_t;
    using BaseType                          = TransformerEstimator<InputData, TransformedType>;
    using TransformerType                   = NormalizeTransformer<InputData>;
    using IteratorRangeT                    = typename InputData::iterator_range;

    // ----------------------------------------------------------------------
    // |
    // |  Public Methods
    // |
    // ----------------------------------------------------------------------
    NormalizeEstimatorImpl(AnnotationMapsPtr pAllColumnAnnotations, size_t colIndex);
    ~NormalizeEstimatorImpl(void) override = default;

    FEATURIZER_MOVE_CONSTRUCTOR_ONLY(NormalizeEstimatorImpl);

private:
    // ----------------------------------------------------------------------
    // |
    // |  Private Data
    // |
    // ----------------------------------------------------------------------
    size_t const                            _colIndex;

    // ----------------------------------------------------------------------
    // |
    // |  Private Methods
    // |
    // ----------------------------------------------------------------------
    bool begin_training_impl(void) override;
    FitResult fit_impl(InputData const *, size_t) override;
    void complete_training_impl(void) override;

    // MSVC has problems when the definition is separate from the declaration
    typename BaseType::TransformerUniquePtr create_transformer_impl(void) override {
        // ----------------------------------------------------------------------
        using VectorNormsAnnotationData = Components::VectorNormsAnnotationData<UpdaterT>;
        using VectorNormsEstimator       = Components::VectorNormsEstimator<IteratorRangeT, UpdaterT, MaxNumTrainingItemsV>;
        // ----------------------------------------------------------------------

        VectorNormsAnnotationData const & data(VectorNormsEstimator::get_annotation_data(this->get_column_annotations(), _colIndex, Components::VectorNormsEstimatorName));

        return typename BaseType::TransformerUniquePtr(new NormalizeTransformer<InputData>(data.Norms));
    }
};

} // namespace Details

/////////////////////////////////////////////////////////////////////////
///  \class         NormalizeEstimatorBase
///  \brief         Chains up VectorNormsEstimator and NormalizeTransformerImpl
///                 to normalize input vector data so that it's norm becoming 1
///
template <
    typename InputData,
    typename UpdaterT,
    size_t MaxNumTrainingItemsV=std::numeric_limits<size_t>::max()
>
class NormalizeEstimatorBase :
    public Components::PipelineExecutionEstimatorImpl<
        Components::VectorNormsEstimator<typename InputData::iterator_range, UpdaterT, MaxNumTrainingItemsV>,
        Details::NormalizeEstimatorImpl<InputData, UpdaterT, MaxNumTrainingItemsV>
    > {
public:
    // ----------------------------------------------------------------------
    // |
    // |  Public Types
    // |
    // ----------------------------------------------------------------------
    using BaseType =
        Components::PipelineExecutionEstimatorImpl<
            Components::VectorNormsEstimator<typename InputData::iterator_range, UpdaterT, MaxNumTrainingItemsV>,
            Details::NormalizeEstimatorImpl<InputData, UpdaterT, MaxNumTrainingItemsV>
        >;

    // ----------------------------------------------------------------------
    // |
    // |  Public Methods
    // |
    // ----------------------------------------------------------------------
    NormalizeEstimatorBase(AnnotationMapsPtr pAllColumnAnnotations, size_t colIndex);
    ~NormalizeEstimatorBase(void) override = default;

    FEATURIZER_MOVE_CONSTRUCTOR_ONLY(NormalizeEstimatorBase);
};

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// |
// |  Implementation
// |
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// |
// |  NormalizeTransformer
// |
// ----------------------------------------------------------------------
template <typename InputData>
NormalizeTransformer<InputData>::NormalizeTransformer(std::vector<std::double_t> norms) :
    _norms(std::move(norms)),
    _row(0) {
        if (_norms.empty()) {
            throw std::invalid_argument("Norms input to NormalizeTransformer is empty!");
        }
        for (auto const &norm : _norms) {
            if (norm < 0) {
                throw std::invalid_argument("Norms shouldn't be less than 0!");
            }
        }
}

template <typename InputData>
NormalizeTransformer<InputData>::NormalizeTransformer(Archive &ar) :
    NormalizeTransformer(
        [&ar](void) {
            // Version
            std::uint16_t                   majorVersion(Traits<std::uint16_t>::deserialize(ar));
            std::uint16_t                   minorVersion(Traits<std::uint16_t>::deserialize(ar));

            if(majorVersion != 1 || minorVersion != 0)
                throw std::runtime_error("Unsupported archive version");

            // Data
            std::vector<std::double_t> norms(Traits<std::vector<std::double_t>>::deserialize(ar));

            return NormalizeTransformer<InputData>(std::move(norms));
        }()
    ) {
}

template <typename InputData>
void NormalizeTransformer<InputData>::save(Archive &ar) const /*override*/ {
    // Version
    Traits<std::uint16_t>::serialize(ar, 1); // Major
    Traits<std::uint16_t>::serialize(ar, 0); // Minor

    // Data
    Traits<std::vector<std::double_t>>::serialize(ar, _norms);
}

template <typename InputData>
bool NormalizeTransformer<InputData>::operator==(NormalizeTransformer const &other) const {

#if (defined __clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wfloat-equal"
#endif

    return this->_norms == other._norms;

#if (defined __clang__)
#   pragma clang diagnostic pop
#endif

}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
template <typename InputData>
void NormalizeTransformer<InputData>::execute_impl(InputData const &input, typename BaseType::CallbackFunction const &callback) /*override*/ {
    // unpack input to two iterators
    IteratorType begin = input.begin_;
    IteratorType const & end = begin + input.input_size_;

    if (_row >= _norms.size()) {
        throw std::runtime_error("Number of norms is not aligned with number of rows!");
    }

    // The output size is the same as input in this case.
    // So we ask the caller to allocate output memory and return output iterator
    OutputIteratorType output_iterator = input.callback_(input.input_size_);

    size_t output_count = 0;
    while (begin != end) {
        if(is_null(begin, std::integral_constant<bool, Microsoft::Featurizer::Traits<ValueType>::IsNullableType>())) {
            *output_iterator = Traits<std::double_t>::CreateNullValue();
            ++output_iterator;
        }
        else {
            auto result = static_cast<std::double_t>(static_cast<long double>(get_value(begin, std::integral_constant<bool, Microsoft::Featurizer::Traits<ValueType>::IsNullableType>())) / static_cast<long double>(_norms[_row]));
            *output_iterator = result;
            ++output_iterator;
        }
        ++begin;
        ++output_count;
    }
    ++_row;
    callback(output_count);
}



// ----------------------------------------------------------------------
// |
// |  NormalizeEstimatorBase
// |
// ----------------------------------------------------------------------
template <typename InputData, typename UpdaterT, size_t MaxNumTrainingItemsV>
NormalizeEstimatorBase<InputData, UpdaterT, MaxNumTrainingItemsV>::NormalizeEstimatorBase(AnnotationMapsPtr pAllColumnAnnotations, size_t colIndex) :
    BaseType(
        "NormalizeEstimatorBase",
        pAllColumnAnnotations,
        [pAllColumnAnnotations, colIndex](void) { return Components::VectorNormsEstimator<typename InputData::iterator_range, UpdaterT, MaxNumTrainingItemsV>(std::move(pAllColumnAnnotations), colIndex); },
        [pAllColumnAnnotations, colIndex](void) { return Details::NormalizeEstimatorImpl<InputData, UpdaterT, MaxNumTrainingItemsV>(std::move(pAllColumnAnnotations), colIndex); }
    ) {
}

// ----------------------------------------------------------------------
// |
// |  Details::NormalizeEstimatorImpl
// |
// ----------------------------------------------------------------------
template <typename InputData, typename UpdaterT, size_t MaxNumTrainingItemsV>
Details::NormalizeEstimatorImpl<InputData, UpdaterT, MaxNumTrainingItemsV>::NormalizeEstimatorImpl(AnnotationMapsPtr pAllColumnAnnotations, size_t colIndex) :
    BaseType("NormalizeEstimatorImpl", std::move(pAllColumnAnnotations)),
    _colIndex(
        std::move(
            [this, &colIndex](void) -> size_t & {
                if(colIndex >= this->get_column_annotations().size())
                    throw std::invalid_argument("colIndex");

                return colIndex;
            }()
        )
    ) {
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
template <typename InputData, typename UpdaterT, size_t MaxNumTrainingItemsV>
bool Details::NormalizeEstimatorImpl<InputData, UpdaterT, MaxNumTrainingItemsV>::begin_training_impl(void) /*override*/ {
    return false;
}

template <typename InputData, typename UpdaterT, size_t MaxNumTrainingItemsV>
FitResult Details::NormalizeEstimatorImpl<InputData, UpdaterT, MaxNumTrainingItemsV>::fit_impl(InputData const *, size_t) /*override*/ {
    throw std::runtime_error("This will never be called");
}

template <typename InputData, typename UpdaterT, size_t MaxNumTrainingItemsV>
void Details::NormalizeEstimatorImpl<InputData, UpdaterT, MaxNumTrainingItemsV>::complete_training_impl(void) /*override*/ {
}

} // namespace Base
} // namespace Featurizers
} // namespace Featurizer
} // namespace Microsoft
