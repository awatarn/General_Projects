icc -m32  -w -mia32 -I"/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/include" \
	./source/pardiso_unsym_c2.c \
	 "/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/lib/libmkl_intel.a" \
	"/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/lib/libmkl_intel_thread.a" \
	"/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/lib/libmkl_core.a" \
	-Wl,-rpath,/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/lib -Wl,-rpath,/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/../compiler/lib -Wl,-rpath,/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/../tbb/lib \
	-L"/opt/intel/compilers_and_libraries_2017.4.181/mac/mkl/../compiler/lib" -liomp5 -lpthread -lm -o _results/intel_omp_ia32_lib/pardiso_unsym_c2.out
