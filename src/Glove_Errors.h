/*
 * Glove_Errors.h
 *
 *  Created on: 5 de jul de 2018
 *      Author: lucascesarfd
 */

#ifndef SRC_GLOVE_ERRORS_H_
#define SRC_GLOVE_ERRORS_H_

typedef enum Glove_Ret
{
	RETURN_OK=0,
	RETURN_ERROR,

	USB_ERROR=100,
	USB_PORT_NOT_OPEN,
	USB_COMM_ERROR,
	USB_NO_DATA,
	USB_TIMEOUT,
	USB_READ_DATA_ERROR,
	USB_WRITE_DATA_ERROR,

	DECODE_CORRUPTED_DATA=200,

	DATA_OUT_OF_BOUNDS=300

}Glove_Ret;



#endif /* SRC_GLOVE_ERRORS_H_ */
