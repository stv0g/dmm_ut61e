/*
 * SerialPort.cpp
 *
 *  Created on: 14.11.2009
 *      Author: steffen
 */

#include "SerialPort.h"
#include <exception>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <strings.h>
#include <stdio.h>

SerialPort::SerialPort(char port[]) {
	fd = open(port, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		perror("Unable to open port ");
	}

	tcgetattr(fd, &oldtio); /* save current serial port settings */
	bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

	newtio.c_cflag = B19200 | CS7 | PARENB | PARODD | CSTOPB | CREAD | CLOCAL;
	newtio.c_iflag = 0;
	newtio.c_oflag = 0;
	newtio.c_lflag = ICANON;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

	int line_bits;
	ioctl(fd, TIOCMGET, &line_bits);
	line_bits |= TIOCM_DTR;
	line_bits &= ~TIOCM_RTS;
	ioctl(fd, TIOCMSET, &line_bits);
}

SerialPort::~SerialPort() {
	/* restore the old port settings */
	tcsetattr(fd, TCSANOW, &oldtio);
}
