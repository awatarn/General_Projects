      program hello_word
      use mpi
      integer ierr   
      
      call MPI_INIT(ierr)
      print *, "Hello world"
      call MPI_FINALIZE(ierr)

      stop
      end
