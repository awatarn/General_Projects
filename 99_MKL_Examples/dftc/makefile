#===============================================================================
# Copyright 2005-2017 Intel Corporation All Rights Reserved.
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
##      Build and run Intel(R) Math Kernel Library C-style DFT examples.
##*****************************************************************************

help:
	@echo "To run DFTC examples:"
	@echo "  make {libia32|dylibia32|libintel64|dylibintel64}"
	@echo "       [compiler=<name>] [interface=<name>] [threading=<name>]"
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
	@echo "  compiler=<name> selects the compiler to build the examples:"
	@echo "      compiler=gnu   - GNU gcc"
	@echo "      compiler=clang - Clang C compiler"
	@echo "      compiler=pgi   - PGI pgcc. Available only for intel64."
	@echo "      compiler=intel - Intel(R) C compiler icc (DEFAULT)"
	@echo
	@echo "  interface=<name> selects kind of MKL_INT type for %intel64 targets:"
	@echo "      interface=lp64  - 32-bit integers (DEFAULT)"
	@echo "      interface=ilp64 - 64-bit integers"
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
	@echo "      Default value: all examples listed in file dftc.lst"
	@echo
	@echo "Additional options:"
	@echo "  RES_DIR=<path> defines where to place the results"
	@echo "      Default value: ./_results"
	@echo
	@echo "  MKLROOT=<path> defines alternative Intel(R) MKL root directory"
	@echo "      Default value: ../.."
	@echo
	@echo "  MKLRUNLIB_PATH=<path> defines alternative runtime library directory"
	@echo "      for dynamic linkage."
	@echo '      Default value: $$(MKLROOT)/lib'
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
## make libia32 function=basic_dp_complex_dft_1d
##     Build with Intel(R) C compiler and run this example
##     as a 32-bit application with static linkage
##
## make dylibia32 compiler=gnu
##     Build with GNU C compiler and run all examples as 32-bit applications
##     with dynamic linkage
##
## make libintel64 compiler=gnu
##     Build with GNU C compiler and run all examples as statically linked
##     applications for Intel(R) 64 processor family
##
## make dylibintel64
##     Build with Intel(R) C compiler and run all examples as
##     dynamically linked applications for Intel(R) 64 processor family
##-----------------------------------------------------------------------------

MY_MAKEFILE := $(MAKEFILE_LIST)

ifeq (,$(RUN)$(IA))
  target := $(firstword $(filter lib% dylib%,$(MAKECMDGOALS)))
else
  target = $(RUN)$(IA)
endif

MY_WAY := $(findstring target-specific,$(.FEATURES))

ifndef MKLROOT
  MKLROOT = ../..
endif

_lib_dir = lib

ifndef MKLRUNLIB_PATH
  MKLRUNLIB_PATH = $(MKLROOT)/$(_lib_dir)
endif

include dftc.lst

ifndef RES_DIR
  RES_DIR = _results
endif

ifndef function
  function = $(DFT)
endif

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

ifeq (,$(filter lp64 ilp64,$(interface)))
  override interface = lp64
endif
ifeq (,$(findstring 32,$(target)))
  iface = $(interface)_
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

ifeq (,$(filter pgmp,$(omp)))
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
ifeq (dylib,$(findstring dylib,$(MAKECMDGOALS)))
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
  COPTS += -Wall -Werror
  TARGET_ARCH.ia32 = -m32
  TARGET_ARCH.intel64 = -m64
  IFACE_COMP_PART = intel
else
  ifeq ($(compiler),pgi)
    CC = pgcc
    TARGET_ARCH.ia32 = -tp core2
    TARGET_ARCH.intel64 = -tp core2-64
  else
    CC = icc
    COPTS += -Wall -Werror
  endif
  IFACE_COMP_PART = intel
endif

# clang does not support omp pragmas
ifneq ($(gnu_or_clang),)
  function := $(filter-out config_number_of_user_threads config_thread_limit, $(function))
endif

ifeq ($(compiler)-$(parallel),pgi-pgi)
  IFACE_THREADING_PART = pgi
else
  IFACE_THREADING_PART = intel
endif

IFACE_LIB.ia32 = mkl_$(IFACE_COMP_PART)
ifeq ($(interface),ilp64)
  CPPFLAGS += -DMKL_ILP64
  IFACE_LIB.intel64 = mkl_$(IFACE_COMP_PART)_ilp64
else
  IFACE_LIB.intel64 = mkl_$(IFACE_COMP_PART)_lp64
endif
IFACE_LIB = $(IFACE_LIB.$(IA))

TARGET_ARCH = $(TARGET_ARCH.$(IA))

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
 
  ifeq ($(compiler)-$(IFACE_THREADING_PART),pgi-pgi)
   COPTS += -mp
   LDFLAGS += -mp
  endif

  ifeq ($(compiler),intel)
    COPTS += -qopenmp
    LDFLAGS += -L$(CMPLR_PATH) -liomp5
  endif

  ifeq ($(compiler),gnu)
    LDFLAGS += -L$(CMPLR_PATH) -liomp5
  endif
endif

CORE_LIB = mkl_core

ifeq ($(compiler)-$(IFACE_THREADING_PART),pgi-pgi)
 LDFLAGS += -pgf90libs
endif
  
ifeq ($(compiler),pgi)
  LDFLAGS += -Wl,-no_compact_unwind
endif
  
MKL_PATH = $(MKLROOT)/$(_lib_dir)
CMPLR_PATH = $(MKLROOT)/../compiler/lib

res_dir = $(RES_DIR)/$(compiler)_$(iface)$(threadname)_$(target)

ifdef VERBOSE
  $(info )
  $(info MAKELEVEL=$(MAKELEVEL) target=$(target))
  $(info MKLROOT=$(MKLROOT) RES_DIR=$(RES_DIR))
  $(info compiler=$(compiler) interface=$(interface) threading=$(threading) \
         parallel=$(parallel) omp=$(omp))
endif

##-----------------------------------------------------------------------------
## Rules

.PHONY: libia32 dylibia32 libintel64 dylibintel64 \
	lib32 dylib32 libem64t dylibem64t \
	help run mkdir clean report \
	FORCE

.SUFFIXES:
.SUFFIXES: .c .o .res

vpath %.c source

# just old targets for compatibility
lib32: libia32
dylib32: dylibia32
libem64t: libintel64
dylibem64t: dylibintel64

ifeq ($(MY_WAY),target-specific$(RECURSION))

lib%: RUN = lib
dylib%: RUN = dylib

%ia32: IA = ia32
%intel64: IA = intel64

RUNENV = DYLD_BIND_AT_LAUNCH=1

ifneq ($(LIB_PATH),)
lib%: RPATH = -Wl,-rpath,$(CMPLR_PATH)
lib%: RUNENV += DYLD_LIBRARY_PATH=$(LIB_PATH):$(DYLD_LIBRARY_PATH)
else
lib%: RPATH = -Wl,-rpath,$(CMPLR_PATH)
endif

lib%: MKL_LIBS = $(MKL_PATH)/lib$(IFACE_LIB).a $(MKL_PATH)/lib$(THREADING_LIB).a $(MKL_PATH)/lib$(CORE_LIB).a

ifneq ($(LIB_PATH),)
dylib%: RPATH = -Wl,-rpath,$(MKLRUNLIB_PATH) -Wl,-rpath,$(CMPLR_PATH)
dylib%: RUNENV += DYLD_LIBRARY_PATH=$(LIB_PATH):$(DYLD_LIBRARY_PATH)
else
dylib%: RPATH = -Wl,-rpath,$(MKLRUNLIB_PATH) -Wl,-rpath,$(CMPLR_PATH)
endif
ifneq ($(MKL_LIBS_LINK),mixed)
dylib%: MKL_LIBS = -L$(MKL_PATH) -lmkl_rt
dylib%: RUNENV += MKL_INTERFACE_LAYER=$(interface) MKL_THREADING_LAYER=$(threadlayer)
else
dylib%: MKL_LIBS = -L$(MKL_PATH) -l$(IFACE_LIB) -l$(THREADING_LIB) -l$(CORE_LIB)
endif

libia32 dylibia32 libintel64 dylibintel64: run

else # using recursive MAKE if target-specific targets are unsupported

ifeq ($(RUN),dylib)
  ifneq ($(LIB_PATH),)
    RPATH = -Wl,-rpath,$(MKLRUNLIB_PATH) -Wl,-rpath,$(CMPLR_PATH)
    RUNENV += DYLD_LIBRARY_PATH=$(LIB_PATH):$(DYLD_LIBRARY_PATH)
  else
    RPATH = -Wl,-rpath,$(MKLRUNLIB_PATH) -Wl,-rpath,$(CMPLR_PATH)
  endif
  ifneq ($(MKL_LIBS_LINK),mixed)
    MKL_LIBS = -L$(MKL_PATH) -lmkl_rt
    RUNENV += MKL_INTERFACE_LAYER=$(interface) MKL_THREADING_LAYER=$(threadlayer)
  else
    MKL_LIBS = -L$(MKL_PATH) -l$(IFACE_LIB) -l$(THREADING_LIB) -l$(CORE_LIB)
  endif
else
  ifneq ($(LIB_PATH),)
    RPATH = -Wl,-rpath,$(CMPLR_PATH)
    RUNENV += DYLD_LIBRARY_PATH=$(LIB_PATH):$(DYLD_LIBRARY_PATH)
  else
    RPATH = -Wl,-rpath,$(CMPLR_PATH)
  endif
  MKL_LIBS = $(MKL_PATH)/lib$(IFACE_LIB).a
  MKL_LIBS += $(MKL_PATH)/lib$(THREADING_LIB).a $(MKL_PATH)/lib$(CORE_LIB).a
endif

libia32:
	$(MAKE) -f $(MY_MAKEFILE) run IA=ia32 RUN=lib

dylibia32:
	$(MAKE) -f $(MY_MAKEFILE) run IA=ia32 RUN=dylib

libintel64:
	$(MAKE) -f $(MY_MAKEFILE) run IA=intel64 RUN=lib

dylibintel64:
	$(MAKE) -f $(MY_MAKEFILE) run IA=intel64 RUN=dylib

endif

run: mkdir $(RES)

mkdir: FORCE
	@echo
	@echo See results in $(res_dir)
	@echo
	mkdir -p $(res_dir)

$(res_dir)/%.o: %.c FORCE
	@echo
	# compile $*.c
	$(CC) -c $(COPTS) $(CPPFLAGS) $(CFLAGS) $(TARGET_ARCH) \
	  -I$(MKLROOT)/include $< -o $@

%.res: $(res_dir)/%.o
	@echo
	# create $*.out
	$(CC) $(LDFLAGS) $(TARGET_ARCH) \
	  $< \
	  $(MKL_LIBS) \
	  $(RPATH) \
	  $(OMP_LIB) -lpthread -lm -o $(res_dir)/$*.out
	-rm -f $(res_dir)/$@ # remove res-file
	@echo
	# run $*.out
	$(RUNENV) \
	  $(res_dir)/$*.out > $(res_dir)/$@

report: FORCE
	@echo
	@echo "--------------------------------------------------"
	@echo There are `ls source/*.c | wc -l` \
	  examples in source
	@echo
	@for D in `ls -d $(RES_DIR)/* 2>/dev/null`; do \
	  /bin/echo -n "In   $$D	Total run examples:" \
	    "`ls $$D/*.res 2>/dev/null | wc -l`"; \
	  /bin/echo -n "	and Passed: "; \
	  cat /dev/null $$D/*.res 2>/dev/null | grep -c PASSED | cat; \
	done
	@echo "--------------------------------------------------"
	@echo

clean: FORCE
	-rm -rf $(RES_DIR) # clean all results

FORCE: ;

##-----------------------------------------------------------------------------
