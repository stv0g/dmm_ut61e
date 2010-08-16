/*
 * UT61E.cpp
 *
 *  Created on: 12.11.2009
 *      Author: steffen
 */

#include "UT61E.h"
#include <exception>
#include <cstdlib>

const char* UT61E::modelbl[] = { "V", "A", "Ohm", "-|>-", "Hz", "F", "H", "-/ -" };
const char* UT61E::fmodelbl[] = { "Duty" , "Frequence" };
const char* UT61E::powerlbl[] = {"AC", "DC" };
const char* UT61E::rangelbl[] = { "Auto", "Manual" };
const char* UT61E::loadlbl[] = { "Overload", "Normal", "Underload" };
const char* UT61E::peaklbl[] = { "Maximum", "Minimum" };

UT61E::UT61E() {
	mode = VOLTAGE;
}

UT61E::~UT61E() {}

const char* UT61E::getMode() {
	return UT61E::modelbl[mode];
}

const char* UT61E::getPower() {
	return UT61E::powerlbl[power];
}

const char* UT61E::getRange() {
	return UT61E::rangelbl[range];
}

bool UT61E::check(char * data) {
	if ((data[0] & 0x30) == 0x30 && data[12] == 0x0d && data[13] == 0x0a) {
		return true;
	} else {
		return false;
	}
}

void UT61E::parse(char * data) {
	char digits[] = { data[1], data[2], data[3], data[4], data[5] };
	value = atof(digits);

	lastmode = mode;

	bat = (data[7] & 2) ? true : false;
	rel = (data[8] & 2) ? true : false;
	hold = (data[11] & 2) ? true : false;

	if (data[7] & 0x04)
		value *= -1;

	if (data[10] & 8)
		power = DC;
	else if (data[10] & 4)
		power = AC;

	if (data[10] & 2)
		range = AUTO;
	else
		range = MANUAL;

	if (data[7] & 1)
		load = OVERLOAD;
	else if (data[9] & 8)
		load = UNDERLOAD;
	else
		load = NORMAL;

	if (data[9] & 4)
		peak = MAX;
	else if (data[9] & 2)
		peak = MIN;

	if (data[10] & 1)
		fmode = FREQUENCE;

	if (data[7] & 8)
		fmode = DUTY;

	double multp = 1;
	switch (data[6]) {
	case '1':
		mode = DIODE;
		break;

	case '2':
		mode = FREQUENCY;

		switch (data[0]) {
		case '0':
			multp = 1e-2;
			break;
		case '1':
			multp = 1e-1;
			break;
		case '3':
			multp = 1;
			break;
		case '4':
			multp = 1e1;
			break;
		case '5':
			multp = 1e2;
			break;
		case '6':
			multp = 1e3;
			break;
		case '7':
			multp = 1e4;
			break;
		default:
			throw std::exception();
		}
		break;

	case '3':
		mode = RESISTANCE;

		switch (data[0]) {
		case '0':
			multp = 1e-2;
			break;
		case '1':
			multp = 1e-1;
			break;
		case '2':
			multp = 1;
			break;
		case '3':
			multp = 1e1;
			break;
		case '4':
			multp = 1e2;
			break;
		case '5':
			multp = 1e3;
			break;
		case '6':
			multp = 1e4;
			break;
		default:
			throw std::exception();
		}
		break;

	case '5':
		mode = CONDUCTANCE;
		break;

	case '6':
		mode = CAPACITANCE;

		switch (data[0]) {
		case '0':
			multp = 1e-12;
			break;
		case '1':
			multp = 1e-11;
			break;
		case '2':
			multp = 1e-10;
			break;
		case '3':
			multp = 1e-9;
			break;
		case '4':
			multp = 1e-8;
			break;
		case '5':
			multp = 1e-7;
			break;
		case '6':
			multp = 1e-6;
			break;
		case '7':
			multp = 1e-5;
			break;
		default:
			throw std::exception();
		}
		break;

	case 0x3b: // V
		mode = VOLTAGE;

		switch (data[0]) {
		case '0':
			multp = 1e-4;
			break;
		case '1':
			multp = 1e-3;
			break;
		case '2':
			multp = 1e-2;
			break;
		case '3':
			multp = 1e-1;
			break;
		case '4':
			multp = 1e-5;
			break;
		default:
			throw std::exception();
		}
		break;

	case '0': // A
		mode = CURENT;
		if (data[0] == '0')
			multp = 1e-3;
		else {
			throw std::exception();
		}
		break;

	case 0x3d: // uA
		mode = CURENT;

		switch (data[0]) {
		case '0':
			multp = 1e-8;
			break;
		case '1':
			multp = 1e-7;
			break;
		default:
			throw std::exception();
		}

		break;
	case 0x3f: // mA
		mode = CURENT;

		switch (data[0]) {
		case '0':
			multp = 1e-6;
			break;
		case '1':
			multp = 1e-5;
			break;
		default:
			throw std::exception();
		}

		break;
	default:
		throw std::exception();
	}

	value *= multp;

	if (mode != lastmode) {
		max = 0;
		min = 0;
		sample = 0;
		avarage = value;
	}

	if (value > max)
		max = value;

	if (value < min)
		min = value;

	avarage = (sample * avarage + value) / ++sample;
}
