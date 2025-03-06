###########################################################################
## Makefile generated for component 'matModel'. 
## 
## Makefile     : matModel_rtw.mk
## Generated on : Fri Feb 21 12:34:51 2025
## Final product: .\matModel.lib
## Product type : static-library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# COMPILER_COMMAND_FILE   Compiler command listing model reference header paths
# CMD_FILE                Command file
# MODELLIB                Static library target

PRODUCT_NAME              = matModel
MAKEFILE                  = matModel_rtw.mk
MATLAB_ROOT               = C:\PROGRA~1\MATLAB\R2023b
MATLAB_BIN                = C:\PROGRA~1\MATLAB\R2023b\bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)\win64
START_DIR                 = C:\Users\kingn\Documents\MATLAB
TGT_FCN_LIB               = ISO_C++11
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 
RELATIVE_PATH_TO_ANCHOR   = ..\..\..
COMPILER_COMMAND_FILE     = matModel_rtw_comp.rsp
CMD_FILE                  = matModel_rtw.rsp
C_STANDARD_OPTS           = 
CPP_STANDARD_OPTS         = 
NODEBUG                   = 1
MODELLIB                  = matModel.lib

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          Microsoft Visual C++ 2019 v16.0 | nmake (64-bit Windows)
# Supported Version(s):    16.0
# ToolchainInfo Version:   2023b
# Specification Revision:  1.0
# 
#-------------------------------------------
# Macros assumed to be defined elsewhere
#-------------------------------------------

# NODEBUG
# cvarsdll
# cvarsmt
# conlibsmt
# ldebug
# conflags
# cflags

#-----------
# MACROS
#-----------

MW_EXTERNLIB_DIR    = $(MATLAB_ROOT)\extern\lib\win64\microsoft
MW_LIB_DIR          = $(MATLAB_ROOT)\lib\win64
CPU                 = AMD64
APPVER              = 5.02
CVARSFLAG           = $(cvarsmt)
CFLAGS_ADDITIONAL   = -D_CRT_SECURE_NO_WARNINGS
CPPFLAGS_ADDITIONAL = -EHs -D_CRT_SECURE_NO_WARNINGS /wd4251 /Zc:__cplusplus
LIBS_TOOLCHAIN      = $(conlibs)

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = 

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: Microsoft Visual C Compiler
CC = cl

# Linker: Microsoft Visual C Linker
LD = link

# C++ Compiler: Microsoft Visual C++ Compiler
CPP = cl

# C++ Linker: Microsoft Visual C++ Linker
CPP_LD = link

# Archiver: Microsoft Visual C/C++ Archiver
AR = lib

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)\mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: NMAKE Utility
MAKE = nmake


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -Zi
C_OUTPUT_FLAG       = -Fo
LDDEBUG             = /DEBUG
OUTPUT_FLAG         = -out:
CPPDEBUG            = -Zi
CPP_OUTPUT_FLAG     = -Fo
CPPLDDEBUG          = /DEBUG
OUTPUT_FLAG         = -out:
ARDEBUG             =
STATICLIB_OUTPUT_FLAG = -out:
MEX_DEBUG           = -g
RM                  = @del
ECHO                = @echo
MV                  = @ren
RUN                 = @cmd /C

#--------------------------------------
# "Faster Runs" Build Configuration
#--------------------------------------

ARFLAGS              = /nologo
CFLAGS               = $(cflags) $(CVARSFLAG) $(CFLAGS_ADDITIONAL) \
                       /O2 /Oy-
CPPFLAGS             = /TP $(cflags) $(CVARSFLAG) $(CPPFLAGS_ADDITIONAL) \
                       /O2 /Oy-
CPP_LDFLAGS          = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)
CPP_SHAREDLIB_LDFLAGS  = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) \
                         -dll -def:$(DEF_FILE)
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)
MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) \
                       -dll -def:$(DEF_FILE)



###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = .\matModel.lib
PRODUCT_TYPE = "static-library"
BUILD_TYPE = "Static Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = 

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_CUSTOM = 
DEFINES_STANDARD = -DMODEL=matModel

DEFINES = $(DEFINES_CUSTOM) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)\codegen\lib\matModel\MWAvgPoolingLayer.cpp $(START_DIR)\codegen\lib\matModel\MWBatchNormalizationLayer.cpp $(START_DIR)\codegen\lib\matModel\MWCNNLayer.cpp $(START_DIR)\codegen\lib\matModel\MWElementwiseAffineLayer.cpp $(START_DIR)\codegen\lib\matModel\MWFCLayer.cpp $(START_DIR)\codegen\lib\matModel\MWFusedConvActivationLayer.cpp $(START_DIR)\codegen\lib\matModel\MWInputLayer.cpp $(START_DIR)\codegen\lib\matModel\MWMaxPoolingLayer.cpp $(START_DIR)\codegen\lib\matModel\MWOutputLayer.cpp $(START_DIR)\codegen\lib\matModel\MWSoftmaxLayer.cpp $(START_DIR)\codegen\lib\matModel\MWTensorBase.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnAvgPoolingLayerImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnBatchNormalizationLayerImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnElementwiseAffineLayerImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnFCLayerImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnFusedConvActivationLayerImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnInputLayerImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnMaxPoolingLayerImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnOutputLayerImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnSoftmaxLayerImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnCNNLayerImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnTargetNetworkImpl.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnLayerImplFactory.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnUtils.cpp $(START_DIR)\codegen\lib\matModel\MWOnednnCustomLayerBase.cpp $(START_DIR)\codegen\lib\matModel\matModel_data.cpp $(START_DIR)\codegen\lib\matModel\matModel_initialize.cpp $(START_DIR)\codegen\lib\matModel\matModel_terminate.cpp $(START_DIR)\codegen\lib\matModel\matModel.cpp $(START_DIR)\codegen\lib\matModel\dlnetwork.cpp $(START_DIR)\codegen\lib\matModel\melSpectrogram.cpp $(START_DIR)\codegen\lib\matModel\designAuditoryFilterBank.cpp $(START_DIR)\codegen\lib\matModel\designMelFilterBank.cpp $(START_DIR)\codegen\lib\matModel\applyFilterBank.cpp $(START_DIR)\codegen\lib\matModel\predict.cpp $(START_DIR)\codegen\lib\matModel\categorical.cpp $(START_DIR)\codegen\lib\matModel\FFTImplementationCallback.cpp

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = MWAvgPoolingLayer.obj MWBatchNormalizationLayer.obj MWCNNLayer.obj MWElementwiseAffineLayer.obj MWFCLayer.obj MWFusedConvActivationLayer.obj MWInputLayer.obj MWMaxPoolingLayer.obj MWOutputLayer.obj MWSoftmaxLayer.obj MWTensorBase.obj MWOnednnAvgPoolingLayerImpl.obj MWOnednnBatchNormalizationLayerImpl.obj MWOnednnElementwiseAffineLayerImpl.obj MWOnednnFCLayerImpl.obj MWOnednnFusedConvActivationLayerImpl.obj MWOnednnInputLayerImpl.obj MWOnednnMaxPoolingLayerImpl.obj MWOnednnOutputLayerImpl.obj MWOnednnSoftmaxLayerImpl.obj MWOnednnCNNLayerImpl.obj MWOnednnTargetNetworkImpl.obj MWOnednnLayerImplFactory.obj MWOnednnUtils.obj MWOnednnCustomLayerBase.obj matModel_data.obj matModel_initialize.obj matModel_terminate.obj matModel.obj dlnetwork.obj melSpectrogram.obj designAuditoryFilterBank.obj designMelFilterBank.obj applyFilterBank.obj predict.obj categorical.obj FFTImplementationCallback.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = C:\Program Files (x86)\Intel\oneAPI\lib\dnnl.lib

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS =  /LIBPATH:"$(MATLAB_ROOT)\bin\win64" "$(MATLAB_ROOT)\bin\win64\libiomp5md.lib"

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_ = /source-charset:utf-8
CFLAGS_OPTS = /openmp /wd4101
CFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CFLAGS = $(CFLAGS) $(CFLAGS_) $(CFLAGS_OPTS) $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_ = /source-charset:utf-8
CPPFLAGS_OPTS = /openmp /wd4101
CPPFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CPPFLAGS = $(CPPFLAGS) $(CPPFLAGS_) $(CPPFLAGS_OPTS) $(CPPFLAGS_BASIC)

#---------------
# C++ Linker
#---------------

CPP_LDFLAGS_ = /nodefaultlib:vcomp  

CPP_LDFLAGS = $(CPP_LDFLAGS) $(CPP_LDFLAGS_)

#------------------------------
# C++ Shared Library Linker
#------------------------------

CPP_SHAREDLIB_LDFLAGS_ = /nodefaultlib:vcomp  

CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS) $(CPP_SHAREDLIB_LDFLAGS_)

#-----------
# Linker
#-----------

LDFLAGS_ = /nodefaultlib:vcomp  

LDFLAGS = $(LDFLAGS) $(LDFLAGS_)

#--------------------------
# Shared Library Linker
#--------------------------

SHAREDLIB_LDFLAGS_ = /nodefaultlib:vcomp  

SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS) $(SHAREDLIB_LDFLAGS_)

###########################################################################
## INLINED COMMANDS
###########################################################################


!include $(MATLAB_ROOT)\rtw\c\tools\vcdefs.mak


###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute set_environment_variables


all : build
	@cmd /C "@echo ### Successfully generated all binary outputs."


build : set_environment_variables prebuild $(PRODUCT)


prebuild : 


download : $(PRODUCT)


execute : download


set_environment_variables : 
	@set INCLUDE=$(INCLUDES);$(INCLUDE)
	@set LIB=$(LIB)


###########################################################################
## FINAL TARGET
###########################################################################

#---------------------------------
# Create a static library         
#---------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS)
	@cmd /C "@echo ### Creating static library "$(PRODUCT)" ..."
	$(AR) $(ARFLAGS) -out:$(PRODUCT) @$(CMD_FILE)
	@cmd /C "@echo ### Created: $(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


.cc.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


.cxx.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.cc.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.cxx.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)\codegen\lib\matModel}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(START_DIR)\codegen\lib\matModel}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)\codegen\lib\matModel}.cc.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)\codegen\lib\matModel}.cxx.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.cc.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.cxx.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


MWAvgPoolingLayer.obj : "$(START_DIR)\codegen\lib\matModel\MWAvgPoolingLayer.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWAvgPoolingLayer.cpp"


MWBatchNormalizationLayer.obj : "$(START_DIR)\codegen\lib\matModel\MWBatchNormalizationLayer.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWBatchNormalizationLayer.cpp"


MWCNNLayer.obj : "$(START_DIR)\codegen\lib\matModel\MWCNNLayer.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWCNNLayer.cpp"


MWElementwiseAffineLayer.obj : "$(START_DIR)\codegen\lib\matModel\MWElementwiseAffineLayer.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWElementwiseAffineLayer.cpp"


MWFCLayer.obj : "$(START_DIR)\codegen\lib\matModel\MWFCLayer.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWFCLayer.cpp"


MWFusedConvActivationLayer.obj : "$(START_DIR)\codegen\lib\matModel\MWFusedConvActivationLayer.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWFusedConvActivationLayer.cpp"


MWInputLayer.obj : "$(START_DIR)\codegen\lib\matModel\MWInputLayer.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWInputLayer.cpp"


MWMaxPoolingLayer.obj : "$(START_DIR)\codegen\lib\matModel\MWMaxPoolingLayer.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWMaxPoolingLayer.cpp"


MWOutputLayer.obj : "$(START_DIR)\codegen\lib\matModel\MWOutputLayer.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOutputLayer.cpp"


MWSoftmaxLayer.obj : "$(START_DIR)\codegen\lib\matModel\MWSoftmaxLayer.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWSoftmaxLayer.cpp"


MWTensorBase.obj : "$(START_DIR)\codegen\lib\matModel\MWTensorBase.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWTensorBase.cpp"


MWOnednnAvgPoolingLayerImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnAvgPoolingLayerImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnAvgPoolingLayerImpl.cpp"


MWOnednnBatchNormalizationLayerImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnBatchNormalizationLayerImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnBatchNormalizationLayerImpl.cpp"


MWOnednnElementwiseAffineLayerImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnElementwiseAffineLayerImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnElementwiseAffineLayerImpl.cpp"


MWOnednnFCLayerImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnFCLayerImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnFCLayerImpl.cpp"


MWOnednnFusedConvActivationLayerImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnFusedConvActivationLayerImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnFusedConvActivationLayerImpl.cpp"


MWOnednnInputLayerImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnInputLayerImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnInputLayerImpl.cpp"


MWOnednnMaxPoolingLayerImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnMaxPoolingLayerImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnMaxPoolingLayerImpl.cpp"


MWOnednnOutputLayerImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnOutputLayerImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnOutputLayerImpl.cpp"


MWOnednnSoftmaxLayerImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnSoftmaxLayerImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnSoftmaxLayerImpl.cpp"


MWOnednnCNNLayerImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnCNNLayerImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnCNNLayerImpl.cpp"


MWOnednnTargetNetworkImpl.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnTargetNetworkImpl.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnTargetNetworkImpl.cpp"


MWOnednnLayerImplFactory.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnLayerImplFactory.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnLayerImplFactory.cpp"


MWOnednnUtils.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnUtils.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnUtils.cpp"


MWOnednnCustomLayerBase.obj : "$(START_DIR)\codegen\lib\matModel\MWOnednnCustomLayerBase.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\MWOnednnCustomLayerBase.cpp"


matModel_data.obj : "$(START_DIR)\codegen\lib\matModel\matModel_data.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\matModel_data.cpp"


matModel_initialize.obj : "$(START_DIR)\codegen\lib\matModel\matModel_initialize.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\matModel_initialize.cpp"


matModel_terminate.obj : "$(START_DIR)\codegen\lib\matModel\matModel_terminate.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\matModel_terminate.cpp"


matModel.obj : "$(START_DIR)\codegen\lib\matModel\matModel.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\matModel.cpp"


dlnetwork.obj : "$(START_DIR)\codegen\lib\matModel\dlnetwork.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\dlnetwork.cpp"


melSpectrogram.obj : "$(START_DIR)\codegen\lib\matModel\melSpectrogram.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\melSpectrogram.cpp"


designAuditoryFilterBank.obj : "$(START_DIR)\codegen\lib\matModel\designAuditoryFilterBank.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\designAuditoryFilterBank.cpp"


designMelFilterBank.obj : "$(START_DIR)\codegen\lib\matModel\designMelFilterBank.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\designMelFilterBank.cpp"


applyFilterBank.obj : "$(START_DIR)\codegen\lib\matModel\applyFilterBank.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\applyFilterBank.cpp"


predict.obj : "$(START_DIR)\codegen\lib\matModel\predict.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\predict.cpp"


categorical.obj : "$(START_DIR)\codegen\lib\matModel\categorical.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\categorical.cpp"


FFTImplementationCallback.obj : "$(START_DIR)\codegen\lib\matModel\FFTImplementationCallback.cpp"
	$(CPP) $(CPPFLAGS) -Fo"$@" "$(START_DIR)\codegen\lib\matModel\FFTImplementationCallback.cpp"


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : rtw_proj.tmw $(COMPILER_COMMAND_FILE) $(MAKEFILE)


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@cmd /C "@echo ### PRODUCT = $(PRODUCT)"
	@cmd /C "@echo ### PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@cmd /C "@echo ### BUILD_TYPE = $(BUILD_TYPE)"
	@cmd /C "@echo ### INCLUDES = $(INCLUDES)"
	@cmd /C "@echo ### DEFINES = $(DEFINES)"
	@cmd /C "@echo ### ALL_SRCS = $(ALL_SRCS)"
	@cmd /C "@echo ### ALL_OBJS = $(ALL_OBJS)"
	@cmd /C "@echo ### LIBS = $(LIBS)"
	@cmd /C "@echo ### MODELREF_LIBS = $(MODELREF_LIBS)"
	@cmd /C "@echo ### SYSTEM_LIBS = $(SYSTEM_LIBS)"
	@cmd /C "@echo ### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	@cmd /C "@echo ### CFLAGS = $(CFLAGS)"
	@cmd /C "@echo ### LDFLAGS = $(LDFLAGS)"
	@cmd /C "@echo ### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	@cmd /C "@echo ### CPPFLAGS = $(CPPFLAGS)"
	@cmd /C "@echo ### CPP_LDFLAGS = $(CPP_LDFLAGS)"
	@cmd /C "@echo ### CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)"
	@cmd /C "@echo ### ARFLAGS = $(ARFLAGS)"
	@cmd /C "@echo ### MEX_CFLAGS = $(MEX_CFLAGS)"
	@cmd /C "@echo ### MEX_CPPFLAGS = $(MEX_CPPFLAGS)"
	@cmd /C "@echo ### MEX_LDFLAGS = $(MEX_LDFLAGS)"
	@cmd /C "@echo ### MEX_CPPLDFLAGS = $(MEX_CPPLDFLAGS)"
	@cmd /C "@echo ### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	@cmd /C "@echo ### EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	@cmd /C "@echo ### MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files ..."
	@if exist $(PRODUCT) $(RM) $(PRODUCT)
	$(RM) $(ALL_OBJS)
	$(ECHO) "### Deleted all derived files."


