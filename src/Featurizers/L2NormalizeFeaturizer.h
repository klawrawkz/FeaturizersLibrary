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
///  \typedef       L2NormalizeEstimator
///  \brief         Normalize a vector so that its l2 norm is 1
///
template <
    typename InputData,
    size_t MaxNumTrainingItemsV=std::numeric_limits<size_t>::max()
>
using L2NormalizeEstimator = Base::NormalizeEstimatorBase<
                                    InputData,
                                    Components::Updaters::L2NormUpdater<typename InputData::value_type>,
                                    MaxNumTrainingItemsV>;

} // Featurizers
} // Featurizer
} // Microsoft
