#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTensorBase.hpp"
#else
#include "MWTensorBase.hpp"
#endif
#include "MWOnednnCommonHeaders.hpp"
#include "MWOnednnAvgPoolingLayerImpl.hpp"
#include <memory>
 namespace MWOnednnTarget { 
MWAvgPoolingLayerImpl::MWAvgPoolingLayerImpl(MWCNNLayer* layer, 
MWTargetNetworkImpl* ntwk_impl, int FLuSVNoPhAFKtLUchSvv, int GFienSVKLlDQuZeqAdLC, int 
GsZlHFuhbvjLtRMDjXnW, int HJHXkKmgFxxIOsIvRRnF, int CqtPRJvHlGJFssiPzsOm, int 
CpMjJjtGOeWOzwxpAAQP, int ECTnqgWHyHCHCLBZlffd, int EvebzoroiuKkIxwjkGnD, 
bool aPoolingIncludesPadding) : MWCNNLayerImpl{layer, ntwk_impl} , 
FeVcBgtQmTLtmnNcJGMY{FLuSVNoPhAFKtLUchSvv}  , 
GeeOVBfQrpMacIFBLKOo{GFienSVKLlDQuZeqAdLC}  , 
FLuSVNoPhAFKtLUchSvv{FLuSVNoPhAFKtLUchSvv} , GFienSVKLlDQuZeqAdLC{GFienSVKLlDQuZeqAdLC} , 
GsZlHFuhbvjLtRMDjXnW{GsZlHFuhbvjLtRMDjXnW} , HJHXkKmgFxxIOsIvRRnF{HJHXkKmgFxxIOsIvRRnF} 
, CqtPRJvHlGJFssiPzsOm{CqtPRJvHlGJFssiPzsOm} , 
CpMjJjtGOeWOzwxpAAQP{CpMjJjtGOeWOzwxpAAQP} , 
ECTnqgWHyHCHCLBZlffd{ECTnqgWHyHCHCLBZlffd} , 
EvebzoroiuKkIxwjkGnD{EvebzoroiuKkIxwjkGnD} , 
GnxRkpzrPZimKtYYHSuG{aPoolingIncludesPadding} { } 
MWAvgPoolingLayerImpl::~MWAvgPoolingLayerImpl() { } void 
MWAvgPoolingLayerImpl::propagateSize() { clearLayerMapsAndVectors(); 
MWOnednnUtils::reorderInputsTo(this); MWCNNLayer* avgpoolLayer = getLayer(); 
MWTensorBase* ipTensor = avgpoolLayer->getInputTensor(0); MWTensorBase* 
opTensor = avgpoolLayer->getOutputTensor(0); if ((FeVcBgtQmTLtmnNcJGMY == 
-1) && (GeeOVBfQrpMacIFBLKOo == -1)) { FLuSVNoPhAFKtLUchSvv = 
ipTensor->getHeight() + CqtPRJvHlGJFssiPzsOm + CpMjJjtGOeWOzwxpAAQP; 
GFienSVKLlDQuZeqAdLC = ipTensor->getWidth() + ECTnqgWHyHCHCLBZlffd + 
EvebzoroiuKkIxwjkGnD; } int n = ipTensor->getBatchSize(); int c = 
ipTensor->getChannels(); int h = ipTensor->getHeight(); int w = 
ipTensor->getWidth(); dnnl::memory::dims pool_usr_tz = {n, c, h, w}; 
dnnl::memory::dims pool_dst_tz = {n, c, opTensor->getHeight(), 
opTensor->getWidth()}; dnnl::memory::dims pool_kernel = {FLuSVNoPhAFKtLUchSvv, 
GFienSVKLlDQuZeqAdLC}; dnnl::memory::dims GFggoMvRWucDMqzlWzCl = 
{GsZlHFuhbvjLtRMDjXnW, HJHXkKmgFxxIOsIvRRnF}; dnnl::memory::dims 
FwLnexHgxHRquTKmNpoa = {CqtPRJvHlGJFssiPzsOm, ECTnqgWHyHCHCLBZlffd}; 
dnnl::memory::dims FrpxvsDMwwgbpqHXWxmN = {CpMjJjtGOeWOzwxpAAQP, 
EvebzoroiuKkIxwjkGnD}; auto pool_dst_md = dnnl::memory::desc({pool_dst_tz}, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::any); auto paddingMode 
= GnxRkpzrPZimKtYYHSuG? 
dnnl::algorithm::pooling_avg_include_padding : 
dnnl::algorithm::pooling_avg_exclude_padding; pool_d = 
std::unique_ptr<dnnl::pooling_forward::desc>(new dnnl::pooling_forward::desc( 
dnnl::prop_kind::forward_inference, paddingMode, 
getReorderedIpMemory()->get_desc(), pool_dst_md, GFggoMvRWucDMqzlWzCl, 
pool_kernel, FwLnexHgxHRquTKmNpoa, FrpxvsDMwwgbpqHXWxmN)); pool_pd = 
std::unique_ptr<dnnl::pooling_forward::primitive_desc>( new 
dnnl::pooling_forward::primitive_desc(*pool_d, *eFaDPmxDdzHlRYSAoMmX->eng)); 
setLayerMemory(std::make_shared<dnnl::memory>(pool_pd->dst_desc(), 
*eFaDPmxDdzHlRYSAoMmX->eng)); pool = std::unique_ptr<dnnl::pooling_forward>(new 
dnnl::pooling_forward(*pool_pd)); pipeline.push_back(*pool); 
pipeline_memory_args.push_back( {{DNNL_ARG_FROM, *getReorderedIpMemory()}, 
{DNNL_ARG_TO, *getLayerMemory()}}); } void MWAvgPoolingLayerImpl::predict() { 
executeInputsReorderPrimitive(); auto s = 
dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); assert(pipeline.size() == 
pipeline_memory_args.size()); for (int i = 0; i < 
static_cast<int>(pipeline.size()); i++) { pipeline[i].execute(s, 
pipeline_memory_args[i]); }
#if MW_POOL_TAP
 reorderedLayerOutputTap(0);
#endif
 return; } } 