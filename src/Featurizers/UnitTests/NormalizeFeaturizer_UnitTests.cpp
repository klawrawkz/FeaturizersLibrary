// ----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License
// ----------------------------------------------------------------------
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../../3rdParty/optional.h"
#include "../../Featurizers/L1NormalizeFeaturizer.h"
#include "../../Featurizers/L2NormalizeFeaturizer.h"
#include "../../Featurizers/MaxNormalizeFeaturizer.h"
#include "../TestHelpers.h"
#include "../../Traits.h"
#include "../Structs.h"
namespace NS = Microsoft::Featurizer;

template<typename ValueType, typename InputIter, typename OutIter>
using InputData = NS::Featurizers::Base::NormalizerInput<ValueType, InputIter, OutIter>;

namespace {
template <typename EstimatorT, typename InputT>
void TrainEx(EstimatorT& estimator, std::vector<InputT> const& inputBatches) {
    // ----------------------------------------------------------------------
    // using Batches = std::vector<std::vector<InputT>>;
    // ----------------------------------------------------------------------

    estimator.begin_training();

    typename InputT::const_iterator        iter(inputBatches.begin());

    while(estimator.get_state() == NS::TrainingState::Training) {
        auto range = std::make_pair(iter->begin_, iter->begin_ + iter->input_size_);
        if(estimator.fit(range) == NS::FitResult::Reset) {
            iter = inputBatches.begin();
            continue;
         }

        ++iter;
        if(iter == inputBatches.end()) {
            estimator.on_data_completed();

            iter = inputBatches.begin();
            }
        }

    estimator.complete_training();
}

template <typename EstimatorT>
typename EstimatorT::TransformedType PredictEx(EstimatorT& estimator, 
    typename EstimatorT::InputType const& data) {

    typename EstimatorT::TransformerUniquePtr           pTransformer(estimator.create_transformer());

    size_t output_count = 0;
    auto const                              callback(
        [&output_count](typename EstimatorT::TransformedType value) mutable {
        output_count += value;
      }
    );

    pTransformer->execute(data, callback);

    pTransformer->flush(callback);

    return output_count;
}


template <typename EstimatorT>
size_t TransformerEstimatorTestEx(
    EstimatorT estimator,
    std::vector<typename EstimatorT::InputData> const& inputData,
    typename EstimatorT::InputType const& data) {

    TrainEx<EstimatorT>(estimator, inputData);
    return PredictEx(estimator, data);
}
}

TEST_CASE("double_t - max norm") {
    using ValueType = std::double_t;
    using TransformedType = std::double_t;
    using OutIter = std::back_insert_iterator<std::vector<ValueType>>;
    using InputType = InputData<ValueType, std::vector<ValueType>::const_iterator, OutIter>;

    std::vector<ValueType> row1({ 7.9, 4.37, 6, 10 });
    std::vector<ValueType> output;

    std::function<OutIter(size_t)> alloc_callback;
    alloc_callback = [&output](size_t size) mutable -> OutIter {
        output.reserve(size);
        return std::back_inserter(output);
        };

    auto input_data = NS::Featurizers::Base::make_normalizer_input<ValueType, OutIter>(row1.cbegin(), row1.size(), alloc_callback);
    auto trainingBatches = std::vector<InputType>(NS::TestHelpers::make_vector<InputType>(input_data));

    auto inferencingOutput = NS::TestHelpers::make_vector<std::double_t>(0.79, 0.437, 0.6, 1.0);

    NS::AnnotationMapsPtr const                                          pAllColumnAnnotations(NS::CreateTestAnnotationMapsPtr(1));
    auto estimator = NS::Featurizers::MaxNormalizeEstimator<InputType>(pAllColumnAnnotations, 0);

    size_t result_count = TransformerEstimatorTestEx(estimator, trainingBatches, input_data);

    //CHECK(
    //    NS::TestHelpers::TransformerEstimatorTest(
    //    NS::Featurizers::MaxNormalizeEstimator<InputType>(pAllColumnAnnotations, 0),
    //    trainingBatches,
    //    inferencingInput
    //) == inferencingOutput
    //);
}

