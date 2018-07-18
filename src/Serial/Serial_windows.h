/*
 * Serial.h
 *
 *  Created on: 17 de jul de 2018
 *      Author: lucascesarfd
 */

#if defined (__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

#ifndef SRC_SERIAL_WINDOWS_H_
#define SRC_SERIAL_WINDOWS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;

#include <windows.h>
#define READ_TIMEOUT 10      // milliseconds

namespace glove {

inline void delay(unsigned long ms)
{
	Sleep(ms);
}

class Serial {
	char rxchar;
	string port;
	long baud;
	long dsize;
	char parity;
	float stopbits;
	HANDLE hComm; //handle
	OVERLAPPED osReader;
	OVERLAPPED osWrite;
	BOOL fWaitingOnRead;
	COMMTIMEOUTS timeouts_ori;

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
