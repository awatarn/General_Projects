#===============================================================================
# Copyright 2006-2017 Intel Corporation All Rights Reserved.
#
# The source code,  information  and material  ("Material") contained  herein is
# owned by Intel Corporation or its  suppliers or licensors,  and  title to such
# Material remains with Intel  Corporation or its  suppliers or  licensors.  The
# Material  contains  proprietary  information  of  Intel or  its suppliers  and
# licensors.  The Material is protected by  worldwide copyright  laws and treaty
# provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
# modified, published,  uploaded, posted, transmitted,  distributed or disclosed
# in any way without Intel's prior express written permission.  No license under
# any patent,  copyright or other  intellectual property rights  in the Material
# is granted to  or  conferred  upon  you,  either   expressly,  by implication,
# inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
# property rights must be express and approved by Intel in writing.
#
# Unless otherwise agreed by Intel in writing,  you may not remove or alter this
# notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
# suppliers or licensors in any way.
#===============================================================================

##  Content:
##      Intel(R) Math Kernel Library Trigonometric Transforms examples creation
##      and run
##
##******************************************************************************


help:
	@echo "Usage: make {libia32|libintel64|dylibia32|dylibintel64} [function=name]"
	@echo "[compiler=compiler_name] [interface=interface_name] [threading=threading_name]"
	@echo
	@echo "name           - function name. Please see pdettc.lst file"
	@echo
	@echo "compiler_name  - can be gnu, clang or intel. Default value is intel."
	@echo "                 Intel(R) C Compiler as default"
	@echo
	@echo "interface_name - can be lp64 or ilp64 for intel64. Default value is lp64."
	@echo
	@echo "threading_name - can be parallel or sequential. Default value is parallel."

##------------------------------------------------------------------------------
## example of usage:
##
## make libia32 function=d_trig_tforms_bvp - build by Intel(R) C Compiler (as default) and
##                                          run d_trig_tforms_bvp example for 32-bit applications,
##                                          static linking
##
##------------------------------------------------------------------------------

include pdettc.lst

ifndef function
function = $(PDE_TT_C)
endif

ifndef compiler
compiler=intel
endif

ifndef interface
interface=lp64
endif

ifndef threading
threading=parallel
endif

RES = $(addsuffix .res ,$(function))

ifndef MKLROOT
MKLROOT = ../..
endif
MKL_PATH = "$(MKLROOT)/lib"
CMPLR_PATH = "$(MKLROOT)/../compiler/lib"

gnu_or_clang = $(if $(filter gnu,$(compiler)),gcc,$(filter clang,$(compiler)))
ifneq ($(gnu_or_clang),)
CC=$(gnu_or_clang) $(OPT_M64) -I/usr/include/malloc
IFACE_COMP_PART=intel
IFACE_THREADING_PART=intel
OPT_M64=-m64
else
override CC=icc
IFACE_COMP_PART=intel
IFACE_THREADING_PART=intel
OPT_M64=
endif

ifeq ($(interface),ilp64)
IFACE_LIB=$(MKL_PATH)/libmkl_$(IFACE_COMP_PART)_ilp64.$(EXT)
COPTS = -DMKL_ILP64
else
IFACE_LIB=$(MKL_PATH)/libmkl_$(IFACE_COMP_PART)_lp64.$(EXT)
COPTS =
endif

ifeq ($(_IA),ia32)
IFACE_LIB=$(MKL_PATH)/libmkl_$(IFACE_COMP_PART).$(EXT)
COPTS =
OPT_M64=-m32
endif

ifeq ($(threading),sequential)
THREADING_LIB=$(MKL_PATH)/libmkl_sequential.$(EXT)
OMP_LIB =
else
THREADING_LIB=$(MKL_PATH)/libmkl_$(IFACE_THREADING_PART)_thread.$(EXT)
OMP_LIB = -L$(CMPLR_PATH) -liomp5
endif

RPATH = -Wl,-rpath,$(MKL_PATH) -Wl,-rpath,$(CMPLR_PATH)
CORE_LIB=$(MKL_PATH)/libmkl_core.$(EXT)

MKL_LIBS=$(IFACE_LIB) $(THREADING_LIB) $(CORE_LIB) $(OMP_LIB)

ifeq ($(_IA),ia32)
RES_DIR=_results/$(compiler)_$(threading)_$(_IA)_$(RES_EXT)$Z
else
RES_DIR=_results/$(compiler)_$(interface)_$(threading)_$(_IA)_$(RES_EXT)$Z
endif


libia32 lib32:
	$(MAKE) $(RES) _IA=ia32 EXT=a RES_EXT=lib
dylibia32 dylib32:
	$(MAKE) $(RES) _IA=ia32 EXT=dylib RES_EXT=dylib
libintel64 libem64t:
	$(MAKE) $(RES) _IA=intel64 EXT=a RES_EXT=lib
dylibintel64 dylibem64t:
	$(MAKE) $(RES) _IA=intel64 EXT=dylib RES_EXT=dylib

#-------------------------------------------------------------------------------
vpath %.c source

$(RES): %.res: %.c
	mkdir -p ./$(RES_DIR)
	$(CC) -Wall -Werror $(COPTS) -I$(MKLROOT)/include $< -L$(MKL_PATH) $(MKL_LIBS) $(RPATH) \
			-lpthread -lm -o $(RES_DIR)/$*.out
	$(RES_DIR)/$*.out >$(RES_DIR)/$@
#-------------------------------------------------------------------------------
