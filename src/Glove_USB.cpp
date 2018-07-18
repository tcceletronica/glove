/*
 * Glove_USB.cpp
 *
 *  Created on: 5 de jul de 2018
 *      Author: lucascesarfd
 */

#include "Glove_USB.h"

namespace glove {

Glove_USB::Glove_USB(string com, int baudrate) {

	Serial serial(com, baudrate, 8, 'N', 1);
	serial_ = &serial;
}

Glove_Ret Glove_USB::glove_usb_open(void)
{
	serial_->Open();
	return RETURN_OK;
}


Glove_Ret Glove_USB::glove_usb_read(string& data, int size)
{
	bool ret;

	for(;size>0;size--)
	{
		data.append((const char*)serial_->ReadChar(ret));
		if(ret == false)
			return USB_READ_DATA_ERROR;
	}

	return RETURN_OK;
}

Glove_Ret Glove_USB::glove_usb_write(string& data, int size)
{
	long n;
	n = serial_->Write((char *)data.c_str());
	if(size != n)
		return USB_WRITE_DATA_ERROR;
	return RETURN_OK;
}


Glove_Ret Glove_USB::glove_usb_close(void)
{
	serial_->Close();
	return RETURN_OK;
}

Glove_Ret Glove_USB::glove_usb_flush(void)
{
	serial_->Flush();
	return RETURN_OK;
}



Glove_USB::~Glove_USB() {
	serial_->Close();
}

} /* namespace glove */
