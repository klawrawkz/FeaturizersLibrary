// ----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License
// ----------------------------------------------------------------------
#pragma once

#include "Base/NormalizeFeaturizer.h"
namespace Microsoft {
namespace Featurizer {
namespace Featurizers {

/////////////////////////////////////////////////////////////////////////
///  \typedef       MaxNormalizeEstimator
///  \brief         Normalize a vector so that its max norm is 1
///
template <
    typename InputData,
    size_t MaxNumTrainingItemsV=std::numeric_limits<size_t>::max()
>
using MaxNormalizeEstimator = Base::NormalizeEstimatorBase<
                                    InputData,
                                    Components::Updaters::MaxNormUpdater<typename InputData::value_type>,
                                    MaxNumTrainingItemsV>;

} // Featurizers
} // Featurizer
} // Microsoft
