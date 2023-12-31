/*
============================================================================
Name : Program17b.c
Author : Kuldip Nivruti Bhatale
Roll No : MT2023087
Description : Write a program to simulate online ticket reservation. Implement write lock
Write a program to open a file, store a ticket number and exit. Write a separate program, to
open the file, implement write lock, read the ticket number, increment the number and print
the new ticket number then close the file.
 
Date: 25th Aug, 2023.
============================================================================
*/



#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

struct data{
	int ticket;
};
int main(){
	
        int fd = open("ticketInfo.txt", O_RDWR);
        if(fd==-1){
                printf("Failed to open the file");
        }

	struct flock lock;
	
   	lock.l_type = F_WRLCK;
   	lock.l_whence = SEEK_SET;
   	lock.l_start = 0;
   	lock.l_len = 0;
	lock.l_pid = getpid();

	fcntl(fd,F_SETLKW,&lock);
        struct data ticket_info;
        read(fd, &ticket_info, sizeof(ticket_info));

	printf("Value of ticket before: %d\n",ticket_info.ticket);
        ticket_info.ticket++;

	lseek(fd, 0L, SEEK_SET);	

	printf("Value of ticket after: %d\n",ticket_info.ticket);
	write(fd, &ticket_info, sizeof(ticket_info));
	
	getchar();

	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	printf("done\n");
   	
        close(fd);

        return 0;
}

