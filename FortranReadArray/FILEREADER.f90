program FILEREADER
!
!  This program reads data from array.txt
!  Ref: http://stackoverflow.com/questions/8828377/
!  reading-data-from-txt-file-in-fortran
!
   real, dimension(:,:), allocatable :: x
   integer :: n,m

   open (unit=99, file='array.txt', status='old', action='read')
   read(99, *) n
   read(99, *) m
   allocate(x(n,m))

   do I=1,n,1
      read(99,*) x(I,:)
      write(*,*) x(I,:)
   enddo

end
