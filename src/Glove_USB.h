/*
 * Glove_USB.h
 *
 *  Created on: 5 de jul de 2018
 *      Author: lucascesarfd
 */

#ifndef SRC_GLOVE_USB_H_
#define SRC_GLOVE_USB_H_

#include "Glove_Errors.h"
#include <iostream>
#include <libusbp.hpp>
#include "Serial/Serial_windows.h"
#include "Serial/Serial_unix.h"

namespace glove {

class Glove_USB {
public:
	Glove_USB(string com, int baudrate);
	virtual ~Glove_USB();
	Glove_Ret glove_usb_read(string& data, int size);
	Glove_Ret glove_usb_write(string& data, int size);
	Glove_Ret glove_usb_open(void);
	Glove_Ret glove_usb_close(void);
	Glove_Ret glove_usb_flush(void);

private:
	Serial *serial_;
};

} /* namespace glove */

#endif /* SRC_GLOVE_USB_H_ */
