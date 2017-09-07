#===============================================================================
# Copyright 2008-2017 Intel Corporation All Rights Reserved.
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
##      Intel(R) Math Kernel Library. Single Dynamic Library Interface example.
##******************************************************************************

help:
	@echo $$'$(WRONG_OPTION)'
	@echo "Intel(R) MKL Single Dynamic Library Interface example help."
	@echo "    The example demonstrates the dynamical setting of Intel(R) MKL threading layer:"
	@echo "        1) by the specifying of environment variable MKL_THREADING_LAYER and"
	@echo "        2) by the functional call to MKL_Set_Threading_Layer()."
	@echo ""
	@echo "Usage: make Target [Options...]"
	@echo ""
	@echo "   Target:"
	@echo "       ia32    - dynamically linked example"
	@echo "                     for processors that use the IA-32 architecture"
	@echo "       intel64 - dynamically linked example"
	@echo "                     for processors that use the Intel(R) 64 architecture"
	@echo "       help    - print this help"
	@echo ""
	@echo "   Options:"
	@echo "       compiler={intel|gnu|clang}"
	@echo "           Specifies the compiler used to compile the example."
	@echo "           Default: intel."
	@echo "       MKLROOT=<MKL_directory>"
	@echo "           Specifies the location of Intel(R) MKL libraries used to build this example."
	@echo "           Default: the Intel(R) MKL installation directory."
	@echo ""
	@echo "Usage examples:"
	@echo ""
	@echo "   make intel64"
	@echo "       Link 'Single Dynamic Library Interface' example"
	@echo "           for processors that use the Intel(R) 64 architecture"
	@echo "           using Intel compiler"
	@echo ""
	@echo "   make ia32 compiler=gnu"
	@echo "       Link 'Single Dynamic Library Interface' example"
	@echo "           for processors that use the IA-32 architecture"
	@echo "           using GNU compiler"
	@echo ""
#------------------------------------------------------------------------------

FUNCTION_LIST = sdli
TARGET = $(FUNCTION_LIST)

# Defaults
compiler=intel

ifneq ($(compiler),intel)
ifneq ($(compiler),gnu)
ifneq ($(compiler),clang)
MSG2+= compiler=$(compiler)
endif
endif
endif

ifneq ("$(MSG2)","")
WRONG_OPTION=\n\n*** COMMAND LINE ERROR: Wrong value of option(s):  $(MSG2)\n\n
TARGET=help
endif
###

ifdef _IA

ifndef MKLROOT
  MKLROOT = ../..
endif
MKL_PATH = $(MKLROOT)/lib

gnu_or_clang = $(if $(filter gnu,$(compiler)),gcc,$(filter clang,$(compiler)))
ifneq ($(gnu_or_clang),)
  override COMPILER = $(gnu_or_clang)
else
  override COMPILER = icc
endif
OPTIONS =

ifeq ($(_IA),ia32)
  M32_64 = -m32 # This option tells compiler to generate code for IA-32 architecture.
else
  M32_64 = -m64 # This option tells compiler to generate code for Intel64 architecture.
endif

# required libraries
IFACE_LIB=-L"$(MKL_PATH)" -lmkl_rt
RPATH=-Wl,-rpath,$(MKL_PATH) -Wl,-rpath,$(CMPLR_PATH)

CMPLR_PATH = $(MKLROOT)/../compiler/lib
OMP_LIB = -L"$(CMPLR_PATH)" -liomp5

LABEL = $(compiler)_$(_IA)
RES_DIR = _results/$(LABEL)

OUT_TO_FILE=>> $(RES_DIR)/$@.res
PRINT_RESULT=@cat $(RES_DIR)/$@.res

endif # ifdef _IA

#-------------------------------------------------------------------------------

ia32:
	@$(MAKE) $(TARGET) --no-print-directory _IA=ia32
intel64:
	@$(MAKE) $(TARGET) --no-print-directory _IA=intel64

#-------------------------------------------------------------------------------

$(FUNCTION_LIST):
	@echo "----- Intel(R) MKL Single Dynamic Library Interface example -----"
	@mkdir -p ./$(RES_DIR)
	@echo $$'\n----- Compiling $(LABEL) ----- $@'
	@echo $$'      The single Intel(R) MKL library is linked: $(IFACE_LIB)\n'
	$(COMPILER) $(M32_64) $(OPTIONS) -I"$(MKLROOT)/include" \
	./source/$@.c \
	$(IFACE_LIB) \
	$(RPATH) \
	$(OMP_LIB) -lpthread -lm \
	-o $(RES_DIR)/$@.out

	@echo "Intel(R) MKL Single Dynamic Library Interface example" > $(RES_DIR)/$@.res
	@echo
	@echo "----- Run the example $@"
	@echo "Case 1: Run $@ with default behavior: no parameters, no Intel(R) MKL environment variables"
	@echo "Case 1: Run $@ with default behavior:" $(OUT_TO_FILE)
	@echo "        no parameters, no Intel(R) MKL environment variables" $(OUT_TO_FILE)
	@echo "        Intel(R) MKL Intel parallel threading layer will be used" $(OUT_TO_FILE)
	@$(RES_DIR)/$@.out $(OUT_TO_FILE)

	@echo "Case 2: Set environment variable MKL_THREADING_LAYER"
	@echo "Case 2: Set environment variable MKL_THREADING_LAYER" $(OUT_TO_FILE)
	@echo "        set MKL_THREADING_LAYER=SEQUENTIAL" $(OUT_TO_FILE)
	@echo "        Intel(R) MKL sequential threading layer will be used" $(OUT_TO_FILE)
	@export MKL_THREADING_LAYER=SEQUENTIAL; \
		$(RES_DIR)/$@.out $(OUT_TO_FILE)

	@echo "Case 3: Environment variable vs the function call"
	@echo "Case 3: Environment variable vs the function call" $(OUT_TO_FILE)
	@echo "        set MKL_THREADING_LAYER=SEQUENTIAL" $(OUT_TO_FILE)
	@echo "        Run $@ with the parameter 'par'"  $(OUT_TO_FILE)
	@echo "        The example will set PARALLEL threading Layer" $(OUT_TO_FILE)
	@echo "            by the call MKL_Set_Threading_Layer(MKL_THREADING_INTEL)" $(OUT_TO_FILE)
	@echo "        The function call has precedence over environment variable" $(OUT_TO_FILE)
	@echo "        Intel threading layer will be used" $(OUT_TO_FILE)
	@export MKL_THREADING_LAYER=SEQUENTIAL; \
		$(RES_DIR)/$@.out 'par' $(OUT_TO_FILE)

	@echo "Case 4: Select sequential Intel(R) MKL by the call MKL_Set_Threading_Layer(MKL_THREADING_SEQUENTIAL)"
	@echo "Case 4: Select sequential Intel(R) MKL by the call MKL_Set_Threading_Layer(MKL_THREADING_SEQUENTIAL)" $(OUT_TO_FILE)
	@echo "        Run $@ with the parameter 'seq'" $(OUT_TO_FILE)
	@echo "        The example will set SEQUENTIAL threading Layer" $(OUT_TO_FILE)
	@echo "            by the call MKL_Set_Threading_Layer(MKL_THREADING_SEQUENTIAL)" $(OUT_TO_FILE)
	@$(RES_DIR)/$@.out 'seq' $(OUT_TO_FILE)

	@echo ----- The example '$@' completed
	@echo
	@echo ----- Results [ file $(RES_DIR)/$@.res ]:
	$(PRINT_RESULT)
	@echo ----- EOF -----
	@rm -f $(RES_DIR)/$@.o

#-------------------------------------------------------------------------------
