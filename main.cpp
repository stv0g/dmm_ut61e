#include "UT61E.h"
#include "SerialPort.h"
#include <iostream>
#include <cstdlib>

void usage(void) {
    std::cout << "Usage: dmmut61e <serialport>" << std::endl;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
        	usage();
        	exit(0);
	}	

	UT61E * dmm = new UT61E();
	SerialPort * port = new SerialPort(argv[1]);

	int res;
	char buf[14];

	while (true) {
		res = read(port->fd, buf, 14);

		if (dmm->check(buf) && res == 14) {
			dmm->parse(buf);

			if (!dmm->hold)
				std::cout << dmm->sample << ";" << dmm->value << ";" << dmm->max << ";" << dmm->min << ";" << dmm->avarage << ";" << dmm->getMode() << ";" << dmm->getPower() << ";" << dmm->getRange() << std::endl;

		} else {
			std::cerr << "Invalid data!" << std::endl;
		}

	}


}
