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
///  \typedef       L1NormalizeEstimator
///  \brief         Normalize a vector so that its l1 norm is 1
///
template <
    typename InputData,
    size_t MaxNumTrainingItemsV=std::numeric_limits<size_t>::max()
>
using L1NormalizeEstimator = Base::NormalizeEstimatorBase<
                                    InputData,
                                    Components::Updaters::L1NormUpdater<typename InputData::value_type>,
                                    MaxNumTrainingItemsV>;



} // Featurizers
} // Featurizer
} // Microsoft
