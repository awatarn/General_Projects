icc -m32  -w -mia32 -I"/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/include" \
	pardiso_unsym_function.c \
	 "/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/lib/libmkl_intel.a" \
	"/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/lib/libmkl_intel_thread.a" \
	"/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/lib/libmkl_core.a" \
	-Wl,-rpath,/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/lib -Wl,-rpath,/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/../compiler/lib -Wl,-rpath,/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/../tbb/lib \
	-L"/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/../compiler/lib" -liomp5 -lpthread -lm -o a.out
