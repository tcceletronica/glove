/*
 * Serial.cpp
 *
 *  Created on: 17 de jul de 2018
 *      Author: lucascesarfd
 */
#if defined (__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

#else
#include "Serial_unix.h"

namespace glove {

char rxchar;
string port;
long baud;
long dsize;
char parity;
float stopbits;
long fd;//serial_fd

Serial::Serial()
{
	fd = -1;
	port = "/dev/ttyS0";
	SetBaudRate(9600);
	SetDataSize(8);
	SetParity('N');
	SetStopBits(1);
}

Serial::Serial(string Device, long BaudRate,long DataSize,char ParityType,float NStopBits)
{
	fd = -1;
	SetPort(Device);
	SetBaudRate(BaudRate);
	SetDataSize(DataSize);
	SetParity(ParityType);
	SetStopBits(NStopBits);
}

Serial::~Serial()
{
	Close();
}

termios Serial::getTermios(){

	if(fd == -1)
		printf("\n error while opening port \n");

	struct termios tty;
	memset(&tty, 0, sizeof(tty));

	// Get current settings (will be stored in termios structure)
	if(tcgetattr(fd, &tty) != 0)
	{
		printf("\n error while opening port \n");
	}

	return tty;
}

void Serial::SetPort(string Device) {
	port = Device;
}

string Serial::GetPort() {
	return port;
}

void Serial::SetDataSize(long nbits) {
	if ((nbits < 5) || (nbits > 8)) nbits = 8;
	dsize=nbits;
}

long Serial::GetDataSize() {
	return dsize;
}

void Serial::SetParity(char p) {
	if ((p != 'N') && (p != 'E') && (p != 'O')) {
		p = 'N';
	}
	parity = p;
}

char Serial::GetParity() {
	return parity;
}

void Serial::SetStopBits(float nbits) {
	if (nbits >= 2) stopbits = 2;
	else stopbits = 1;
}

float Serial::GetStopBits() {
	return stopbits;
}


//-----------------------------------------------------------------------------
long Serial::Open(void) {

	fd = open(port.c_str(), O_RDWR);
	if (fd == -1) {
		return -1;
	}

	termios settings = getTermios();

	//settings.c_iflag = 0;
	//settings.c_oflag = 0;

	settings.c_cflag     &=  ~PARENB;       	// No parity bit is added to the output characters
	settings.c_cflag     &=  ~CSTOPB;		// Only one stop-bit is used
	settings.c_cflag     &=  ~CSIZE;			// CSIZE is a mask for the number of bits per character
	settings.c_cflag     |=  CS8;			// Set to 8 bits per character
	settings.c_cflag     &=  ~CRTSCTS;       // Disable hardware flow control (RTS/CTS)
	settings.c_cflag     |=  CREAD | CLOCAL;     				// Turn on READ & ignore ctrl lines (CLOCAL = 1)

	settings.c_cc[VTIME] = 0;
	settings.c_cc[VMIN] = 0;

	cfsetospeed(&settings, baud);
	cfsetispeed(&settings, baud);

	settings.c_oflag     =   0;              // No remapping, no delays
	settings.c_oflag     &=  ~OPOST;			// Make raw

	settings.c_iflag     &= ~(IXON | IXOFF | IXANY);			// Turn off s/w flow ctrl
	settings.c_iflag 	&= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);


	// Canonical input is when read waits for EOL or EOF characters before returning. In non-canonical mode, the rate at which
	// read() returns is instead controlled by c_cc[VMIN] and c_cc[VTIME]
	settings.c_lflag		&= ~ICANON;								// Turn off canonical input, which is suitable for pass-through
	//settings.c_lflag & ~(ECHO);	// Configure echo depending on echo_ boolean
	settings.c_lflag		&= ~ECHOE;								// Turn off echo erase (echo erase only relevant if canonical input is active)
	settings.c_lflag		&= ~ECHONL;								//
	settings.c_lflag		&= ~ISIG;								// Disables recognition of INTR (interrupt), QUIT and SUSP (suspend) characters


	tcsetattr(fd, TCSANOW, &settings);

	return 0;
}

void Serial::Close() {
	if(IsOpened()) close(fd);
	fd=-1;
}

bool Serial::IsOpened()
{
	if(fd== (-1)) return false;
	else return true;
}

void Serial::SetBaudRate(long baudrate) {
	if (baudrate < 50) baud = B0;
	else if (baudrate < 75) baud = B50;
	else if (baudrate < 110) baud = B75;
	else if (baudrate < 134) baud = B110;
	else if (baudrate < 150) baud = B134;
	else if (baudrate < 200) baud = B150;
	else if (baudrate < 300) baud = B200;
	else if (baudrate < 600) baud = B300;
	else if (baudrate < 1200) baud = B600;
	else if (baudrate < 2400) baud = B1200;
	else if (baudrate < 4800) baud = B2400;
	else if (baudrate < 9600) baud = B4800;
	else if (baudrate < 19200) baud = B9600;
	else if (baudrate < 38400) baud = B19200;
	else if (baudrate < 57600) baud = B38400;
	else if (baudrate < 115200) baud = B57600;
	else if (baudrate < 230400) baud = B115200;
	else baud = B230400;
}

long Serial::GetBaudRate() {
	long baudrate=9600;
	if (baud < B50) baudrate = 0;
	else if (baud < B75) baudrate = 50;
	else if (baud < B110) baudrate = 75;
	else if (baud < B134) baudrate = 110;
	else if (baud < B150) baudrate = 134;
	else if (baud < B200) baudrate = 150;
	else if (baud < B300) baudrate = 200;
	else if (baud < B600) baudrate = 300;
	else if (baud < B1200) baudrate = 600;
	else if (baud < B2400) baudrate = 1200;
	else if (baud < B4800) baudrate = 2400;
	else if (baud < B9600) baudrate = 4800;
	else if (baud < B19200) baudrate =9600;
	else if (baud < B38400) baudrate = 19200;
	else if (baud < B57600) baudrate = 38400;
	else if (baud < B115200) baudrate = 57600;
	else if (baud < B230400) baudrate = 115200;
	else baudrate = 230400;
	return baudrate;
}

char Serial::ReadChar(bool& success)
{
	if(fd == 0)
		printf("Read() was called but file descriptor (fileDesc) was 0, indicating file has not been opened.");

	success=false;
	if (!IsOpened()) {return 0;	}
	success=read(fd, &rxchar, 1);
	if(success==1)
		return rxchar;
	else
		return -1;
}

bool Serial::Write(char *data)
{
	printf("dado a ser mostrado %s, descriptor %d", data, fd);
	if (!IsOpened()) {return false;	}
	long n = strlen(data);
	if (n < 0) n = 0;
	else if(n > 1024) n = 1024;
	return (write(fd, data, n)==n);
}

bool Serial::WriteChar(char ch)
{
	char s[2];
	s[0]=ch;
	s[1]=0;//null terminated
	return Write(s);
}

void Serial::Flush()
{
	tcflush(fd, TCIOFLUSH);
}
//------------------------------------------------------------------------------
bool Serial::SetRTS(bool value) {
	long RTS_flag = TIOCM_RTS;
	bool success=true;
	if (value) {
		if (ioctl(fd, TIOCMBIS, &RTS_flag) == -1) success=false;//Set RTS pin
	}
	else {
		if (ioctl(fd, TIOCMBIC, &RTS_flag) == -1) success=false;//Clear RTS pin
	}
	return success;
}
//------------------------------------------------------------------------------
bool Serial::SetDTR(bool value) {
	long DTR_flag = TIOCM_DTR;
	bool success=true;
	if (value) {
		if (ioctl(fd, TIOCMBIS, &DTR_flag) == -1) success=false;//Set RTS pin
	}
	else {
		if (ioctl(fd, TIOCMBIC, &DTR_flag) == -1) success=false;//Clear RTS pin
	}
	return success;
}
//------------------------------------------------------------------------------
bool Serial::GetCTS(bool& success) {
	success=true;
	long status;
	if(ioctl(fd, TIOCMGET, &status)== -1) success=false;
	return ((status & TIOCM_CTS) != 0);
}
//------------------------------------------------------------------------------
bool Serial::GetDSR(bool& success) {
	success=true;
	long status;
	if(ioctl(fd, TIOCMGET, &status)== -1) success=false;
	return ((status & TIOCM_DSR) != 0);
}
//------------------------------------------------------------------------------
bool Serial::GetRI(bool& success) {
	success=true;
	long status;
	if(ioctl(fd, TIOCMGET, &status)== -1) success=false;
	return ((status & TIOCM_RI) != 0);
}
//------------------------------------------------------------------------------
bool Serial::GetCD(bool& success) {
	success=true;
	long status;
	if(ioctl(fd, TIOCMGET, &status)== -1) success=false;
	return ((status & TIOCM_CD) != 0);
}


} /* namespace glove */

#endif
