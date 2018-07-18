/*
 * Serial.h
 *
 *  Created on: 17 de jul de 2018
 *      Author: lucascesarfd
 */

#if defined (__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

#else

#ifndef SRC_SERIAL_UNIX_H_
#define SRC_SERIAL_UNIX_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

using namespace std;

namespace glove {

inline void delay(unsigned long ms)
{
	usleep(ms*1000);
}

class Serial {
	char rxchar;
	string port;
	long baud;
	long dsize;
	char parity;
	float stopbits;
	long fd;//serial_fd
public:
	Serial();
	Serial(string Device, long BaudRate, long DataSize, char ParityType, float NStopBits);
	~Serial();
	long Open(void);//return 0 if success
	void Close();
	char ReadChar(bool& success);//return read char if success
	bool WriteChar(char ch);////return success flag
	bool Write(char *data);//write null terminated string and return success flag
	void Flush(void);
	bool SetRTS(bool value);//return success flag
	bool SetDTR(bool value);//return success flag
	bool GetCTS(bool& success);
	bool GetDSR(bool& success);
	bool GetRI(bool& success);
	bool GetCD(bool& success);
	bool IsOpened();
	void SetPort(string Port);
	string GetPort();
	void SetBaudRate(long baudrate);
	long GetBaudRate();
	void SetDataSize(long nbits);
	long GetDataSize();
	void SetParity(char p);
	char GetParity();
	void SetStopBits(float nbits);
	float GetStopBits();
};


} /* namespace glove */
#endif

#endif /* SRC_SERIAL_SERIAL_UNIX_H_ */
