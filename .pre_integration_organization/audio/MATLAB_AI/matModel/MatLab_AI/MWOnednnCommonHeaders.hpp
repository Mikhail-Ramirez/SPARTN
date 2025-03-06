/* Copyright 2021-2022 The MathWorks, Inc. */

/* Include this in mkldnn layer files that call into MKLDNN APIs. If a layer does not call into MKLDNN APIs, it should not include this header, as it will probably not need to include MWOnednnTargetNetworkImpl.hpp (it should suffice to forward declare the class instead). */

#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWCNNLayer.hpp"
#else
#include "MWCNNLayer.hpp"
#endif

#include "MWOnednnCNNLayerImpl.hpp"
#include "MWOnednnTargetNetworkImpl.hpp"
#include "MWOnednnUtils.hpp"

#include "dnnl.hpp"
