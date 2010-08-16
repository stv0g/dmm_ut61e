/*
 * UT61E.h
 *
 *  Created on: 12.11.2009
 *      Author: steffen
 */

#ifndef UT61E_H_
#define UT61E_H_


class UT61E {
public:
	UT61E();
	virtual ~UT61E();
	bool check(char * data);
	void parse(char * data);
	const char* getMode();
	const char* getPower();
	const char* getRange();

	double value, max, min, avarage;
	long sample;
	char data[14];

	bool hold, rel, bat;

	enum { VOLTAGE, CURENT, RESISTANCE, DIODE, FREQUENCY, CAPACITANCE, INDUCTANCE, CONDUCTANCE } mode, lastmode;
	enum { DUTY, FREQUENCE } fmode;
	enum { AC, DC } power;
	enum { AUTO, MANUAL } range;
	enum { OVERLOAD, NORMAL, UNDERLOAD } load;
	enum { MAX, MIN } peak;

private:
	static const char* modelbl[];
	static const char* fmodelbl[];
	static const char*  powerlbl[];
	static const char* rangelbl[];
	static const char* loadlbl[];
	static const char* peaklbl[];
};

#endif /* UT61E_H_ */
