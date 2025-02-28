#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWCNNLayerImplBase.hpp"
#include "layer/MWLayerImplFactory.hpp"
#else
#include "MWCNNLayerImplBase.hpp"
#include "MWLayerImplFactory.hpp"
#endif
#include "MWOnednnLayerImplFactory.hpp"
#include "MWOnednnCNNLayerImpl.hpp"
#include "MWOnednnTargetNetworkImpl.hpp"
 class MWCNNLayer;
#ifndef CREATE_INPUT_LAYER_IMPL_DEFINITION
#define CREATE_INPUT_LAYER_IMPL_DEFINITION

#include "MWOnednnInputLayerImpl.hpp"
MWCNNLayerImplBase* MWOnednnLayerImplFactory::createInputLayerImpl(MWCNNLayer* arg1,
MWTargetNetworkImplBase* arg2) {
return new MWOnednnTarget::MWInputLayerImpl(arg1,
static_cast<MWOnednnTarget::MWTargetNetworkImpl*>(arg2));
}

#endif

#ifndef CREATE_ELEMENTWISEAFFINE_LAYER_IMPL_DEFINITION
#define CREATE_ELEMENTWISEAFFINE_LAYER_IMPL_DEFINITION

#include "MWOnednnElementwiseAffineLayerImpl.hpp"
MWCNNLayerImplBase* MWOnednnLayerImplFactory::createElementwiseAffineLayerImpl(MWCNNLayer* arg1,
MWTargetNetworkImplBase* arg2,
int arg3,
int arg4,
int arg5,
int arg6,
int arg7,
int arg8,
bool arg9,
int arg10,
int arg11,
const char* arg12,
const char* arg13) {
return new MWOnednnTarget::MWElementwiseAffineLayerImpl(arg1,
static_cast<MWOnednnTarget::MWTargetNetworkImpl*>(arg2),
arg3,
arg4,
arg5,
arg6,
arg7,
arg8,
arg9,
arg10,
arg11,
arg12,
arg13);
}

#endif

#ifndef CREATE_BATCHNORMALIZATION_LAYER_IMPL_DEFINITION
#define CREATE_BATCHNORMALIZATION_LAYER_IMPL_DEFINITION

#include "MWOnednnBatchNormalizationLayerImpl.hpp"
MWCNNLayerImplBase* MWOnednnLayerImplFactory::createBatchNormalizationLayerImpl(MWCNNLayer* arg1,
MWTargetNetworkImplBase* arg2,
float arg3,
const char* arg4,
const char* arg5,
const char* arg6,
const char* arg7,
int arg8) {
return new MWOnednnTarget::MWBatchNormalizationLayerImpl(arg1,
static_cast<MWOnednnTarget::MWTargetNetworkImpl*>(arg2),
arg3,
arg4,
arg5,
arg6,
arg7,
arg8);
}

#endif

#ifndef CREATE_FUSEDCONVACTIVATION_LAYER_IMPL_DEFINITION
#define CREATE_FUSEDCONVACTIVATION_LAYER_IMPL_DEFINITION

#include "MWOnednnFusedConvActivationLayerImpl.hpp"
MWCNNLayerImplBase* MWOnednnLayerImplFactory::createFusedConvActivationLayerImpl(MWCNNLayer* arg1,
MWTargetNetworkImplBase* arg2,
int arg3,
int arg4,
int arg5,
int arg6,
int arg7,
int arg8,
int arg9,
int arg10,
int arg11,
int arg12,
int arg13,
int arg14,
int arg15,
int arg16,
const char* arg17,
const char* arg18,
double arg19,
MWActivationFunctionType::ACTIVATION_FCN_ENUM arg20) {
return new MWOnednnTarget::MWFusedConvActivationLayerImpl(arg1,
static_cast<MWOnednnTarget::MWTargetNetworkImpl*>(arg2),
arg3,
arg4,
arg5,
arg6,
arg7,
arg8,
arg9,
arg10,
arg11,
arg12,
arg13,
arg14,
arg15,
arg16,
arg17,
arg18,
arg19,
arg20);
}

#endif

#ifndef CREATE_MAXPOOLING_LAYER_IMPL_DEFINITION
#define CREATE_MAXPOOLING_LAYER_IMPL_DEFINITION

#include "MWOnednnMaxPoolingLayerImpl.hpp"
MWCNNLayerImplBase* MWOnednnLayerImplFactory::createMaxPoolingLayerImpl(MWCNNLayer* arg1,
MWTargetNetworkImplBase* arg2,
int arg3,
int arg4,
int arg5,
int arg6,
int arg7,
int arg8,
int arg9,
int arg10,
bool arg11,
int arg12) {
return new MWOnednnTarget::MWMaxPoolingLayerImpl(arg1,
static_cast<MWOnednnTarget::MWTargetNetworkImpl*>(arg2),
arg3,
arg4,
arg5,
arg6,
arg7,
arg8,
arg9,
arg10,
arg11,
arg12);
}

#endif

#ifndef CREATE_AVGPOOLING_LAYER_IMPL_DEFINITION
#define CREATE_AVGPOOLING_LAYER_IMPL_DEFINITION

#include "MWOnednnAvgPoolingLayerImpl.hpp"
MWCNNLayerImplBase* MWOnednnLayerImplFactory::createAvgPoolingLayerImpl(MWCNNLayer* arg1,
MWTargetNetworkImplBase* arg2,
int arg3,
int arg4,
int arg5,
int arg6,
int arg7,
int arg8,
int arg9,
int arg10,
bool arg11) {
return new MWOnednnTarget::MWAvgPoolingLayerImpl(arg1,
static_cast<MWOnednnTarget::MWTargetNetworkImpl*>(arg2),
arg3,
arg4,
arg5,
arg6,
arg7,
arg8,
arg9,
arg10,
arg11);
}

#endif

#ifndef CREATE_FC_LAYER_IMPL_DEFINITION
#define CREATE_FC_LAYER_IMPL_DEFINITION

#include "MWOnednnFCLayerImpl.hpp"
MWCNNLayerImplBase* MWOnednnLayerImplFactory::createFCLayerImpl(MWCNNLayer* arg1,
MWTargetNetworkImplBase* arg2,
int arg3,
int arg4,
const char* arg5,
const char* arg6) {
return new MWOnednnTarget::MWFCLayerImpl(arg1,
static_cast<MWOnednnTarget::MWTargetNetworkImpl*>(arg2),
arg3,
arg4,
arg5,
arg6);
}

#endif

#ifndef CREATE_SOFTMAX_LAYER_IMPL_DEFINITION
#define CREATE_SOFTMAX_LAYER_IMPL_DEFINITION

#include "MWOnednnSoftmaxLayerImpl.hpp"
MWCNNLayerImplBase* MWOnednnLayerImplFactory::createSoftmaxLayerImpl(MWCNNLayer* arg1,
MWTargetNetworkImplBase* arg2) {
return new MWOnednnTarget::MWSoftmaxLayerImpl(arg1,
static_cast<MWOnednnTarget::MWTargetNetworkImpl*>(arg2));
}

#endif

#ifndef CREATE_OUTPUT_LAYER_IMPL_DEFINITION
#define CREATE_OUTPUT_LAYER_IMPL_DEFINITION

#include "MWOnednnOutputLayerImpl.hpp"
MWCNNLayerImplBase* MWOnednnLayerImplFactory::createOutputLayerImpl(MWCNNLayer* arg1,
MWTargetNetworkImplBase* arg2) {
return new MWOnednnTarget::MWOutputLayerImpl(arg1,
static_cast<MWOnednnTarget::MWTargetNetworkImpl*>(arg2));
}

#endif
