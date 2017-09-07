#===============================================================================
# Copyright 2003-2017 Intel Corporation All Rights Reserved.
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
##      Intel(R) Math Kernel Library VSL examples creation and run
##******************************************************************************

help:
	@echo "Usage: make {libia32|dylibia32|libintel64|dylibintel64} [function=name]"
	@echo "[compiler=compiler_name] [interface=interface_name] [threading=threading_name]"
	@echo "[parallel=parallel_name]"
	@echo "name     - function name in RNGs case and method name in CONV/CORR case. Please see vsl.lst file."
	@echo "compiler_name  - can be gnu, clang, pgi, or intel. Default value is intel."
	@echo "                 Intel(R) C Compiler as default."
	@echo "                 If compiler=gnu then GNU C compiler will be used."
	@echo "                 If compiler=clang then Clang C compiler will be used."
	@echo "                 If compiler=pgi then PGI C compiler will be used. Available only for intel64."
	@echo "interface_name - can be lp64 or ilp64 for intel64. Default value is lp64."
	@echo "threading_name - can be parallel or sequential. Default value is parallel."
	@echo "parallel_name  - can be intel or pgi (only if compiler=pgi). Default value is intel."

##------------------------------------------------------------------------------
## examples of using:
##
## make libia32 function=vsrngcauchy - build by Intel(R) C compiler (as default) and
##                                   run VSRNGCAUCHY example for 32-bit applications,
##                                   static linking
##
## make dylibia32 compiler=gnu - build by GNU C compiler and run
##                           all examples of Intel(R) MKL for 32-bit applications, dynamic
##                           linking
##------------------------------------------------------------------------------

include vsl.lst

ifndef function
function = $(VSL)
endif

ifeq (,$(filter gnu clang pgi,$(compiler)))
   override compiler=intel
   override parallel=intel
endif

ifeq ($(compiler)_$(_IA),pgi_ia32)
   $(info ***)
   $(info *** PGI compiler 32-bit support was dropped off starting from Intel(R) MKL 2017.)
   $(info *** Please try >make help)
   $(info ***)
   $(error )
endif

ifneq ($(interface),ilp64)
   override interface=lp64
endif

ifneq ($(threading),sequential)
   override threading=parallel
endif

ifeq ($(parallel),pgi)
   override omp=pgmp
else
   override parallel=intel
   override omp=iomp5
endif

RES = $(addsuffix .res ,$(function))

ifndef MKLROOT
   MKLROOT = ../..
endif
MKL_PATH = "$(MKLROOT)/lib"
CMPLR_PATH = "$(MKLROOT)/../compiler/lib"

ifeq ($(interface),ilp64)
   IFACE_LIB=$(MKL_PATH)/libmkl_$(IFACE_COMP_PART)_ilp64.$(EXT)
   COPTS = -DMKL_ILP64
else
   IFACE_LIB=$(MKL_PATH)/libmkl_$(IFACE_COMP_PART)_lp64.$(EXT)
   COPTS =
endif

gnu_or_clang = $(if $(filter gnu,$(compiler)),gcc,$(filter clang,$(compiler)))
ifneq ($(gnu_or_clang),)
   CC = $(gnu_or_clang)
   COPTS += -w
endif

ifeq ($(compiler),intel)
   CC=icc
   COPTS += -w
endif

ifeq ($(compiler),pgi)
   CC=pgcc
   COPTS += -Minform=severe -Mnokeepobj
endif

IFACE_COMP_PART=intel

ifeq ($(parallel),intel)
   IFACE_THREADING_PART=intel
endif

ifeq ($(parallel),pgi)
   IFACE_THREADING_PART=pgi
endif

ifeq ($(compiler),pgi)
ifeq ($(threading),parallel)
ifeq ($(parallel),pgi)
ifeq (dylib,$(findstring dylib,$(MAKECMDGOALS)))
$(warning ***)
$(warning *** Targets dylibia32 and dylibintel64 are not available for pgi-parallel examples)
$(warning ***)
$(error Try >make help)
endif
endif
endif
endif

ifeq ($(_IA),ia32)
   IFACE_LIB=$(MKL_PATH)/libmkl_$(IFACE_COMP_PART).$(EXT)
endif

ifeq ($(threading),sequential)
   THREADING_LIB=$(MKL_PATH)/libmkl_sequential.$(EXT)
   OMP_LIB =
else
   THREADING_LIB=$(MKL_PATH)/libmkl_$(IFACE_THREADING_PART)_thread.$(EXT)
   ifeq ($(parallel),pgi)
      COPTS += -mp -pgf90libs
   endif
   ifeq ($(omp),pgmp)
      OMP_LIB =
   else
      OMP_LIB = -L$(CMPLR_PATH) -l$(omp)
   endif
endif

RPATH = -Wl,-rpath,$(MKL_PATH) -Wl,-rpath,$(CMPLR_PATH)
CORE_LIB=$(MKL_PATH)/libmkl_core.$(EXT)

MKL_LIBS=$(IFACE_LIB) $(THREADING_LIB) $(CORE_LIB) $(OMP_LIB)

ifeq ($(_IA),ia32)
   ifeq ($(threading),parallel)
      RES_DIR=_results/$(compiler)_$(threading)_$(parallel)_$(omp)_$(_IA)_$(RES_EXT)$Z
      RES_DIR_ESSL=_results_essl/$(compiler)_$(threading)_$(parallel)_$(omp)_$(_IA)_$(RES_EXT)$Z
   else
      RES_DIR=_results/$(compiler)_$(threading)_$(_IA)_$(RES_EXT)$Z
      RES_DIR_ESSL=_results_essl/$(compiler)_$(threading)_$(_IA)_$(RES_EXT)$Z
   endif
   ifneq ($(gnu_or_clang),)
      COPTS:=$(COPTS) -m32
   endif
else
   ifeq ($(threading),parallel)
      RES_DIR=_results/$(compiler)_$(interface)_$(threading)_$(parallel)_$(omp)_$(_IA)_$(RES_EXT)$Z
      RES_DIR_ESSL=_results_essl/$(compiler)_$(interface)_$(threading)_$(parallel)_$(omp)_$(_IA)_$(RES_EXT)$Z
   else
      RES_DIR=_results/$(compiler)_$(interface)_$(threading)_$(_IA)_$(RES_EXT)$Z
      RES_DIR_ESSL=_results_essl/$(compiler)_$(interface)_$(threading)_$(_IA)_$(RES_EXT)$Z
   endif
   ifneq ($(gnu_or_clang),)
      COPTS:=$(COPTS) -m64
   endif
endif

libia32 lib32:
	$(MAKE) $(RES) EXT=a _IA=ia32 RES_EXT=lib
dylibia32 dylib32:
	$(MAKE) $(RES) EXT=dylib _IA=ia32 RES_EXT=dylib
libintel64 libem64t:
	$(MAKE) $(RES) EXT=a _IA=intel64 RES_EXT=lib
dylibintel64 dylibem64t:
	$(MAKE) $(RES) EXT=dylib _IA=intel64 RES_EXT=dylib

#-------------------------------------------------------------------------------

%.res: ./source/%.c
	mkdir -p ./$(RES_DIR)
	$(CC) $(COPTS) -I$(MKLROOT)/include $< $(MKL_LIBS) $(RPATH) -lm -lpthread -o $(RES_DIR)/$*.out
	echo ./source/$(*F).dat | $(RES_DIR)/$*.out >$(RES_DIR)/$@
	rm -f $(*F).dat

%.res: ./essl/%.c
	mkdir -p ./$(RES_DIR_ESSL)
	$(CC) $(COPTS) -I$(MKLROOT)/include -I./essl/vsl_wrappers $< $(subst example_,vsl_wrappers/sample_,$<) $(MKL_LIBS) $(RPATH) -lm -lpthread -o $(RES_DIR_ESSL)/$*.out
	echo ./essl/$(*F).dat | $(RES_DIR_ESSL)/$*.out >$(RES_DIR_ESSL)/$@
	rm -f $(*F).dat
#-------------------------------------------------------------------------------
