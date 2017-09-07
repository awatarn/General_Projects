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

## Content:
##      Build and run examples using FFTW2 C wrappers to Intel MKL.
##*****************************************************************************

help:
	@echo "To run FFTW2C examples:"
	@echo "  make {libia32|dylibia32|libintel64|dylibintel64}"
	@echo "       [precision={MKL_DOUBLE|MKL_SINGLE}]"
	@echo "       [compiler=<name>] [threading=<name>]"
	@echo "       [parallel=<name>] [omp=<name>] [function=<name>]"
	@echo
	@echo "To get report on run examples:"
	@echo "  make report"
	@echo
	@echo "To clean results:"
	@echo "  make clean"
	@echo
	@echo "To get help just run make or:"
	@echo "  make help"
	@echo
	@echo "Main options:"
	@echo "  targets lib%   use static linkage"
	@echo "  targets dylib% use dynamic linkage"
	@echo
	@echo "  precision=<tag> selects type of floating-point precision:"
	@echo "      precision=MKL_DOUBLE - double (DEFAULT)"
	@echo "      precision=MKL_SINGLE - float"
	@echo
	@echo "  compiler=<name> selects the compiler to build the examples:"
	@echo "      compiler=gnu   - GNU gcc"
	@echo "      compiler=clang - Clang C compiler"
	@echo "      compiler=pgi   - PGI pgcc. Available only for intel64."
	@echo "      compiler=intel - Intel(R) C compiler icc (DEFAULT)"
	@echo
	@echo "  threading=<name> selects threading of MKL:"
	@echo "      threading=parallel   - multithreaded version (DEFAULT)"
	@echo "      threading=sequential - sequential version"
	@echo
	@echo "  parallel=<name> selects Intel(R) MKL threading layer for threading=parallel:"
	@echo "      parallel=intel - libmkl_intel_thread"
	@echo "      parallel=pgi   - libmkl_pgi_thread"
	@echo "      Default value depends on the setting of compiler=<name>"
	@echo
	@echo "  omp=<name> selects OpenMP runtime library for threading=parallel:"
	@echo "      omp=iomp5 - Intel OpenMP runtime"
	@echo "      omp=pgmp  - PGI OpenMP runtime"
	@echo "      Default value depends on the setting of parallel=<name>"
	@echo
	@echo "  function=<name> selects examples to execute"
	@echo "      Default value: all examples listed in file fftw2xc.lst"
	@echo
	@echo "Additional options:"
	@echo "  RES_DIR=<path> defines where to place the results"
	@echo "      Default value: ./_results"
	@echo
	@echo "  MKLROOT=<path> defines alternative Intel(R) MKL root directory"
	@echo "      Default value: ../.."
	@echo
	@echo "  INSTALL_DIR=<path> defines path to fftw2 wrapper library."
	@echo '      Default value: ./wrap_lib_$$(compiler)'
	@echo
	@echo "  MKLRUNLIB_PATH=<path> defines alternative runtime library directory"
	@echo "      for dynamic linkage."
	@echo '      Default value: $$(MKLROOT)/lib/$$(IA), with $$(IA) defined by the target'
	@echo
	@echo "  LIB_PATH=<path> defines location of OpenMP runtime libraries"
	@echo "      needed by GNU or PGI threading layer. Set this value if"
	@echo "      respective libraries (libgomp, libpgmp) cannot be found in"
	@echo "      LIBRARY_PATH and DYLD_LIBRARY_PATH"
	@echo
	@echo "  TARGET_ARCH=<flags> defines additional compiler flags"
	@echo "      Refer to the compiler documentation about the architecture specific"
	@echo "      flags. For example, Intel compilers may need TARGET_ARCH=-xAVX"
	@echo "      to generate Intel(R) AVX instructions."

##-----------------------------------------------------------------------------
## Usage examples:
##
## make libia32 compiler=gnu
##     Use GNU C compiler to build double-precision wrapper library,
##     compile and run examples as 32-bit applications statically linked to MKL
##
## make dylibintel64 precision=MKL_SINGLE
##     Use Intel(R) C compiler to build single-precision wrapper library,
##     build and run examples linked dynamically to MKL
##     as applications for Intel(R) 64 processor family
##-----------------------------------------------------------------------------
## Default values

MY_MAKEFILE := $(MAKEFILE_LIST)

ifndef MKLROOT
  MKLROOT = ../..
endif

ifndef INSTALL_DIR
  INSTALL_DIR = .
endif

_lib_dir = lib

ifndef MKLRUNLIB_PATH
  MKLRUNLIB_PATH = $(MKLROOT)/$(_lib_dir)
endif

ifndef RES_DIR
  RES_DIR = _results
endif

include fftw2xc.lst

ifneq ($(precision),MKL_SINGLE)
  override precision = MKL_DOUBLE
  prec = double
else
  prec = single
endif

ifndef function
  function = $(DFT)
endif

COMMON = dfti_example_support.c

RES = $(addsuffix .res,$(function))

ifeq (,$(filter gnu pgi clang,$(compiler)))
  override compiler = intel
  override parallel = intel
endif

ifeq ($(compiler)_$(_IA),pgi_ia32)
   $(info ***)
   $(info *** PGI compiler 32-bit support was dropped off starting from Intel(R) MKL 2017.)
   $(info *** Please try >make help)
   $(info ***)
   $(error )
endif

# FFTW2 wrappers support lp64 only
ifeq (intel64,$(_IA))
  override interface = lp64
  lp64_ = $(interface)_
else
  override interface =
endif

ifeq (,$(filter parallel sequential,$(threading)))
  override threading = parallel
endif

ifneq ($(threading),sequential)
_parallel_intel = intel
_parallel_gnu = intel
_parallel_clang = intel
_parallel_pgi = pgi

_omp_intel = iomp5
_omp_gnu = iomp5
_omp_clang = iomp5
_omp_pgi = pgmp

parallel = $(_parallel_$(compiler))
omp = $(_omp_$(parallel))
endif

ifeq (,$(filter gomp pgmp,$(omp)))
  override omp = iomp5
endif

ifeq ($(parallel),pgi)
  ifeq ($(compiler)-$(parallel),pgi-pgi)
    override omp = pgmp
  else
    override omp = iomp5
  endif
else
  override parallel = intel
  override omp = iomp5
endif

ifeq ($(compiler),pgi)
ifeq ($(threading),parallel)
ifeq ($(parallel),pgi)
ifeq ($(_LIB),dylib)
ifeq ($(MKL_LIBS_LINK),mixed)
	override MKL_LIBS_LINK =
endif
endif
endif
endif
endif

gnu_or_clang = $(if $(filter gnu,$(compiler)),gcc,$(filter clang,$(compiler)))
ifneq ($(gnu_or_clang),)
  CC = $(gnu_or_clang)
  COPTS.ia32 = -m32
  COPTS.intel64 = -m64
  COPTS.DIAG = -Wall -Werror
  IFACE_COMP_PART = intel# gf missing
else
  ifeq ($(compiler),pgi)
    CC = pgcc
    COPTS.ia32 = -tp core2
    COPTS.intel64 = -tp core2-64
    COPTS.DIAG = 
  else
    CC = icc
    COPTS.DIAG = -Wall -Werror
  endif
  IFACE_COMP_PART = intel
endif

ifeq ($(compiler)-$(parallel),pgi-pgi)
  IFACE_THREADING_PART = pgi
else
  IFACE_THREADING_PART = intel
endif

IFACE_LIB.ia32 = mkl_$(IFACE_COMP_PART)
ifeq ($(interface),ilp64)
  COPTS.intel64 += -DMKL_ILP64
  IFACE_LIB.intel64 = mkl_$(IFACE_COMP_PART)_ilp64
else
  IFACE_LIB.intel64 = mkl_$(IFACE_COMP_PART)_lp64
endif
IFACE_LIB = $(IFACE_LIB.$(_IA))

ifeq ($(threading),sequential)
  threadname = $(threading)
  threadlayer = $(threading)
  THREADING_LIB = mkl_sequential
  OMP_LIB =
else
  threadname = $(threading)_$(omp)
  threadlayer = $(parallel)
  THREADING_LIB = mkl_$(IFACE_THREADING_PART)_thread
  ifeq ($(omp),iomp5)
    OMP_LIB = -L$(CMPLR_PATH) -l$(omp)
  else
    ifneq ($(LIB_PATH),)
      OMP_LIB = -L$(LIB_PATH)
    endif
    OMP_LIB += -l$(omp)
  endif
endif

CORE_LIB = mkl_core

WRAP_LIB_DIR = $(INSTALL_DIR)/wrap_lib_$(compiler)/$(_IA)
WRAP_LIB_NAME = fftw2xc_$(prec)
WRAP_LIB_EXT = a

COPTS = -D$(precision) -I$(MKLROOT)/include -I$(MKLROOT)/include/fftw
ifeq ($(prec),single)
  COPTS += -DFFTW_ENABLE_FLOAT
endif

ifeq ($(compiler)-$(IFACE_THREADING_PART),pgi-pgi)
  COPTS += -mp -pgf90libs
endif

ifeq ($(compiler),pgi)
  COPTS += -Mnokeepobj -Wl,-no_compact_unwind
endif

COPTS += $(COPTS.$(_IA)) $(COPTS.DIAG) $(CFLAGS) $(CPPFLAGS)

MKL_PATH = $(MKLROOT)/$(_lib_dir)

CMPLR_PATH = $(MKLROOT)/../compiler/lib

# Finally compose the MKL_LIBS
ifeq ($(_LIB),dylib)
  ifneq ($(MKL_LIBS_LINK),mixed)
    MKL_LIBS = -L$(MKL_PATH) -lmkl_rt
  else
    MKL_LIBS = -L$(MKL_PATH) -l$(IFACE_LIB) -l$(THREADING_LIB) -l$(CORE_LIB)
  endif
else
  MKL_LIBS += $(MKL_PATH)/lib$(IFACE_LIB).a
  MKL_LIBS += $(MKL_PATH)/lib$(THREADING_LIB).a
  MKL_LIBS += $(MKL_PATH)/lib$(CORE_LIB).a
endif

RUNENV = DYLD_BIND_AT_LAUNCH=1

ifneq ($(LIB_PATH),)
  RUNENV += DYLD_LIBRARY_PATH=$(LIB_PATH):$(DYLD_LIBRARY_PATH)
endif

ifeq ($(_LIB),dylib)
  RPATH = -Wl,-rpath,$(MKLRUNLIB_PATH) -Wl,-rpath,$(CMPLR_PATH) -Wl,-rpath,$(TBB_PATH)
  ifneq ($(MKL_LIBS_LINK),mixed)
    RUNENV += MKL_INTERFACE_LAYER=$(interface) MKL_THREADING_LAYER=$(threadlayer)
  endif
else
  RPATH = -Wl,-rpath,$(CMPLR_PATH)
endif

res_dir = $(RES_DIR)/$(compiler)_$(lp64_)$(threadname)_$(_LIB)$(_IA)_$(prec)

COMMON.obj = $(COMMON:%.c=$(res_dir)/%.o)

ifdef VERBOSE
  $(info )
  $(info MAKELEVEL=$(MAKELEVEL))
  $(info MKLROOT=$(MKLROOT) RES_DIR=$(RES_DIR))
  $(info compiler=$(compiler) interface=$(interface) threading=$(threading) \
         parallel=$(parallel) omp=$(omp))
endif

##-----------------------------------------------------------------------------
## Rules

vpath %.c source

.SUFFIXES:
.SUFFIXES: .c .res .out

.PHONY: lib32 libia32 dylib32 dylibia32 \
        libem64t libintel64 dylibem64t dylibintel64 \
        clean cleanup run mkresdir wrap_lib FORCE

libia32 lib32:
	$(MAKE) -f $(MY_MAKEFILE) run _IA=ia32 _LIB=lib

dylib32 dylibia32:
	$(MAKE) -f $(MY_MAKEFILE) run _IA=ia32 _LIB=dylib

libintel64 libem64t:
	$(MAKE) -f $(MY_MAKEFILE) run _IA=intel64 _LIB=lib

dylibem64t dylibintel64:
	$(MAKE) -f $(MY_MAKEFILE) run _IA=intel64 _LIB=dylib

run: mkresdir wrap_lib $(COMMON.obj) $(RES)

mkresdir:
	@echo
	@echo See results in $(res_dir)
	@echo
	mkdir -p $(res_dir)

wrap_lib: $(WRAP_LIB_DIR)/lib$(WRAP_LIB_NAME).$(WRAP_LIB_EXT)

# We shall use absolute path for WRAP_LIB_DIR
_install_to=$(shell mkdir -p $(WRAP_LIB_DIR); cd $(WRAP_LIB_DIR) && pwd)
$(WRAP_LIB_DIR)/lib$(WRAP_LIB_NAME).$(WRAP_LIB_EXT):
	@echo
	# build $@
	cd $(MKLROOT)/interfaces/fftw2xc && $(MAKE) lib$(_IA) \
	   PRECISION=$(precision) compiler=$(compiler) \
	   INSTALL_DIR=$(_install_to) \
	   INSTALL_LIBNAME=lib$(WRAP_LIB_NAME).$(WRAP_LIB_EXT)

$(res_dir)/%.o: %.c
	# compile $*.c
	$(CC) $(COPTS) $(TARGET_ARCH) \
	  -c $< -o $@

.PRECIOUS: $(res_dir)/%.out
$(res_dir)/%.out: %.c FORCE
	@echo
	# compile $*.c
	$(CC) $(COPTS) $(TARGET_ARCH) \
	  $< \
	  $(COMMON.obj) \
	  -L$(WRAP_LIB_DIR) -l$(WRAP_LIB_NAME) \
	  $(MKL_LIBS) $(RPATH) \
	  $(OMP_LIB) -lpthread -lm -o $(res_dir)/$*.out

%.res: mkresdir $(res_dir)/%.out FORCE
	-rm -f $(res_dir)/$@ # remove $@ file
	@echo
	# run $*.out
	$(RUNENV) \
	  $(res_dir)/$*.out > $(res_dir)/$@

report: FORCE
	@echo
	@echo "--------------------------------------------------"
	@echo There are `ls source/*_ex[0-9]*.c | wc -l` \
	  examples in source
	@echo
	@for D in `ls -d $(RES_DIR)/* 2>/dev/null`; do \
	  echo -n "In   $$D	Total run examples:" \
	    "`ls $$D/*.res 2>/dev/null | wc -l`"; \
	  echo -n "	and Passed: "; \
	  cat /dev/null $$D/*.res 2>/dev/null | grep -c PASSED | cat; \
	done
	@echo "--------------------------------------------------"
	@echo

clean: FORCE
	-rm -rf $(RES_DIR) # clean all results

cleanup: clean
	-rm -rf $(INSTALL_DIR)/wrap_lib_* # clean all built wrap_libs

FORCE: ;

##-----------------------------------------------------------------------------
