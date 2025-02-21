//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// dlnetwork.cpp
//
// Code generation for function 'dlnetwork'
//

// Include files
#include "dlnetwork.h"
#include "matModel_internal_types.h"
#include "MWAvgPoolingLayer.hpp"
#include "MWBatchNormalizationLayer.hpp"
#include "MWCNNLayer.hpp"
#include "MWElementwiseAffineLayer.hpp"
#include "MWFCLayer.hpp"
#include "MWFusedConvActivationLayer.hpp"
#include "MWInputLayer.hpp"
#include "MWMaxPoolingLayer.hpp"
#include "MWOnednnTargetNetworkImpl.hpp"
#include "MWOutputLayer.hpp"
#include "MWSoftmaxLayer.hpp"
#include "MWTensor.hpp"
#include "MWTensorBase.hpp"
#include "stdio.h"
#include <cstdlib>

// Named Constants
const char *errStringBase{
    "Error during execution of the generated code. %s at line: %d, file: "
    "%s\nExiting program execution ...\n"};

// Function Declarations
static void checkRunTimeError(const char *errMsg, const char *file,
                              unsigned int b_line);

// Function Definitions
void binDroneModel0_0::allocate()
{
  targetImpl->allocate(2, layers, numLayers, 0);
  for (int idx{0}; idx < 18; idx++) {
    layers[idx]->allocate();
  }
  (static_cast<MWTensor<float> *>(inputTensors[0]))
      ->setData(layers[0]->getLayerOutput(0));
}

void binDroneModel0_0::cleanup()
{
  deallocate();
  for (int idx{0}; idx < 18; idx++) {
    layers[idx]->cleanup();
  }
  if (targetImpl) {
    targetImpl->cleanup();
  }
  isInitialized = false;
}

void binDroneModel0_0::deallocate()
{
  targetImpl->deallocate();
  for (int idx{0}; idx < 18; idx++) {
    layers[idx]->deallocate();
  }
}

void binDroneModel0_0::postsetup()
{
  targetImpl->postSetup();
}

void binDroneModel0_0::resetState()
{
}

void binDroneModel0_0::setSize()
{
  for (int idx{0}; idx < 18; idx++) {
    layers[idx]->propagateSize();
  }
  allocate();
  postsetup();
}

void binDroneModel0_0::setup()
{
  if (isInitialized) {
    resetState();
  } else {
    targetImpl->preSetup();
    (static_cast<MWInputLayer *>(layers[0]))
        ->createInputLayer(targetImpl, inputTensors[0], "SSC", 0);
    (static_cast<MWElementwiseAffineLayer *>(layers[1]))
        ->createElementwiseAffineLayer(
            targetImpl, layers[0]->getOutputTensor(0), 1, 1, 2, 1, 1, 2, false,
            1, 1,
            "./codegen/lib/matModel/cnn_binDroneModel0_0_imageinput_scale.bin",
            "./codegen/lib/matModel/cnn_binDroneModel0_0_imageinput_offset.bin",
            "SSC", -1);
    (static_cast<MWBatchNormalizationLayer *>(layers[2]))
        ->createBatchNormalizationLayer(
            targetImpl, layers[1]->getOutputTensor(0), 1.0E-5F,
            "./codegen/lib/matModel/"
            "cnn_binDroneModel0_0_batchnorm_1_offset.bin",
            "./codegen/lib/matModel/cnn_binDroneModel0_0_batchnorm_1_scale.bin",
            "./codegen/lib/matModel/"
            "cnn_binDroneModel0_0_batchnorm_1_trainedMean.bin",
            "./codegen/lib/matModel/"
            "cnn_binDroneModel0_0_batchnorm_1_trainedVariance.bin",
            2, "SSC", -1);
    (static_cast<MWFusedConvActivationLayer *>(layers[3]))
        ->createFusedConvActivationLayer(
            targetImpl, 1, layers[2]->getOutputTensor(0), 3, 3, 2, 32, 1, 1, 1,
            1, 1, 1, 1, 1, 1,
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_1_w.bin",
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_1_b.bin", 0.0F,
            MWActivationFunctionType::ACTIVATION_FCN_ENUM::RELU, "SSC", 1);
    (static_cast<MWFusedConvActivationLayer *>(layers[4]))
        ->createFusedConvActivationLayer(
            targetImpl, 1, layers[3]->getOutputTensor(0), 3, 3, 32, 32, 1, 1, 1,
            1, 1, 1, 1, 1, 1,
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_2_w.bin",
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_2_b.bin", 0.0F,
            MWActivationFunctionType::ACTIVATION_FCN_ENUM::RELU, "SSC", 0);
    (static_cast<MWMaxPoolingLayer *>(layers[5]))
        ->createMaxPoolingLayer<float, float>(
            targetImpl, layers[4]->getOutputTensor(0), 3, 3, 2, 2, 0, 1, 1, 1,
            0, 0, "FLOAT", 1, "SSC", 1);
    (static_cast<MWFusedConvActivationLayer *>(layers[6]))
        ->createFusedConvActivationLayer(
            targetImpl, 1, layers[5]->getOutputTensor(0), 3, 3, 32, 64, 1, 1, 1,
            1, 1, 1, 1, 1, 1,
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_3_w.bin",
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_3_b.bin", 0.0F,
            MWActivationFunctionType::ACTIVATION_FCN_ENUM::RELU, "SSC", 0);
    (static_cast<MWFusedConvActivationLayer *>(layers[7]))
        ->createFusedConvActivationLayer(
            targetImpl, 1, layers[6]->getOutputTensor(0), 3, 3, 64, 64, 1, 1, 1,
            1, 1, 1, 1, 1, 1,
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_4_w.bin",
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_4_b.bin", 0.0F,
            MWActivationFunctionType::ACTIVATION_FCN_ENUM::RELU, "SSC", 1);
    (static_cast<MWMaxPoolingLayer *>(layers[8]))
        ->createMaxPoolingLayer<float, float>(
            targetImpl, layers[7]->getOutputTensor(0), 3, 3, 2, 2, 0, 1, 1, 1,
            0, 0, "FLOAT", 1, "SSC", 0);
    (static_cast<MWFusedConvActivationLayer *>(layers[9]))
        ->createFusedConvActivationLayer(
            targetImpl, 1, layers[8]->getOutputTensor(0), 3, 3, 64, 128, 1, 1,
            1, 1, 1, 1, 1, 1, 1,
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_5_w.bin",
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_5_b.bin", 0.0F,
            MWActivationFunctionType::ACTIVATION_FCN_ENUM::RELU, "SSC", 1);
    (static_cast<MWFusedConvActivationLayer *>(layers[10]))
        ->createFusedConvActivationLayer(
            targetImpl, 1, layers[9]->getOutputTensor(0), 3, 3, 128, 128, 1, 1,
            1, 1, 1, 1, 1, 1, 1,
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_6_w.bin",
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_6_b.bin", 0.0F,
            MWActivationFunctionType::ACTIVATION_FCN_ENUM::RELU, "SSC", 0);
    (static_cast<MWMaxPoolingLayer *>(layers[11]))
        ->createMaxPoolingLayer<float, float>(
            targetImpl, layers[10]->getOutputTensor(0), 3, 3, 2, 2, 0, 1, 0, 1,
            0, 0, "FLOAT", 1, "SSC", 1);
    (static_cast<MWFusedConvActivationLayer *>(layers[12]))
        ->createFusedConvActivationLayer(
            targetImpl, 1, layers[11]->getOutputTensor(0), 3, 3, 128, 256, 1, 1,
            1, 1, 1, 1, 1, 1, 1,
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_7_w.bin",
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_7_b.bin", 0.0F,
            MWActivationFunctionType::ACTIVATION_FCN_ENUM::RELU, "SSC", 0);
    (static_cast<MWFusedConvActivationLayer *>(layers[13]))
        ->createFusedConvActivationLayer(
            targetImpl, 1, layers[12]->getOutputTensor(0), 3, 3, 256, 256, 1, 1,
            1, 1, 1, 1, 1, 1, 1,
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_8_w.bin",
            "./codegen/lib/matModel/cnn_binDroneModel0_0_conv_8_b.bin", 0.0F,
            MWActivationFunctionType::ACTIVATION_FCN_ENUM::RELU, "SSC", 1);
    (static_cast<MWAvgPoolingLayer *>(layers[14]))
        ->createAvgPoolingLayer<float, float>(
            targetImpl, layers[13]->getOutputTensor(0), -1, -1, 1, 1, 0, 0, 0,
            0, 1, 0, "FLOAT", 1, "SSC", 0);
    (static_cast<MWFCLayer *>(layers[15]))
        ->createFCLayer(targetImpl, layers[14]->getOutputTensor(0), 256, 2,
                        "./codegen/lib/matModel/cnn_binDroneModel0_0_fc_w.bin",
                        "./codegen/lib/matModel/cnn_binDroneModel0_0_fc_b.bin",
                        "CB", 1);
    (static_cast<MWSoftmaxLayer *>(layers[16]))
        ->createSoftmaxLayer(targetImpl, layers[15]->getOutputTensor(0), "CB",
                             0);
    (static_cast<MWOutputLayer *>(layers[17]))
        ->createOutputLayer(targetImpl, layers[16]->getOutputTensor(0), "CB",
                            -1);
    outputTensors[0] = layers[17]->getOutputTensor(0);
    setSize();
  }
  isInitialized = true;
}

static void checkRunTimeError(const char *errMsg, const char *file,
                              unsigned int b_line)
{
  printf(errStringBase, errMsg, b_line, file);
  exit(EXIT_FAILURE);
}

void binDroneModel0_0::activations(int layerIdx)
{
  for (int idx{0}; idx <= layerIdx; idx++) {
    layers[idx]->predict();
  }
}

binDroneModel0_0::binDroneModel0_0()
{
  numLayers = 18;
  isInitialized = false;
  targetImpl = 0;
  layers[0] = new MWInputLayer;
  layers[0]->setName("imageinput");
  layers[1] = new MWElementwiseAffineLayer;
  layers[1]->setName("imageinput_normalization");
  layers[1]->setInPlaceIndex(0, 0);
  layers[2] = new MWBatchNormalizationLayer;
  layers[2]->setName("batchnorm_1");
  layers[2]->setInPlaceIndex(0, 0);
  layers[3] = new MWFusedConvActivationLayer;
  layers[3]->setName("conv_1_relu_1");
  layers[4] = new MWFusedConvActivationLayer;
  layers[4]->setName("conv_2_relu_2");
  layers[5] = new MWMaxPoolingLayer;
  layers[5]->setName("maxpool_1");
  layers[6] = new MWFusedConvActivationLayer;
  layers[6]->setName("conv_3_relu_3");
  layers[7] = new MWFusedConvActivationLayer;
  layers[7]->setName("conv_4_relu_4");
  layers[8] = new MWMaxPoolingLayer;
  layers[8]->setName("maxpool_2");
  layers[9] = new MWFusedConvActivationLayer;
  layers[9]->setName("conv_5_relu_5");
  layers[10] = new MWFusedConvActivationLayer;
  layers[10]->setName("conv_6_relu_6");
  layers[11] = new MWMaxPoolingLayer;
  layers[11]->setName("maxpool_3");
  layers[12] = new MWFusedConvActivationLayer;
  layers[12]->setName("conv_7_relu_7");
  layers[13] = new MWFusedConvActivationLayer;
  layers[13]->setName("conv_8_relu_8");
  layers[14] = new MWAvgPoolingLayer;
  layers[14]->setName("gap");
  layers[15] = new MWFCLayer;
  layers[15]->setName("fc");
  layers[16] = new MWSoftmaxLayer;
  layers[16]->setName("softmax");
  layers[17] = new MWOutputLayer;
  layers[17]->setName("output_softmax");
  layers[17]->setInPlaceIndex(0, 0);
  targetImpl = new MWOnednnTarget::MWTargetNetworkImpl;
  inputTensors[0] = new MWTensor<float>;
  inputTensors[0]->setHeight(128);
  inputTensors[0]->setWidth(85);
  inputTensors[0]->setChannels(2);
  inputTensors[0]->setBatchSize(1);
  inputTensors[0]->setSequenceLength(1);
}

binDroneModel0_0::~binDroneModel0_0()
{
  try {
    if (isInitialized) {
      cleanup();
    }
    for (int idx{0}; idx < 18; idx++) {
      delete layers[idx];
    }
    if (targetImpl) {
      delete targetImpl;
    }
    delete inputTensors[0];
  } catch (...) {
  }
}

int binDroneModel0_0::getBatchSize()
{
  return inputTensors[0]->getBatchSize();
}

float *binDroneModel0_0::getInputDataPointer(int b_index)
{
  return (static_cast<MWTensor<float> *>(inputTensors[b_index]))->getData();
}

float *binDroneModel0_0::getInputDataPointer()
{
  return (static_cast<MWTensor<float> *>(inputTensors[0]))->getData();
}

float *binDroneModel0_0::getLayerOutput(int layerIndex, int portIndex)
{
  return targetImpl->getLayerOutput(layers, layerIndex, portIndex);
}

int binDroneModel0_0::getLayerOutputSize(int layerIndex, int portIndex)
{
  return static_cast<unsigned int>(
             layers[layerIndex]->getOutputTensor(portIndex)->getNumElements()) *
         sizeof(float);
}

float *binDroneModel0_0::getOutputDataPointer(int b_index)
{
  return (static_cast<MWTensor<float> *>(outputTensors[b_index]))->getData();
}

float *binDroneModel0_0::getOutputDataPointer()
{
  return (static_cast<MWTensor<float> *>(outputTensors[0]))->getData();
}

int binDroneModel0_0::getOutputSequenceLength(int layerIndex, int portIndex)
{
  return layers[layerIndex]->getOutputTensor(portIndex)->getSequenceLength();
}

namespace coder {
namespace internal {
void dlnetwork_delete(binDroneModel0_0 &obj)
{
  if (obj.isInitialized) {
    obj.cleanup();
  }
}

void dlnetwork_setup(binDroneModel0_0 &obj)
{
  try {
    obj.setup();
  } catch (std::runtime_error const &err) {
    obj.cleanup();
    checkRunTimeError(err.what(), __FILE__, __LINE__);
  } catch (...) {
    obj.cleanup();
    checkRunTimeError("", __FILE__, __LINE__);
  }
}

} // namespace internal
} // namespace coder

// End of code generation (dlnetwork.cpp)
