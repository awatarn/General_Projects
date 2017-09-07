#===============================================================================
# Copyright 2004-2017 Intel Corporation All Rights Reserved.
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
##      Intel(R) Math Kernel Library Extended Eigensolvers create and run examples
##******************************************************************************

include solvers_eec.lst

help:
	@echo $$'$(WRONG_OPTION)'
	@echo "Intel(R) MKL Extended Eigensolvers example ."
	@echo ""
	@echo "Usage: make Target [Options...] [Function...]"
	@echo ""
	@echo "   Target:"
	@echo "       libia32      - statically linked example for processors that use the IA-32 architecture"
	@echo "       dylibia32    - dynamically linked example for processors that use the IA-32 architecture"
	@echo "       libintel64   - statically linked example for processors that use the Intel(R) 64 architecture"
	@echo "       dylibintel64 - dynamically linked example for processors that use the Intel(R) 64 architecture"
	@echo "       help         - print this help"
	@echo ""
	@echo "   Options:"
	@echo "       interface={lp64|ilp64}"
	@echo "           Programming interface for intel64."
	@echo "           Default: lp64."
	@echo "       threading={parallel|sequential}"
	@echo "           Specifies whether to use Intel(R) MKL in threaded or sequential mode."
	@echo "           Default: parallel."
	@echo "       compiler={intel|gnu|clang}"
	@echo "           Specifies the compiler used to compile the example."
	@echo "           Default: intel."
	@echo "       MKLROOT=<MKL_directory>"
	@echo "           Specifies the location of Intel(R) MKL libraries used to build this example."
	@echo "           Default: the Intel(R) MKL installation directory."
	@echo ""
	@echo "   Function:"
	@echo "       function=function_name1 function_name2 ..."
	@echo "           Specifies the list of required functions."
	@echo "           The supported function list:"
	@echo "               $(SOLVERS_EE_SPARSE)"
	@echo "               $(SOLVERS_EE_BANDED)"
	@echo "               $(SOLVERS_EE_DENSE)"
	@echo "           Default: all examples."
	@echo ""
	@echo "Usage examples:"
	@echo ""
	@echo "   make libia32 function=\"dexample_sparse_c\""
	@echo "       Link 'dexample_sparse_c' examples against parallel static MKL-IA32."
	@echo ""
	@echo "   make dylibintel64 interface=ilp64 threading=sequential"
	@echo "       Link all the examples against sequential dynamic MKL-Intel64"
	@echo "       using ILP64 interface."
	@echo ""
#------------------------------------------------------------------------------


ifndef function
function = $(SOLVERS_EE)
endif

TARGET = $(function)

# Defaults
compiler=intel
interface=lp64
threading=parallel

ifneq ($(compiler),intel)
ifneq ($(compiler),gnu)
ifneq ($(compiler),clang)
MSG2+= compiler=$(compiler)
endif
endif
endif

ifneq ($(interface),ilp64)
ifneq ($(interface),lp64)
MSG2+= interface=$(interface)
endif
endif

ifneq ($(threading),parallel)
ifneq ($(threading),sequential)
MSG2+= threading=$(threading)
endif
endif

ifneq ("$(MSG2)","")
WRONG_OPTION=\n\n*** COMMAND LINE ERROR: Wrong value of option(s):  $(MSG2)\n\n
TARGET=help
endif
###

ifdef _IA

ifeq ($(SD),static)
  EXT=a
  RES_EXT=lib
else
  EXT=dylib
  RES_EXT=dylib
endif

ifndef MKLROOT
MKLROOT = ../..
endif
MKL_PATH = $(MKLROOT)/lib
CMPLR_PATH = $(MKLROOT)/../compiler/lib

gnu_or_clang = $(if $(filter gnu,$(compiler)),gcc,$(filter clang,$(compiler)))
ifneq ($(gnu_or_clang),)
  override COMPILER = $(gnu_or_clang)
else
  override COMPILER = icc
endif
IFACE_COMP_PART = intel
IFACE_THREADING_PART = intel
OPTIONS = -w

ifeq ($(_IA),ia32)
  ifeq ($(interface),ilp64)
    $(warning  *** ILP64 interface is not available for MKL-IA32)
    $(error Try >make help)
  endif
  ifeq ($(compiler),intel)
      # This option tells the compiler to generate optimized code
      # for Pentium or later processor.
      # If you don't need it, you can remove this option.
    OPTIONS += -mia32
  endif
  IFACE_SUFF=
  M32_64 = -m32 # This option tells compiler to generate code for IA-32 architecture.
else
  IFACE_SUFF=_$(interface)
  M32_64 = -m64 # This option tells compiler to generate code for Intel64 architecture.
endif

ifeq ($(EXT),dylib)
    MKL_PREFIX=-l
    MKL_SUFFIX=
    MKL_LD_PATH=-L"$(MKL_PATH)"
else
    MKL_PREFIX="$(MKL_PATH)/lib
    MKL_SUFFIX=.a"
    MKL_LD_PATH=
endif
IFACE_LIB=$(MKL_PREFIX)mkl_$(IFACE_COMP_PART)$(IFACE_SUFF)$(MKL_SUFFIX)

ifeq ($(interface),ilp64)
  OPTIONS += -DMKL_ILP64
endif

ifeq ($(threading),sequential)
  THREADING_LIB=$(MKL_PREFIX)mkl_sequential$(MKL_SUFFIX)
  OMP_LIB =
else
  THREADING_LIB=$(MKL_PREFIX)mkl_$(IFACE_THREADING_PART)_thread$(MKL_SUFFIX)
  OMP_LIB = -L"$(CMPLR_PATH)" -liomp5
endif

RPATH = -Wl,-rpath,$(MKL_PATH) -Wl,-rpath,$(CMPLR_PATH)
CORE_LIB=$(MKL_PREFIX)mkl_core$(MKL_SUFFIX)

LABEL = $(compiler)$(IFACE_SUFF)_$(threading)_$(_IA)_$(RES_EXT)
RES_DIR = _results/$(LABEL)

ifneq ("$(out_to_screen)","")
OUT_TO_FILE=
else
OUT_TO_FILE=> $(RES_DIR)/$@.res
endif

endif # ifdef _IA

#-------------------------------------------------------------------------------

ia32: libia32 dylibia32
intel64: libintel64 dylibintel64

libia32 lib32:
	@$(MAKE) $(TARGET) --no-print-directory SD=static  _IA=ia32
dylibia32 dylib32:
	@$(MAKE) $(TARGET) --no-print-directory SD=dynamic _IA=ia32
libintel64 libem64t:
	@$(MAKE) $(TARGET) --no-print-directory SD=static  _IA=intel64
dylibintel64 dylibem64t:
	@$(MAKE) $(TARGET) --no-print-directory SD=dynamic _IA=intel64

#-------------------------------------------------------------------------------

$(SOLVERS_EE):
	@mkdir -p ./$(RES_DIR)
	@echo $$'\n----- Compiling $(LABEL) ----- $@'
	$(COMPILER) $(M32_64) $(OPTIONS) -I"$(MKLROOT)/include" \
	./source/$@.c \
	$(MKL_LD_PATH) $(IFACE_LIB) \
	$(THREADING_LIB) \
	$(CORE_LIB) \
	$(RPATH) \
	$(OMP_LIB) -lpthread -lm -o $(RES_DIR)/$@.out
	@echo ----- Execution $(LABEL) ----- $@
	$(RES_DIR)/$@.out $(OUT_TO_FILE)

#-------------------------------------------------------------------------------
