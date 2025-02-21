#ifdef PRECOMPILE_LAYERFILES
#include "layer/MWTensorBase.hpp"
#else
#include "MWTensorBase.hpp"
#endif
#include "MWOnednnCommonHeaders.hpp"
#include "MWOnednnFCLayerImpl.hpp"
#include <memory>
#include <vector>
 namespace MWOnednnTarget { MWFCLayerImpl::~MWFCLayerImpl() { } 
MWFCLayerImpl::MWFCLayerImpl(MWCNNLayer* layer, MWTargetNetworkImpl* ntwk_impl, 
int XCLDbxHBtWRStETWIkId, int muwRQxtWMMXAPxSuMYBw, const char* 
yCdIUfwoZFngCRRRkCTg, const char* NtWaRGCHLeTapjWdEHHS) : 
MWCNNLayerImpl(layer, ntwk_impl) { vIWQzNvYZSuxmOTVDFhU = 
(float*)calloc(XCLDbxHBtWRStETWIkId * muwRQxtWMMXAPxSuMYBw, sizeof(float)); 
MdSWZSOAjugbWppryHbR = (float*)calloc(muwRQxtWMMXAPxSuMYBw, sizeof(float)); 
fxxCPKTclxXPxrdMAkwi = XCLDbxHBtWRStETWIkId * muwRQxtWMMXAPxSuMYBw; 
loadWeights(yCdIUfwoZFngCRRRkCTg); fXhhiexIRPLyKXApPmmy = 
muwRQxtWMMXAPxSuMYBw; loadBias(NtWaRGCHLeTapjWdEHHS); dnnl::memory::dims 
fc_bias_tz = {muwRQxtWMMXAPxSuMYBw}; bias_md = 
std::make_shared<dnnl::memory::desc>(fc_bias_tz, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::x); bias = std::make_shared<dnnl::memory>(*bias_md, 
*eFaDPmxDdzHlRYSAoMmX->eng, MdSWZSOAjugbWppryHbR); } void MWFCLayerImpl::propagateSize() 
{ clearLayerMapsAndVectors(); MWCNNLayer* fcLayer = getLayer(); MWTensorBase* 
ipTensor = fcLayer->getInputTensor(); MWTensorBase* opTensor = 
fcLayer->getOutputTensor(); dnnl::memory::dims fc_src_tz = 
{ipTensor->getBatchSize() * ipTensor->getSequenceLength(), 
ipTensor->getChannels(), ipTensor->getHeight(), ipTensor->getWidth()}; 
dnnl::memory::dims dim1 = {opTensor->getChannels(), ipTensor->getChannels()}; 
dnnl::memory::dims dim2 = {opTensor->getChannels(), ipTensor->getChannels(), 
ipTensor->getHeight(), ipTensor->getWidth()}; auto prevMemoryDesc = 
MWOnednnUtils::getLayerMemoryPrimitive(ipTensor)->get_desc(); format_type 
formatTag = MWOnednnUtils::getformatType(prevMemoryDesc, ipTensor); bool 
isSequenceIp = (formatTag == format_type::TNC_FORMAT); bool dimNCHWFlag = 
((ipTensor->getHeight() != 1) && (ipTensor->getWidth() != 1)) || 
!(isSequenceIp); fc_src_memory = 
MWOnednnUtils::getLayerMemoryPrimitive(ipTensor); 
std::shared_ptr<dnnl::memory::desc> fc_src_md; if (isSequenceIp) { 
MWOnednnUtils::reorderInputsTo(this, format_type::TNC_FORMAT); fc_src_tz = 
{ipTensor->getBatchSize() * ipTensor->getSequenceLength(), 
ipTensor->getChannels()}; fc_src_md = 
std::make_shared<dnnl::memory::desc>(fc_src_tz, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::nc); fc_src_memory = getReorderedIpMemory(); } else { 
fc_src_md = std::make_shared<dnnl::memory::desc>(fc_src_tz, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::any); } 
dnnl::memory::dims fc_weights_tz = (dimNCHWFlag) ? dim2 : dim1; if 
(dimNCHWFlag) { weights_md = std::make_shared<dnnl::memory::desc>( 
fc_weights_tz, dnnl::memory::data_type::f32, dnnl::memory::format_tag::oihw); } 
else { weights_md = std::make_shared<dnnl::memory::desc>( fc_weights_tz, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::nc); } auto 
fc_weights_md = dnnl::memory::desc(fc_weights_tz, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::any); prepareWeights(); weights = 
std::make_shared<dnnl::memory>(*weights_md, *eFaDPmxDdzHlRYSAoMmX->eng, 
vIWQzNvYZSuxmOTVDFhU); auto fc_bias_md = dnnl::memory::desc({opTensor->getChannels()}, 
dnnl::memory::data_type::f32, dnnl::memory::format_tag::any); auto fc_dst_md = 
dnnl::memory::desc( {ipTensor->getSequenceLength() * ipTensor->getBatchSize(), 
opTensor->getChannels()}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::any); ip_d = 
std::unique_ptr<dnnl::inner_product_forward::desc>(new 
dnnl::inner_product_forward::desc( dnnl::prop_kind::forward, *fc_src_md, 
fc_weights_md, fc_bias_md, fc_dst_md)); ip_pd = 
std::unique_ptr<dnnl::inner_product_forward::primitive_desc>( new 
dnnl::inner_product_forward::primitive_desc(*ip_d, *eFaDPmxDdzHlRYSAoMmX->eng)); 
if (dimNCHWFlag) { if (dnnl::memory::desc(ip_pd->src_desc()) != 
fc_src_memory->get_desc()) { fc_src_memory = 
std::make_shared<dnnl::memory>(ip_pd->src_desc(), *eFaDPmxDdzHlRYSAoMmX->eng); 
fc_reorder_src = std::unique_ptr<dnnl::reorder>(new dnnl::reorder( 
*MWOnednnUtils::getLayerMemoryPrimitive(ipTensor), *fc_src_memory)); 
pipeline_memory_args.push_back( {{DNNL_ARG_FROM, 
*MWOnednnUtils::getLayerMemoryPrimitive(ipTensor)}, {DNNL_ARG_TO, 
*fc_src_memory}}); pipeline.push_back(*fc_reorder_src); } } fc_weights_memory = 
weights; if (dnnl::memory::desc(ip_pd->weights_desc()) != 
fc_weights_memory->get_desc()) { fc_weights_memory = 
std::make_shared<dnnl::memory>(ip_pd->weights_desc(), 
*eFaDPmxDdzHlRYSAoMmX->eng); fc_reorder_weights = 
std::unique_ptr<dnnl::reorder>(new dnnl::reorder(*weights, 
*fc_weights_memory)); pipeline_weights.push_back(*fc_reorder_weights); 
pipeline_weights_memory_args.push_back( {{DNNL_ARG_FROM, *weights}, 
{DNNL_ARG_TO, *fc_weights_memory}}); executeWeightsReorderPrimitive(); } 
dnnl::memory::desc layerOpdesc; if (isSequenceIp) { layerOpdesc = 
dnnl::memory::desc( {ipTensor->getSequenceLength(), ipTensor->getBatchSize(), 
opTensor->getChannels()}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::tnc); } else { layerOpdesc = 
dnnl::memory::desc({ipTensor->getBatchSize(), opTensor->getChannels(), 
opTensor->getHeight(), opTensor->getWidth()}, dnnl::memory::data_type::f32, 
dnnl::memory::format_tag::nchw); } assert(opTensor->getHeight() == 1 && 
opTensor->getWidth() == 1); 
setLayerMemory(std::make_shared<dnnl::memory>(layerOpdesc, 
*eFaDPmxDdzHlRYSAoMmX->eng)); ip = 
std::unique_ptr<dnnl::inner_product_forward>(new 
dnnl::inner_product_forward(*ip_pd)); 
pipeline_memory_args.push_back({{DNNL_ARG_SRC, *fc_src_memory}, 
{DNNL_ARG_WEIGHTS, *fc_weights_memory}, {DNNL_ARG_BIAS, *bias}, {DNNL_ARG_DST, 
*getLayerMemory()}}); pipeline.push_back(*ip); return; } void 
MWFCLayerImpl::predict() { executeInputsReorderPrimitive(); auto s = 
dnnl::stream(*eFaDPmxDdzHlRYSAoMmX->eng); assert(pipeline.size() == 
pipeline_memory_args.size()); for (int i = 0; i < 
static_cast<int>(pipeline.size()); i++) { pipeline[i].execute(s, 
pipeline_memory_args[i]); }
#if MW_FC_TAP
 reorderedLayerOutputTap(0);
#endif
 return; } void MWFCLayerImpl::loadWeights(const char* RVrPByQXdKmunRZHKWJD) { 
FILE* SDWKEQTZaTFZByPlzUDR = MWCNNLayer::openBinaryFile(RVrPByQXdKmunRZHKWJD); 
MWCNNLayer::call_fread(vIWQzNvYZSuxmOTVDFhU, sizeof(float), fxxCPKTclxXPxrdMAkwi, 
SDWKEQTZaTFZByPlzUDR, RVrPByQXdKmunRZHKWJD); fclose(SDWKEQTZaTFZByPlzUDR); return; } void 
MWFCLayerImpl::prepareWeights() { MWTensorBase* ipTensor = 
getLayer()->getInputTensor(); MWTensorBase* opTensor = 
getLayer()->getOutputTensor(); int AzTsxYcYjIEJsGQbeYHm = 
ipTensor->getHeight(); int BLjrjqvCcCommiXWQLjs = ipTensor->getWidth(); int 
CZNYmBcNFSZWvaCklqeM = AzTsxYcYjIEJsGQbeYHm * BLjrjqvCcCommiXWQLjs 
* ipTensor->getChannels(); int eVAFqeShtGZAZluKdMvQ = CZNYmBcNFSZWvaCklqeM * 
opTensor->getChannels(); if (AzTsxYcYjIEJsGQbeYHm != 1 && 
BLjrjqvCcCommiXWQLjs != 1) { float* veFyKKHbdqBIvQLYBqfF = 
(float*)malloc(sizeof(float) * AzTsxYcYjIEJsGQbeYHm * 
BLjrjqvCcCommiXWQLjs); for (int k = 0; k < eVAFqeShtGZAZluKdMvQ / 
AzTsxYcYjIEJsGQbeYHm / BLjrjqvCcCommiXWQLjs; k++) { for (int i = 0; i < 
AzTsxYcYjIEJsGQbeYHm * BLjrjqvCcCommiXWQLjs; i++) { veFyKKHbdqBIvQLYBqfF[i] = 
vIWQzNvYZSuxmOTVDFhU[k * AzTsxYcYjIEJsGQbeYHm * BLjrjqvCcCommiXWQLjs + i]; } for 
(int j = 0; j < AzTsxYcYjIEJsGQbeYHm; j++) for (int i = 0; i < 
BLjrjqvCcCommiXWQLjs; i++) { vIWQzNvYZSuxmOTVDFhU[k * AzTsxYcYjIEJsGQbeYHm * 
BLjrjqvCcCommiXWQLjs + j * BLjrjqvCcCommiXWQLjs + i] = veFyKKHbdqBIvQLYBqfF[j + i 
* AzTsxYcYjIEJsGQbeYHm]; } } free(veFyKKHbdqBIvQLYBqfF); } return; } void 
MWFCLayerImpl::loadBias(const char* RVrPByQXdKmunRZHKWJD) { FILE* SDWKEQTZaTFZByPlzUDR 
= MWCNNLayer::openBinaryFile(RVrPByQXdKmunRZHKWJD); 
MWCNNLayer::call_fread(MdSWZSOAjugbWppryHbR, sizeof(float), fXhhiexIRPLyKXApPmmy, 
SDWKEQTZaTFZByPlzUDR, RVrPByQXdKmunRZHKWJD); fclose(SDWKEQTZaTFZByPlzUDR); return; } void 
MWFCLayerImpl::cleanup() { if (vIWQzNvYZSuxmOTVDFhU) { free(vIWQzNvYZSuxmOTVDFhU); vIWQzNvYZSuxmOTVDFhU 
= NULL; } if (MdSWZSOAjugbWppryHbR) { free(MdSWZSOAjugbWppryHbR); MdSWZSOAjugbWppryHbR = NULL; } 
return; } void MWFCLayerImpl::setLearnables(std::vector<float*> learnables) { 
assert(learnables.size() == 2); float* xHViLEwTujGGrPZZgmbF = learnables[0]; 
memcpy(vIWQzNvYZSuxmOTVDFhU, xHViLEwTujGGrPZZgmbF, sizeof(float) * 
fxxCPKTclxXPxrdMAkwi); prepareWeights(); executeWeightsReorderPrimitive(); 
float* NmExSIssnXpisMKKatUq = learnables[1]; memcpy(MdSWZSOAjugbWppryHbR, NmExSIssnXpisMKKatUq, 
sizeof(float) * fXhhiexIRPLyKXApPmmy); } } 