#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTensorBase.hpp"
#include "layer/MWTensor.hpp"
#else
#include "MWTensorBase.hpp"
#include "MWTensor.hpp"
#endif
#include "MWOnednnCommonHeaders.hpp"
#include "MWOnednnFusedConvActivationLayerImpl.hpp"
#include <memory>
#include <cassert>
#include <vector>
 namespace MWOnednnTarget {
#if MW_CONV_TAP
 extern int tap_count; extern void mw_interm_tap(float* inp, int size, int count);
#endif
 MWFusedConvActivationLayerImpl::MWFusedConvActivationLayerImpl( MWCNNLayer* 
layer, MWTargetNetworkImpl* ntwk_impl, int filt_H, int filt_W, int numGrps, int 
numChnls, int numFilts, int GsZlHFuhbvjLtRMDjXnW, int HJHXkKmgFxxIOsIvRRnF, int 
CqtPRJvHlGJFssiPzsOm, int CpMjJjtGOeWOzwxpAAQP, int ECTnqgWHyHCHCLBZlffd, 
int EvebzoroiuKkIxwjkGnD, int AIXLuRgdeiqpaCehGSYD, int 
ATEikvMQPqBefhJzjzhc, int fOpFYwKNwIfWjnPzNuob, const char* 
yCdIUfwoZFngCRRRkCTg, const char* NtWaRGCHLeTapjWdEHHS, double 
activationAlpha, MWActivationFunctionType::ACTIVATION_FCN_ENUM 
JsZenQeBPMhwsyEhVHiD) : MWCNNLayerImpl(layer, ntwk_impl) , 
AzTsxYcYjIEJsGQbeYHm(filt_H) , BLjrjqvCcCommiXWQLjs(filt_W) , 
CGbFsczkgkhjcHoCKzBx(numGrps) , GsZlHFuhbvjLtRMDjXnW(GsZlHFuhbvjLtRMDjXnW) , 
HJHXkKmgFxxIOsIvRRnF(HJHXkKmgFxxIOsIvRRnF) , 
CqtPRJvHlGJFssiPzsOm(CqtPRJvHlGJFssiPzsOm) , 
CpMjJjtGOeWOzwxpAAQP(CpMjJjtGOeWOzwxpAAQP) , 
ECTnqgWHyHCHCLBZlffd(ECTnqgWHyHCHCLBZlffd) , 
EvebzoroiuKkIxwjkGnD(EvebzoroiuKkIxwjkGnD) , 
AIXLuRgdeiqpaCehGSYD(AIXLuRgdeiqpaCehGSYD) , 
ATEikvMQPqBefhJzjzhc(ATEikvMQPqBefhJzjzhc) { fxxCPKTclxXPxrdMAkwi 
= numChnls * numFilts * CGbFsczkgkhjcHoCKzBx * AzTsxYcYjIEJsGQbeYHm * 
BLjrjqvCcCommiXWQLjs; fXhhiexIRPLyKXApPmmy = numFilts * CGbFsczkgkhjcHoCKzBx; 
vIWQzNvYZSuxmOTVDFhU = (float*)calloc(fxxCPKTclxXPxrdMAkwi, sizeof(float)); 
MdSWZSOAjugbWppryHbR = (float*)calloc(fXhhiexIRPLyKXApPmmy, sizeof(float)); 
loadWeights(yCdIUfwoZFngCRRRkCTg); loadBias(NtWaRGCHLeTapjWdEHHS); auto 
weights_md = CGbFsczkgkhjcHoCKzBx > 1 ? 
dnnl::memory::desc({{CGbFsczkgkhjcHoCKzBx, numFilts, numChnls, 
AzTsxYcYjIEJsGQbeYHm, BLjrjqvCcCommiXWQLjs}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::goihw}) : dnnl::memory::desc({{numFilts, numChnls, 
AzTsxYcYjIEJsGQbeYHm, BLjrjqvCcCommiXWQLjs}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::oihw}); weights = std::shared_ptr<dnnl::memory>( new 
dnnl::memory(weights_md, *eFaDPmxDdzHlRYSAoMmX->eng, vIWQzNvYZSuxmOTVDFhU)); auto 
bias_md = with_bias ? dnnl::memory::desc({{numFilts * CGbFsczkgkhjcHoCKzBx}, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::x}) : 
dnnl::memory::desc({{}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::x}); bias = with_bias ? 
std::shared_ptr<dnnl::memory>( new dnnl::memory(bias_md, 
*eFaDPmxDdzHlRYSAoMmX->eng, MdSWZSOAjugbWppryHbR)) : std::shared_ptr<dnnl::memory>(new 
dnnl::memory(bias_md, *eFaDPmxDdzHlRYSAoMmX->eng)); conv_bias_md = 
std::unique_ptr<dnnl::memory::desc>( new dnnl::memory::desc({numFilts * 
CGbFsczkgkhjcHoCKzBx}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::any)); conv_weights_md = CGbFsczkgkhjcHoCKzBx > 1 ? 
std::unique_ptr<dnnl::memory::desc>(new dnnl::memory::desc( 
{CGbFsczkgkhjcHoCKzBx, numFilts, numChnls, AzTsxYcYjIEJsGQbeYHm, 
BLjrjqvCcCommiXWQLjs}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::any)) : std::unique_ptr<dnnl::memory::desc>(new 
dnnl::memory::desc( {numFilts, numChnls, AzTsxYcYjIEJsGQbeYHm, 
BLjrjqvCcCommiXWQLjs}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::any)); reorderedWeightsDesc = weights->get_desc(); if 
(fOpFYwKNwIfWjnPzNuob == 2) { RAtlBpdedvgxUsgDTsch.append_sum(1.0f); } float 
scale = 1.0f; float beta = 1.0f;  float m_activationAlpha = 
static_cast<float>(activationAlpha); RAtlBpdedvgxUsgDTsch.append_eltwise( 
scale, AwZQzUhuWVLGrWgLHRuM[JsZenQeBPMhwsyEhVHiD], m_activationAlpha, 
beta); } void MWFusedConvActivationLayerImpl::propagateSize() { 
clearLayerMapsAndVectors(); MWCNNLayer* convLayer = getLayer(); MWTensorBase* 
ipTensor = convLayer->getInputTensor(0); MWTensorBase* opTensor = 
convLayer->getOutputTensor(0); int numInputs = (int)getLayer()->getNumInputs(); 
int n = ipTensor->getBatchSize(); int c = ipTensor->getChannels(); int h = 
ipTensor->getHeight(); int w = ipTensor->getWidth(); int num_filts = 
opTensor->getChannels(); int UpnEytIWGokwbTFkBcSx = 
convLayer->getInPlaceIndex(0); if (numInputs == 2 && UpnEytIWGokwbTFkBcSx != 
-1) { assert(UpnEytIWGokwbTFkBcSx == 1); } dnnl::primitive_attr attr; 
attr.set_post_ops(RAtlBpdedvgxUsgDTsch); conv_src_md = 
std::unique_ptr<dnnl::memory::desc>(new dnnl::memory::desc( {n, c, h, w}, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::any)); conv_dst_md = 
std::unique_ptr<dnnl::memory::desc>( new dnnl::memory::desc({n, num_filts, 
opTensor->getHeight(), opTensor->getWidth()}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::any)); conv_d = with_bias ? 
std::unique_ptr<dnnl::convolution_forward::desc>(new 
dnnl::convolution_forward::desc( dnnl::prop_kind::forward_inference, 
dnnl::algorithm::convolution_direct, *conv_src_md, *conv_weights_md, 
*conv_bias_md, *conv_dst_md, {GsZlHFuhbvjLtRMDjXnW, HJHXkKmgFxxIOsIvRRnF}, 
{AIXLuRgdeiqpaCehGSYD - 1, ATEikvMQPqBefhJzjzhc - 1}, 
{CqtPRJvHlGJFssiPzsOm, ECTnqgWHyHCHCLBZlffd}, {CpMjJjtGOeWOzwxpAAQP, 
EvebzoroiuKkIxwjkGnD})) : std::unique_ptr<dnnl::convolution_forward::desc>(new 
dnnl::convolution_forward::desc( dnnl::prop_kind::forward_inference, 
dnnl::algorithm::convolution_direct, *conv_src_md, *conv_weights_md, 
*conv_dst_md, {GsZlHFuhbvjLtRMDjXnW, HJHXkKmgFxxIOsIvRRnF}, 
{AIXLuRgdeiqpaCehGSYD - 1, ATEikvMQPqBefhJzjzhc - 1}, 
{CqtPRJvHlGJFssiPzsOm, ECTnqgWHyHCHCLBZlffd}, {CpMjJjtGOeWOzwxpAAQP, 
EvebzoroiuKkIxwjkGnD})); conv_pd = 
std::unique_ptr<dnnl::convolution_forward::primitive_desc>( new 
dnnl::convolution_forward::primitive_desc(*conv_d, attr, 
*eFaDPmxDdzHlRYSAoMmX->eng)); conv_src_memory = 
MWOnednnUtils::getLayerMemoryPrimitive(ipTensor); if 
(dnnl::memory::desc(conv_pd->src_desc()) != 
MWOnednnUtils::getLayerMemoryPrimitive(ipTensor)->get_desc()) { 
assert(MWOnednnUtils::getformatType(MWOnednnUtils::getLayerMemoryPrimitive(ipTensor)->get_desc(), 
ipTensor) != format_type::TNC_FORMAT); conv_src_memory = 
std::make_shared<dnnl::memory>(conv_pd->src_desc(), *eFaDPmxDdzHlRYSAoMmX->eng); 
conv_reorder_src = std::unique_ptr<dnnl::reorder>( new 
dnnl::reorder(*MWOnednnUtils::getLayerMemoryPrimitive(ipTensor), 
*conv_src_memory)); pipeline_ipReorder_memory_args.push_back( {{DNNL_ARG_FROM, 
*MWOnednnUtils::getLayerMemoryPrimitive(ipTensor)}, {DNNL_ARG_TO, 
*conv_src_memory}}); pipeline_ipReorder.push_back(*conv_reorder_src); } if 
(dnnl::memory::desc(conv_pd->weights_desc()) != weights->get_desc()) { if 
(conv_pd->weights_desc() != reorderedWeightsDesc) { conv_weights_memory = 
std::make_shared<dnnl::memory>(conv_pd->weights_desc(), 
*eFaDPmxDdzHlRYSAoMmX->eng); conv_reorder_weights = 
std::unique_ptr<dnnl::reorder>(new dnnl::reorder(*weights, 
*conv_weights_memory)); pipeline_weights_memory_args.push_back( 
{{DNNL_ARG_FROM, *weights}, {DNNL_ARG_TO, *conv_weights_memory}}); 
pipeline_weights.push_back(*conv_reorder_weights); 
executeWeightsReorderPrimitive(); reorderedWeightsDesc = 
conv_pd->weights_desc(); } } else { conv_weights_memory = weights; } 
setLayerMemory(std::make_shared<dnnl::memory>(conv_pd->dst_desc(), 
*eFaDPmxDdzHlRYSAoMmX->eng)); conv = 
std::unique_ptr<dnnl::convolution_forward>(new 
dnnl::convolution_forward(*conv_pd)); setReorderLayerMemory(conv_src_memory); 
if (numInputs == 2) { MWTensorBase* additionIpTensor = 
getLayer()->getInputTensor(1); std::shared_ptr<dnnl::memory> 
additionMemPrimitive; additionMemPrimitive = 
MWOnednnUtils::getLayerMemoryPrimitive(additionIpTensor); if 
(MWOnednnUtils::getLayerMemoryPrimitive(additionIpTensor)->get_desc() != 
conv_pd->dst_desc()) { additionMemPrimitive = 
std::make_shared<dnnl::memory>(conv_pd->dst_desc(), *eFaDPmxDdzHlRYSAoMmX->eng); 
addition_reorder = std::unique_ptr<dnnl::reorder>(new dnnl::reorder( 
*MWOnednnUtils::getLayerMemoryPrimitive(additionIpTensor), 
*additionMemPrimitive)); pipeline_memory_args.push_back( {{DNNL_ARG_FROM, 
*MWOnednnUtils::getLayerMemoryPrimitive(additionIpTensor)}, {DNNL_ARG_TO, 
*additionMemPrimitive}}); pipeline.push_back(*addition_reorder); } 
setReorderLayerMemory(additionMemPrimitive, 1); } if (with_bias) { 
pipeline_memory_args.push_back({{DNNL_ARG_SRC, *conv_src_memory}, 
{DNNL_ARG_WEIGHTS, *conv_weights_memory}, {DNNL_ARG_BIAS, *bias}, 
{DNNL_ARG_DST, *getLayerMemory()}}); } else { 
pipeline_memory_args.push_back({{DNNL_ARG_SRC, *conv_src_memory}, 
{DNNL_ARG_WEIGHTS, *conv_weights_memory}, {DNNL_ARG_DST, *getLayerMemory()}}); 
} pipeline.push_back(*conv); return; } void 
MWFusedConvActivationLayerImpl::predict() { executeInputsReorderPrimitive(); 
MWCNNLayer* convLayer = getLayer(); int numInputs = 
static_cast<int>(convLayer->getNumInputs()); int UpnEytIWGokwbTFkBcSx = 
convLayer->getInPlaceIndex(0); MWTensorBase* opTensorBase = 
convLayer->getOutputTensor(0); MWTensor<float>* opTensor = 
static_cast<MWTensor<float>*>(opTensorBase); if (numInputs == 2 && 
UpnEytIWGokwbTFkBcSx == -1) { MWTensorBase* ipTensor2Base = 
convLayer->getInputTensor(UpnEytIWGokwbTFkBcSx);  MWTensor<float>* ipTensor2 = 
static_cast<MWTensor<float>*>(ipTensor2Base); float* ipData2 = 
(float*)(getReorderedIpMemory()->get_data_handle()); float* opData = 
opTensor->getData(); int additionIpSize = static_cast<int>( 
MWOnednnUtils::getLayerMemoryPrimitive(ipTensor2)->get_desc().get_size()); 
memcpy(opData, ipData2, additionIpSize); } auto s = 
dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); assert(pipeline.size() == 
pipeline_memory_args.size()); for (int i = 0; i < 
static_cast<int>(pipeline.size()); i++) { pipeline[i].execute(s, 
pipeline_memory_args[i]); }
#if MW_CONV_TAP
 reorderedLayerOutputTap(0);
#endif
 return; } MWFusedConvActivationLayerImpl::~MWFusedConvActivationLayerImpl() { 
} void MWFusedConvActivationLayerImpl::cleanup() { if (vIWQzNvYZSuxmOTVDFhU) { 
free(vIWQzNvYZSuxmOTVDFhU); vIWQzNvYZSuxmOTVDFhU = NULL; } if (MdSWZSOAjugbWppryHbR) { 
free(MdSWZSOAjugbWppryHbR); MdSWZSOAjugbWppryHbR = NULL; } } void 
MWFusedConvActivationLayerImpl::loadWeights(const char* RVrPByQXdKmunRZHKWJD) { 
FILE* SDWKEQTZaTFZByPlzUDR = MWCNNLayer::openBinaryFile(RVrPByQXdKmunRZHKWJD); 
MWCNNLayer::call_fread(vIWQzNvYZSuxmOTVDFhU, sizeof(float), fxxCPKTclxXPxrdMAkwi, 
SDWKEQTZaTFZByPlzUDR, RVrPByQXdKmunRZHKWJD); fclose(SDWKEQTZaTFZByPlzUDR); return; } void 
MWFusedConvActivationLayerImpl::loadBias(const char* RVrPByQXdKmunRZHKWJD) { 
float* NDjzAZSYJuWymuKDNZYB = MdSWZSOAjugbWppryHbR; FILE* SDWKEQTZaTFZByPlzUDR = 
MWCNNLayer::openBinaryFile(RVrPByQXdKmunRZHKWJD); 
MWCNNLayer::call_fread(NDjzAZSYJuWymuKDNZYB, sizeof(float), fXhhiexIRPLyKXApPmmy, 
SDWKEQTZaTFZByPlzUDR, RVrPByQXdKmunRZHKWJD); fclose(SDWKEQTZaTFZByPlzUDR); return; } void 
MWFusedConvActivationLayerImpl::setLearnables(std::vector<float*> learnables) { 
assert(learnables.size() == 2); float* xHViLEwTujGGrPZZgmbF = learnables[0]; 
memcpy(vIWQzNvYZSuxmOTVDFhU, xHViLEwTujGGrPZZgmbF, sizeof(float) * 
fxxCPKTclxXPxrdMAkwi); executeWeightsReorderPrimitive(); float* 
NmExSIssnXpisMKKatUq = learnables[1]; memcpy(MdSWZSOAjugbWppryHbR, NmExSIssnXpisMKKatUq, 
sizeof(float) * fXhhiexIRPLyKXApPmmy); } } 