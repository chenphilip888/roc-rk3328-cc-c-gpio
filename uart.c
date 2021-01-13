/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

char *readLine(int fd) {
    static unsigned char line[256];
    int size;
    int i;
    char *ptr = line;
    while(1) {
	size = read(fd, (unsigned char *)ptr, 255);
	while (size > 0) {
	    if (*ptr == '\n') {
		ptr++;
	        *ptr = '\0';
	        return line;
	    }
            ptr++;
	    size--;
	}
    }
}

int uart_test( void ) {
    struct termios tty;
    int k;
    int fd;
 
    fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY );
    if (fd == -1) {
	perror("open_port: Unable to open /dev/ttyS1 - \n");
	return(-1);
    }
    fcntl(fd, F_SETFL, 0);
    tcgetattr(fd, &tty);

    if(cfsetispeed(&tty, B115200) < 0)
    {
        printf("invalid baud rate\n");
        return EXIT_FAILURE;
    }
    tty.c_cflag &= ~(CSIZE | PARENB);
    tty.c_cflag |= (CLOCAL | CREAD | CS8);
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
 
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        fprintf (stderr, "error %d from tcsetattr", errno);
        return -1;
    }

    tcflush(fd, TCIFLUSH);

    for ( k = '0'; k <= '~'; k++ ) {
	char i[3] = "9\r\n";
	i[0] = k;
        write(fd, &i[0], sizeof i);
	char *j = readLine(fd);
        printf("%s", j);
    }
    printf("\n");
    close(fd);
    return 0;
}
