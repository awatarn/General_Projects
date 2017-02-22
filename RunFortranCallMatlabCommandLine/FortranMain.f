C
      program FortranMain
C
      implicit none
      integer :: ierr, i
C
      write(*,*) 'hello world'      
      do i=1,5
         call execute_command_line('./UnixTestScript.sh',exitstat=ierr)
         call sleep(5)
      enddo
      end program FortranMain      
      
