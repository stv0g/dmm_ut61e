/*
 * SerialPort.h
 *
 *  Created on: 14.11.2009
 *      Author: steffen
 */

#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include <termios.h>

class SerialPort {
public:
	SerialPort(char port[]);
	virtual ~SerialPort();

	int fd;
private:
	struct termios oldtio, newtio;
};

#endif /* SERIALPORT_H_ */
