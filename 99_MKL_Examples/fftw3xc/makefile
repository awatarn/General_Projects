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
##      Build and run examples using FFTW3 C wrappers to Intel MKL.
##*****************************************************************************

## Usage examples:
##
## make libia32 compiler=gnu
##        Build and run all examples using Intel(R) MKL for 32-bit applications
##        and GNU C compiler.
##
## make libintel64 function=complex_1d_double_ex1 threading=sequential
##        Build and run one example using Intel(R) MKL for Intel(R) 64
##        processor family applications with sequential layer.

help:
	@echo "Usage: make {libia32|dylibia32|libintel64|dylibintel64} [option...]"
	@echo
	@echo "Options:"
	@echo "  function=<name>"
	@echo "        Run only the specified example. Please see file"
	@echo "        fftw3xc.lst for the list of functions."
	@echo
	@echo "  compiler=gnu|clang|pgi|intel"
	@echo "        Build examples using GNU gcc, Clang, PGI pgcc (only for intel64), or"
	@echo "        Intel(R) C compiler icc."
	@echo "        Default value: intel"
	@echo
	@echo "  interface=ilp64|lp64"
	@echo "        For Intel(R) 64 only, use ILP64 Intel(R) MKL interface layer"
	@echo "        when MKL_INT is MKL_INT64"
	@echo "        Default value: lp64"
	@echo
	@echo "  threading=sequential|parallel"
	@echo "        Build examples using parallel or sequential MKL."
	@echo "        Default value: parallel"
	@echo
	@echo "  parallel=pgi|intel"
	@echo "        For threading=parallel, select Intel(R) MKL threading layer."
	@echo "        Default value depends on <compiler>"
	@echo
	@echo "  omp=pgmp|iomp5"
	@echo "        For threading=parallel, select system OpenMP library:"
	@echo "          iomp5 (if compiler=intel)"
	@echo "          pgmp  (if compiler=pgi)"
	@echo "        Default value depends on <parallel>"
	@echo
	@echo "  wraplib=yes|no"
	@echo "        Build and use standalone FFTW3 wrappers library."
	@echo "        The library will be built with respective C compiler"
	@echo '        and put into $$(INSTALL_DIR)/wrap_lib_$$(IA)'
	@echo "        Default value: no (that is, use integrated wrappers)"
	@echo
	@echo "Additional macros:"
	@echo "  RES_DIR=<path>"
	@echo "        Use <path> for building the examples."
	@echo "        Default value: ./_results"
	@echo
	@echo "  MKLROOT=<path>"
	@echo "        Path to Intel(R) MKL root directory with header files and libraries."
	@echo "        Default value: ../.."
	@echo
	@echo "  INSTALL_DIR=<path>"
	@echo "        Use <path> for installing of wraplib, if wraplib=yes"
	@echo "        Default value: . (that is, use current work directory)"
	@echo
	@echo "  MKLRUNLIB_PATH - Intel(R) MKL lib directory to be used to run examples"
	@echo "        in case of dynamic linkage"
	@echo "        Default value is current Intel(R) MKL libs located at"
	@echo '          $$(MKLROOT)/lib'
	@echo
	@echo "  LIB_PATH - If you are using pgi threading layer,"
	@echo "        specify path to libraries: libpgmp"
	@echo "        or add needed path to DYLD_LIBRARY_PATH"
	@echo
	@echo "N.B. To run examples on some specific target architecture"
	@echo "  add needed options to make-arguments: TARGET_ARCH=..."
	@echo "  and refer to GNU, Clang, PGI and Intel compilers documentation"
	@echo "  for more details about target architecture specific options"
	@echo

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

include fftw3xc.lst

function = $(DFT)
compiler = intel
interface = lp64
threading = parallel
wraplib = no

ifeq ($(compiler)_$(_IA),pgi_ia32)
   $(info ***)
   $(info *** PGI compiler 32-bit support was dropped off starting from Intel(R) MKL 2017.)
   $(info *** Please try >make help)
   $(info ***)
   $(error )
endif

wraplibdir = $(INSTALL_DIR)/wrap_lib$(_ilp64)_$(_IA)

##-----------------------------------------------------------------------------
## Main targets

.PHONY: lib32 libia32 dylib32 dylibia32 \
        libem64t libintel64 dylibem64t dylibintel64 \
        report clean cleanup FORCE

libia32 lib32:
	$(MAKE) -f $(MY_MAKEFILE) run _IA=ia32 _LIB=lib

libintel64 libem64t:
	$(MAKE) -f $(MY_MAKEFILE) run _IA=intel64 _LIB=lib

dylibia32 dylib32:
	$(MAKE) -f $(MY_MAKEFILE) run _IA=ia32 _LIB=dylib

dylibintel64 dylibem64t:
	$(MAKE) -f $(MY_MAKEFILE) run _IA=intel64 _LIB=dylib

report: FORCE
	@echo
	@echo "--------------------------------------------------"
	@echo There are `ls source/*.c | wc -l` \
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
	-rm -rf $(INSTALL_DIR)/wrap_lib* # clean all built wrap_libs

FORCE: ;

ifdef VERBOSE
  $(info )
  $(info compiler=$(compiler) interface=$(interface) threading=$(threading) \
         parallel=$(parallel) omp=$(omp) wraplib=$(wraplib))
  $(info )
endif

ifdef _IA
##-----------------------------------------------------------------------------
## Supporting _macros

_CC_intel = icc
_CC_gnu = gcc
_CC_clang = clang
_CC_pgi = pgcc
CC = $(or $(_CC_$(compiler)),icc)

ifneq ($(_IA),ia32)
  _ilp64 = _$(interface)
endif

# Define _cflags
_cflags =
_cflags += $(_cflags_$(compiler))
_cflags += $(_cflags_$(compiler)_$(_IA))
_cflags += $(_cflags_$(compiler)_$(interface))
_cflags += $(_cflags_$(compiler)_$(parallel))
_cflags += $(_cflags_$(compiler)_diag)
_cflags += $(CFLAGS) $(TARGET_ARCH)

_cflags_intel =
_cflags_intel_diag = -Wall -Werror

_cflags_pgi = -Mnokeepobj -Wl,-no_compact_unwind
_cflags_pgi_pgi = -mp -pgf90libs
_cflags_pgi_ia32 = -tp core2
_cflags_pgi_intel64 = -tp core2-64
_cflags_pgi_diag = 

_cflags_gnu_ia32 = -m32
_cflags_gnu_intel64 = -m64
_cflags_gnu_diag = -Wall -Werror

_cflags_clang_ia32 = -m32
_cflags_clang_intel64 = -m64
_cflags_clang_diag = -Wall -Werror

# Define preprocessor flags
_cppflags = -I$(MKLROOT)/include -I$(MKLROOT)/include/fftw $(CPPFLAGS)

# Maybe define wraplib
ifeq ($(wraplib),yes)
  _maybe_wrap = _with_wraplib
  _maybe_wraplib = $(wraplibdir)/libfftw3xc.a
  _maybe_wraplib_libs = -L$(wraplibdir) -lfftw3xc
  _maybe_wraplib_makeflags_ilp64 = CPPFLAGS=-DMKL_ILP64
endif

# Pick Intel(R) MKL interface layer
_lib_iface = mkl_intel$(_ilp64)

# Pick Intel(R) MKL threading layer
ifeq ($(threading),parallel)
  parallel = $(_parallel_$(compiler))
  omp = $(_omp_$(parallel))

  _parallel_intel = intel
  _parallel_gnu = intel# gnu missing
  _parallel_clang = intel
  _parallel_pgi = pgi

  _omp_intel = iomp5
  _omp_gnu = iomp5# gomp missing
  _omp_clang = iomp5
  _omp_pgi = pgmp

  _lib_thread = mkl_$(parallel)_thread
  threadlayer = $(parallel)
else
  _lib_thread = mkl_sequential
  threadlayer = $(threading)
endif

ifeq ($(compiler)-$(parallel)-$(omp),intel--tbb)
  override _lib_thread = mkl_sequential
endif
# Pick Intel(R) MKL core layer
_lib_core = mkl_core

_threading = $(threading)
# Maybe pick OpenMP library
ifeq ($(threading),parallel)
  ifneq ($(omp),)
    _maybe_omp = -L$(CMPLR_PATH) -L$(TBB_PATH) -l$(omp)
    _threading = $(threading)_$(omp)
  endif
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

CMPLR_PATH = $(MKLROOT)/../compiler/lib
TBB_PATH = $(MKLROOT)/../tbb/lib

# Finally compose the _libs
_libs = $(_maybe_wraplib_libs)
ifeq ($(_LIB),dylib)
  _libs += -L$(MKLROOT)/$(_lib_dir)
  ifneq ($(MKL_LIBS_LINK),mixed)
    _libs += -lmkl_rt
  else
    _libs += -l$(_lib_iface) -l$(_lib_thread) -l$(_lib_core)
  endif
else
  _libs += $(MKLROOT)/$(_lib_dir)/lib$(_lib_iface).a
  _libs += $(MKLROOT)/$(_lib_dir)/lib$(_lib_thread).a
  _libs += $(MKLROOT)/$(_lib_dir)/lib$(_lib_core).a
endif
_libs += $(_maybe_omp)
_libs += -lm -lpthread

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

_resdir = $(RES_DIR)/$(compiler)$(_ilp64)_$(_threading)_$(_LIB)$(_IA)$(_maybe_wrap)

##-----------------------------------------------------------------------------
## Rules

vpath %.c source

.SUFFIXES:
.SUFFIXES: .c .o .res .out

.PHONY: run mkresdir wraplib

run: wraplib mkresdir $(function:%=$(_resdir)/%.res)

.PRECIOUS: $(_resdir)/%.out

$(_resdir)/%.res: $(_resdir)/%.out FORCE
	-rm -f $@ # remove $@ file before run
	@echo
	# run $*.out
	$(RUNENV) \
	  $< > $@

$(_resdir)/%.out: %.c
	@echo
	# compile $*.c
	$(CC) $(_cflags) $(_cppflags) $^ $(RPATH) $(_libs) -o $@

mkresdir:
	@echo
	@echo See results in $(_resdir)
	@echo
	test -d $(_resdir) || mkdir -p $(_resdir)

wraplib: $(_maybe_wraplib)

ifeq ($(wraplib),yes)
# We shall use absolute path for install_to
_install_to=$(shell mkdir -p $(wraplibdir); cd $(wraplibdir) && pwd)
$(_maybe_wraplib):
	@echo
	# build $@
	$(MAKE) -C $(MKLROOT)/interfaces/fftw3xc lib$(_IA) \
	  INSTALL_DIR=$(_install_to) INSTALL_LIBNAME=$(@F) \
	  $(_maybe_wraplib_makeflags$(_ilp64))
endif

##-----------------------------------------------------------------------------
endif
