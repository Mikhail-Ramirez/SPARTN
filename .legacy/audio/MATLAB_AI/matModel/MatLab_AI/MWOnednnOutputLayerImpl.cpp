#include "MWOnednnCommonHeaders.hpp"
#include "MWOnednnOutputLayerImpl.hpp"
#include <memory>
 class MWTensorBase; namespace MWOnednnTarget { 
MWOutputLayerImpl::MWOutputLayerImpl(MWCNNLayer* layer, MWTargetNetworkImpl* 
ntwk_impl) : MWCNNLayerImpl(layer, ntwk_impl) { } 
MWOutputLayerImpl::~MWOutputLayerImpl() { } void 
MWOutputLayerImpl::propagateSize() { clearLayerMapsAndVectors(); MWTensorBase* 
ipTensor = getLayer()->getInputTensor(0); auto prevMemoryDesc = 
MWOnednnUtils::getLayerMemoryPrimitive(ipTensor)->get_desc(); format_type 
formatTag = MWOnednnUtils::getformatType(prevMemoryDesc, ipTensor); bool 
isSequenceIp = (formatTag == format_type::TNC_FORMAT); format_type reorderIpTo 
= isSequenceIp ? (format_type::TNC_FORMAT) : (format_type::NCHW_FORMAT); 
MWOnednnUtils::reorderInputsTo(this, reorderIpTo); 
setLayerMemory(std::make_shared<dnnl::memory>(getReorderedIpMemory()->get_desc(), 
*eFaDPmxDdzHlRYSAoMmX->eng)); } void MWOutputLayerImpl::predict() { 
assert(getLayer()->getInPlaceIndex(0) != -1); executeInputsReorderPrimitive(); 
} } 