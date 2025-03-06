#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTensorBase.hpp"
#else
#include "MWTensorBase.hpp"
#endif
#include "MWOnednnCommonHeaders.hpp"
#include "MWOnednnMaxPoolingLayerImpl.hpp"
#include <memory>
 namespace MWOnednnTarget { 
MWMaxPoolingLayerImpl::MWMaxPoolingLayerImpl(MWCNNLayer* layer, 
MWTargetNetworkImpl* ntwk_impl, int FLuSVNoPhAFKtLUchSvv, int GFienSVKLlDQuZeqAdLC, int 
GsZlHFuhbvjLtRMDjXnW, int HJHXkKmgFxxIOsIvRRnF, int CqtPRJvHlGJFssiPzsOm, int 
CpMjJjtGOeWOzwxpAAQP, int ECTnqgWHyHCHCLBZlffd, int EvebzoroiuKkIxwjkGnD, 
bool IbSWJNMuIiKbocfQKqXb, int fSKMHAqIghbYYgyIpNDw) : MWCNNLayerImpl{layer, 
ntwk_impl} , BRSPqxNffoBYKqpSVHne{IbSWJNMuIiKbocfQKqXb} , 
FeVcBgtQmTLtmnNcJGMY{FLuSVNoPhAFKtLUchSvv}  , 
GeeOVBfQrpMacIFBLKOo{GFienSVKLlDQuZeqAdLC}  , 
FLuSVNoPhAFKtLUchSvv{FLuSVNoPhAFKtLUchSvv} , GFienSVKLlDQuZeqAdLC{GFienSVKLlDQuZeqAdLC} , 
GsZlHFuhbvjLtRMDjXnW{GsZlHFuhbvjLtRMDjXnW} , HJHXkKmgFxxIOsIvRRnF{HJHXkKmgFxxIOsIvRRnF} 
, CqtPRJvHlGJFssiPzsOm{CqtPRJvHlGJFssiPzsOm} , 
CpMjJjtGOeWOzwxpAAQP{CpMjJjtGOeWOzwxpAAQP} , 
ECTnqgWHyHCHCLBZlffd{ECTnqgWHyHCHCLBZlffd} , 
EvebzoroiuKkIxwjkGnD{EvebzoroiuKkIxwjkGnD} , 
fSKMHAqIghbYYgyIpNDw{fSKMHAqIghbYYgyIpNDw} { } 
MWMaxPoolingLayerImpl::~MWMaxPoolingLayerImpl() { } float* 
MWMaxPoolingLayerImpl::getIndexData() { return UzaGmBLFEwmwaFXebUma; } 
std::shared_ptr<dnnl::pooling_forward::primitive_desc> 
MWMaxPoolingLayerImpl::getPoolPrimitiveDesc() { return pool_pd; } 
dnnl::memory::dims MWMaxPoolingLayerImpl::getPoolKernel() { return 
FpguQZSermqZCMRiUfML; } dnnl::memory::dims 
MWMaxPoolingLayerImpl::getPoolStrides() { return GFggoMvRWucDMqzlWzCl; } 
dnnl::memory::dims MWMaxPoolingLayerImpl::getPoolPadTL() { return 
FwLnexHgxHRquTKmNpoa; } dnnl::memory::dims 
MWMaxPoolingLayerImpl::getPoolPadBR() { return FrpxvsDMwwgbpqHXWxmN; } void 
createMaxpoolingIndicesPrimitive( dnnl::memory::desc pool_dst_md, 
std::shared_ptr<dnnl::pooling_forward::desc> pool_d, 
std::shared_ptr<dnnl::pooling_forward::primitive_desc>& pool_pd, 
std::shared_ptr<dnnl::memory> srcMemory, std::shared_ptr<dnnl::memory>& 
layerMemory, std::shared_ptr<dnnl::memory>& indices_Memory, 
std::shared_ptr<dnnl::pooling_forward::primitive>& pool, dnnl::memory::dims 
FpguQZSermqZCMRiUfML, dnnl::memory::dims GFggoMvRWucDMqzlWzCl, 
dnnl::memory::dims FwLnexHgxHRquTKmNpoa, dnnl::memory::dims 
FrpxvsDMwwgbpqHXWxmN) { pool_d = std::make_shared<dnnl::pooling_forward::desc>( 
dnnl::prop_kind::forward_training, dnnl::algorithm::pooling_max, 
srcMemory->get_desc(), pool_dst_md, GFggoMvRWucDMqzlWzCl, FpguQZSermqZCMRiUfML, 
FwLnexHgxHRquTKmNpoa, FrpxvsDMwwgbpqHXWxmN); pool_pd = 
std::make_shared<dnnl::pooling_forward::primitive_desc>(*pool_d, 
srcMemory->get_engine()); layerMemory = 
std::make_shared<dnnl::memory>(pool_pd->dst_desc(), srcMemory->get_engine()); 
indices_Memory = std::make_shared<dnnl::memory>(pool_pd->workspace_desc(), 
srcMemory->get_engine()); pool = 
std::make_shared<dnnl::pooling_forward>(*pool_pd); } void 
createMaxpoolingPrimitive(dnnl::memory::desc pool_dst_md, 
std::shared_ptr<dnnl::pooling_forward::desc> pool_d, 
std::shared_ptr<dnnl::pooling_forward::primitive_desc>& pool_pd, 
std::shared_ptr<dnnl::memory> srcMemory, std::shared_ptr<dnnl::memory>& 
layerMemory, std::shared_ptr<dnnl::pooling_forward::primitive>& pool, 
dnnl::memory::dims FpguQZSermqZCMRiUfML, dnnl::memory::dims 
GFggoMvRWucDMqzlWzCl, dnnl::memory::dims FwLnexHgxHRquTKmNpoa, 
dnnl::memory::dims FrpxvsDMwwgbpqHXWxmN) { pool_d = 
std::make_shared<dnnl::pooling_forward::desc>( 
dnnl::prop_kind::forward_inference, dnnl::algorithm::pooling_max, 
srcMemory->get_desc(), pool_dst_md, GFggoMvRWucDMqzlWzCl, FpguQZSermqZCMRiUfML, 
FwLnexHgxHRquTKmNpoa, FrpxvsDMwwgbpqHXWxmN); pool_pd = 
std::make_shared<dnnl::pooling_forward::primitive_desc>(*pool_d, 
srcMemory->get_engine()); layerMemory = 
std::make_shared<dnnl::memory>(pool_pd->dst_desc(), srcMemory->get_engine()); 
pool = std::make_shared<dnnl::pooling_forward>(*pool_pd); } void 
MWMaxPoolingLayerImpl::propagateSize() { clearLayerMapsAndVectors(); 
MWCNNLayer* maxPoolLayer = getLayer(); MWTensorBase* ipTensor = 
maxPoolLayer->getInputTensor(); MWTensorBase* opTensor = 
maxPoolLayer->getOutputTensor(); 
assert(MWOnednnUtils::getformatType(MWOnednnUtils::getLayerMemoryPrimitive(ipTensor)->get_desc(), 
ipTensor) != format_type::TNC_FORMAT); MWOnednnUtils::reorderInputsTo(this); 
int n = ipTensor->getBatchSize(); int c = ipTensor->getChannels(); int h = 
ipTensor->getHeight(); int w = ipTensor->getWidth(); dnnl::memory::dims 
pool_usr_tz = {n, c, h, w}; dnnl::memory::dims pool_dst_tz = {n, c, 
opTensor->getHeight(), opTensor->getWidth()}; if ((FeVcBgtQmTLtmnNcJGMY == 
-1) && (GeeOVBfQrpMacIFBLKOo == -1)) { FLuSVNoPhAFKtLUchSvv = 
ipTensor->getHeight(); GFienSVKLlDQuZeqAdLC = ipTensor->getWidth(); } 
FpguQZSermqZCMRiUfML = {FLuSVNoPhAFKtLUchSvv, GFienSVKLlDQuZeqAdLC}; 
GFggoMvRWucDMqzlWzCl = {GsZlHFuhbvjLtRMDjXnW, HJHXkKmgFxxIOsIvRRnF}; 
FwLnexHgxHRquTKmNpoa = {CqtPRJvHlGJFssiPzsOm, ECTnqgWHyHCHCLBZlffd}; 
FrpxvsDMwwgbpqHXWxmN = {CpMjJjtGOeWOzwxpAAQP, EvebzoroiuKkIxwjkGnD}; auto 
pool_dst_md = dnnl::memory::desc({pool_dst_tz}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::any); if (BRSPqxNffoBYKqpSVHne) { 
createMaxpoolingIndicesPrimitive(pool_dst_md, pool_d, pool_pd, 
getReorderedIpMemory(), layerMemory[0], layerMemory[1], pool, 
FpguQZSermqZCMRiUfML, GFggoMvRWucDMqzlWzCl, FwLnexHgxHRquTKmNpoa, 
FrpxvsDMwwgbpqHXWxmN); eybNKlJCSDUvsznWynwK = 
static_cast<int>(getLayerMemory(1)->get_desc().get_size() / sizeof(float)); 
pipeline_memory_args.push_back({{DNNL_ARG_FROM, *getReorderedIpMemory()}, 
{DNNL_ARG_DST, *layerMemory[0]}, {DNNL_ARG_WORKSPACE, *layerMemory[1]}}); } 
else { createMaxpoolingPrimitive(pool_dst_md, pool_d, pool_pd, 
getReorderedIpMemory(), layerMemory[0], pool, FpguQZSermqZCMRiUfML, 
GFggoMvRWucDMqzlWzCl, FwLnexHgxHRquTKmNpoa, FrpxvsDMwwgbpqHXWxmN); 
pipeline_memory_args.push_back( {{DNNL_ARG_FROM, *getReorderedIpMemory()}, 
{DNNL_ARG_DST, *layerMemory[0]}}); } pipeline.push_back(*pool); return; } void 
MWMaxPoolingLayerImpl::predict() { executeInputsReorderPrimitive(); auto s = 
dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); assert(pipeline.size() == 
pipeline_memory_args.size()); for (int i = 0; i < 
static_cast<int>(pipeline.size()); i++) { pipeline[i].execute(s, 
pipeline_memory_args[i]); }
#if MW_POOL_TAP
 reorderedLayerOutputTap(0);
#endif
 return; } } 