// ----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License
// ----------------------------------------------------------------------
#pragma once

#include "Components/PipelineExecutionEstimatorImpl.h"
#include "Components/DocumentStatisticsEstimator.h"
#include "../Traits.h"

namespace Microsoft {
namespace Featurizer {
namespace Featurizers {

/////////////////////////////////////////////////////////////////////////
///  \struct        TFStruct
///  \brief         Struct to hold return value of count vectorizer,
///                 consist of <dictid, # of appearances>
///
struct TFStruct {
    std::uint32_t const DictionaryId;                              // dict id
    std::uint32_t const NumOfAppearances;                          // number of appearances

    TFStruct(std::uint32_t dictionaryId, std::uint32_t numOfAppearances);
    //FEATURIZER_MOVE_CONSTRUCTOR_ONLY(TFStruct);
    bool operator==(TFStruct const &other) const;
};

/////////////////////////////////////////////////////////////////////////
///  \class         CountVectorizerTransformer
///  \brief         Returns TFStruct for each unique input
///
class CountVectorizerTransformer : public StandardTransformer<std::string, TFStruct> {
public:
    // ----------------------------------------------------------------------
    // |
    // |  Public Types
    // |
    // ----------------------------------------------------------------------
    using BaseType                           = StandardTransformer<std::string, TFStruct>;
    using IndexMap                           = std::unordered_map<std::string, std::uint32_t>;
    using IterRangeType                      = std::tuple<std::string::const_iterator, std::string::const_iterator>;
    using MapWithIterRange                   = std::map<IterRangeType, std::uint32_t, Components::IterRangeComp>;

    // ----------------------------------------------------------------------
    // |
    // |  Public Data
    // |
    // ----------------------------------------------------------------------
    IndexMap const                           Labels;
    //Binary: If True, all non zero counts are set to 1. This is useful for discrete probabilistic models that model binary events rather than integer counts.
    bool const                               Binary;

    // ----------------------------------------------------------------------
    // |
    // |  Public Methods
    // |
    // ----------------------------------------------------------------------
    CountVectorizerTransformer(IndexMap map, bool binary);
    CountVectorizerTransformer(Archive &ar);

    ~CountVectorizerTransformer(void) override = default;

    void save(Archive &ar) const override;

    bool operator==(CountVectorizerTransformer const &other) const;

private:
    // ----------------------------------------------------------------------
    // |
    // |  Private Methods
    // |
    // ----------------------------------------------------------------------

    // MSVC has problems when the definition and declaration are separated
    void execute_impl(typename BaseType::InputType const &input, typename BaseType::CallbackFunction const &callback) override {
        //termfrequency for specific document
        MapWithIterRange documentTermFrequency;

        //todo: will use vector<functor> after string header file is done
        Components::split_temp( 
            input, 
            [&documentTermFrequency] (std::string::const_iterator & iterStart, std::string::const_iterator & iterEnd) {
                MapWithIterRange::iterator docuTermFreqIter(documentTermFrequency.find(std::make_tuple(iterStart, iterEnd)));
                if (docuTermFreqIter != documentTermFrequency.end()) {
                    ++docuTermFreqIter->second;
                } else {
                    documentTermFrequency.insert(std::make_pair(std::make_tuple(iterStart, iterEnd), 1));
                }
            }
        );

        for (auto const & pair : documentTermFrequency) {
            std::string const word(std::string(std::get<0>(pair.first), std::get<1>(pair.first)));
            IndexMap::const_iterator const      labelIter(Labels.find(word));
            if (labelIter != Labels.end()) {
                if (Binary) {
                    callback(TFStruct(labelIter->second, 1));
                } else {
                    callback(TFStruct(labelIter->second, pair.second));
                }
            }
        }
                                                                                                         
    }
};

namespace Details {

/////////////////////////////////////////////////////////////////////////
///  \class         CountVectorizerEstimatorImpl
///  \brief         Estimator that uses the output of the 
///                 InverseDocumentFrequencyEstimator to provide useful
///                 information which helps calculation of CountVectorizer
template <size_t MaxNumTrainingItemsV=std::numeric_limits<size_t>::max()>
class CountVectorizerEstimatorImpl : public TransformerEstimator<std::string, TFStruct> {
public:
    // ----------------------------------------------------------------------
    // |
    // |  Public Types
    // |
    // ----------------------------------------------------------------------
    using BaseType                          = TransformerEstimator<std::string, TFStruct>;
    using TransformerType                   = CountVectorizerTransformer;
    using IndexMap                          = CountVectorizerTransformer::IndexMap;

    // ----------------------------------------------------------------------
    // |
    // |  Public Methods
    // |
    // ----------------------------------------------------------------------
    CountVectorizerEstimatorImpl(AnnotationMapsPtr pAllColumnAnnotations, size_t colIndex, bool binary);
    ~CountVectorizerEstimatorImpl(void) override = default;

    FEATURIZER_MOVE_CONSTRUCTOR_ONLY(CountVectorizerEstimatorImpl);

private:
    // ----------------------------------------------------------------------
    // |
    // |  Private Data
    // |
    // ----------------------------------------------------------------------
    size_t const                             _colIndex;
    bool const                               _binary;

    // ----------------------------------------------------------------------
    // |
    // |  Private Methods
    // |
    // ----------------------------------------------------------------------
    bool begin_training_impl(void) override;

    // MSVC has problems when the declaration and definition are separated
    FitResult fit_impl(typename BaseType::InputType const *, size_t) override {
        throw std::runtime_error("This should not be called");
    }

    void complete_training_impl(void) override;

    // MSVC has problems when the declaration and definition are separated
    typename BaseType::TransformerUniquePtr create_transformer_impl(void) override {
       
        // ----------------------------------------------------------------------
        using DocumentStatisticsAnnotationData          = Components::DocumentStatisticsAnnotationData;
        using DocumentStatisticsEstimator               = Components::DocumentStatisticsEstimator<MaxNumTrainingItemsV>;

        // ----------------------------------------------------------------------

        DocumentStatisticsAnnotationData const &        data(DocumentStatisticsEstimator::get_annotation_data(BaseType::get_column_annotations(), _colIndex, Components::DocumentStatisticsEstimatorName));

        
        typename DocumentStatisticsAnnotationData::FrequencyAndIndexMap const & 
                                                        termFrequencyAndIndex(data.TermFrequencyAndIndex);

        //generate termIndex from termFrequencyAndIndex, better to use lambda function
        IndexMap termIndex;
        for (auto const & termFrequencyAndIndexPair : termFrequencyAndIndex)
            termIndex.insert(std::make_pair(termFrequencyAndIndexPair.first, termFrequencyAndIndexPair.second.Index));

        return std::make_unique<CountVectorizerTransformer>(std::move(termIndex), std::move(_binary));
    }
};

} // namespace Details

/////////////////////////////////////////////////////////////////////////
///  \class         CountVectorizerEstimator
///  \brief         Creates a `CountVectorizerTransformer` object.
///
template <size_t MaxNumTrainingItemsV=std::numeric_limits<size_t>::max()>
class CountVectorizerEstimator :
    public Components::PipelineExecutionEstimatorImpl<
        Components::DocumentStatisticsEstimator<MaxNumTrainingItemsV>,
        Details::CountVectorizerEstimatorImpl<MaxNumTrainingItemsV>
    > {
public:
    // ----------------------------------------------------------------------
    // |
    // |  Public Types
    // |
    // ----------------------------------------------------------------------
    using BaseType =
        Components::PipelineExecutionEstimatorImpl<
            Components::DocumentStatisticsEstimator<MaxNumTrainingItemsV>,
            Details::CountVectorizerEstimatorImpl<MaxNumTrainingItemsV>
        >;

    using IndexMap                          = CountVectorizerTransformer::IndexMap;
    using StringDecorator                   = Components::Details::DocumentStatisticsTrainingOnlyPolicy::StringDecorator;
    // ----------------------------------------------------------------------
    // |
    // |  Public Methods
    // |
    // ----------------------------------------------------------------------
    CountVectorizerEstimator(
        AnnotationMapsPtr pAllColumnAnnotations, 
        size_t colIndex, 
        bool binary = false,
        std::float_t minDf = 0.0f, 
        std::float_t maxDf = 1.0f,
        StringDecorator decorator = StringDecorator(), 
        nonstd::optional<IndexMap> vocabulary = nonstd::optional<IndexMap>(), 
        nonstd::optional<std::uint32_t> maxFeatures = nonstd::optional<std::uint32_t>()
    );
    ~CountVectorizerEstimator(void) override = default;

    FEATURIZER_MOVE_CONSTRUCTOR_ONLY(CountVectorizerEstimator);
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
// |  TFStruct
// |
// ----------------------------------------------------------------------
TFStruct::TFStruct(std::uint32_t dictionaryId, std::uint32_t numOfAppearances) :
    DictionaryId(std::move(dictionaryId)),
    NumOfAppearances(std::move(numOfAppearances)) {
}
bool TFStruct::operator==(TFStruct const &other) const {
    return (DictionaryId == other.DictionaryId) && (NumOfAppearances == other.NumOfAppearances);
}

// ----------------------------------------------------------------------
// |
// |  CountVectorizerTransformer
// |
// ----------------------------------------------------------------------
CountVectorizerTransformer::CountVectorizerTransformer(IndexMap map, bool binary) :
    Labels(
        std::move([&map](void) ->  IndexMap & {
            if (map.size() == 0) {
                throw std::invalid_argument("Index map is empty!");
            }
            return map;
        }()
        )),
    Binary(std::move(binary)) {
}

CountVectorizerTransformer::CountVectorizerTransformer(Archive &ar) :
    // TODO: Labels(Traits<decltype(Labels)>::deserialize(ar)),
    Binary(Traits<decltype(Binary)>::deserialize(ar)) {
}

void CountVectorizerTransformer::save(Archive &ar) const /*override*/ {
    // TODO: Traits<decltype(Labels)>::serialize(ar, Labels);
    Traits<decltype(Binary)>::serialize(ar, Binary);
}

bool CountVectorizerTransformer::operator==(CountVectorizerTransformer const &other) const {
    return Labels == other.Labels
        && Binary == other.Binary;
}

// ----------------------------------------------------------------------
// |
// |  CountVectorizerEstimator
// |
// ----------------------------------------------------------------------

template <size_t MaxNumTrainingItemsV>
CountVectorizerEstimator<MaxNumTrainingItemsV>::CountVectorizerEstimator(
    AnnotationMapsPtr pAllColumnAnnotations, 
    size_t colIndex, 
    bool binary,
    std::float_t minDf, 
    std::float_t maxDf,
    StringDecorator decorator, 
    nonstd::optional<IndexMap> vocabulary, 
    nonstd::optional<std::uint32_t> maxFeatures
) :
    BaseType(
        "CountVectorizerEstimator",
        pAllColumnAnnotations,
        [pAllColumnAnnotations, colIndex, &decorator, &vocabulary, &maxFeatures, &minDf, &maxDf](void) { 
            return Components::DocumentStatisticsEstimator<MaxNumTrainingItemsV>(
                std::move(pAllColumnAnnotations), 
                std::move(colIndex),
                std::move(decorator),
                std::move(vocabulary),
                std::move(maxFeatures),
                std::move(minDf),
                std::move(maxDf)
            ); 
        },
        [pAllColumnAnnotations, colIndex, &binary](void) { 
            return Details::CountVectorizerEstimatorImpl<MaxNumTrainingItemsV>(
                std::move(pAllColumnAnnotations), 
                std::move(colIndex), 
                std::move(binary)
            ); 
        }
    ) {
}

// ----------------------------------------------------------------------
// |
// |  Details::CountVectorizerEstimatorImpl
// |
// ----------------------------------------------------------------------
template <size_t MaxNumTrainingItemsV>
Details::CountVectorizerEstimatorImpl<MaxNumTrainingItemsV>::CountVectorizerEstimatorImpl(AnnotationMapsPtr pAllColumnAnnotations, size_t colIndex, bool binary) :
    BaseType("CountVectorizerEstimatorImpl", std::move(pAllColumnAnnotations)),
    _colIndex(
        std::move(
            [this, &colIndex](void) -> size_t & {
                if(colIndex >= this->get_column_annotations().size())
                    throw std::invalid_argument("colIndex");

                return colIndex;
            }()
        )
    ),
    _binary(std::move(binary)) {
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
template <size_t MaxNumTrainingItemsV>
bool Details::CountVectorizerEstimatorImpl<MaxNumTrainingItemsV>::begin_training_impl(void) /*override*/ {
    return false;
}

template <size_t MaxNumTrainingItemsV>
void Details::CountVectorizerEstimatorImpl<MaxNumTrainingItemsV>::complete_training_impl(void) /*override*/ {
}

} // namespace Featurizers
} // namespace Featurizer
} // namespace Microsoft
