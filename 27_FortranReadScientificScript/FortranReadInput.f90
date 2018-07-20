! This program demonstrates reading data from a text file.
! Ref: https://jblevins.org/log/control-file
! Author: Apiwat Wisitsorasak
! Last update: 21 July 2018

program FortranReadInput

    implicit none

    ! Internal variables
    integer, parameter :: devicenumber = 15
    integer            :: ios = 0, line = 0, pos
    character(len=100) :: buffer, label
    character(len=100) :: command_arg    

    ! Input-file variables
    real :: pi
    integer, dimension(5) :: vector

    ! Getting filename from a command line argument
    if (command_argument_count().ge.1) then
        call get_command_argument(1, command_arg)
        write(*,*) 'Input filename: ',command_arg
    end if

    ! Reading file
    
    open(devicenumber, file=command_arg)

    do while (ios.eq.0)
        read(devicenumber, '(A)', iostat=ios) buffer
        if (ios.eq.0) then
            line = line + 1
            pos = scan(buffer, ' ')
            label = buffer(1:pos)
            buffer = buffer(pos+1:)

            select case (label)
            case('pi')
                read(buffer, *, iostat=ios) pi
                write(*,'(A15,F14.4)') 'Read pi    : ',pi
            case('vector')
                read(buffer, *, iostat=ios) vector
                write(*,'(A15,5I5)') 'Read vector: ',vector
            case default
                write(*,*) 'Skipping invalid label at line: ', line
            end select
        end if
    end do
end program FortranReadInput
