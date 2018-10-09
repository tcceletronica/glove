/*
 * Glove_Comm.h
 *
 *  Created on: 5 de jul de 2018
 *      Author: lucascesarfd
 */

#ifndef SRC_GLOVE_COMM_H_
#define SRC_GLOVE_COMM_H_

#include <stdlib.h>

#include "Glove_USB.h"
#include "Glove_Errors.h"

//#define SOH '\x01'
//#define EOT '\x04'
//#define ACK '\x06'
//#define NAK '\x15'
//#define STX '\x02'
//#define ETX '\x03'

#define SOH '#'
#define STX '$'
#define ETX '&'
#define DELIMITER ';'

namespace glove {

typedef struct Pose
{
	int x;
	int y;
	int z;
}Pose;

typedef struct Rot
{
	int roll;
	int pitch;
	int yaw;
}Rot;

typedef struct Fingers
{
	int fng1;
	int fng2;
	int fng3;
	int fng4;
	int fng5;
}Fingers;

class Glove_Comm {

public:
	Glove_Comm(string comm_port);
	virtual ~Glove_Comm();

	/*!
	 * @brief Function to get the glove position in meters.
	 *
	 * @param x (int) X position in meters.
	 * @param y (int) Y position in meters.
	 * @param z (int) Z position in meters.
	 * @return See #Glove_Ret
	 */
	Glove_Ret glove_get_position(int* x, int* y, int* z);

	/*!
	 * @brief Function to get the glove angular position in degrees.
	 *
	 * @param roll (int) X reference angular position in degrees.
	 * @param pitch (int) Y reference angular position in degrees.
	 * @param yaw (int) Z reference angular position in degrees.
	 * @return See #Glove_Ret
	 */
	Glove_Ret glove_get_accel(int* roll, int* pitch, int* yaw);

	/*!
	 * @brief Function to get the relative finger flexing.
	 *
	 * @param fng1 (int) Index finger flexion (0-100).
	 * @param fng2 (int) Middle finger flexion (0-100).
	 * @param fng3 (int) Ring finger flexion (0-100).
	 * @param fng4 (int) Baby finger flexion (0-100).
	 * @param fng5 (int) Thumb flexion (0-100).
	 * @return See #Glove_Ret
	 */
	Glove_Ret glove_get_flex(int* fng1, int* fng2, int* fng3, int* fng4, int* fng5);


	/*!
	 * @brief Function to activate the stimuli on the glove.
	 *
	 * @param state (int) 100 for on. 0 for off.
	 * @return See #Glove_Ret
	 */
	Glove_Ret glove_send_stim(int state);


	Glove_Ret glove_set_motors(int fng1, int fng2, int fng3, int fng4, int fng5);

	Glove_Ret glove_package_encode(string ID, void * content, string & payload);

	Glove_Ret glove_startup();

	Glove_Ret glove_package_receive(string ID, void * content);
	Glove_Ret glove_package_send(string ID, string Payload, int size);
	Glove_Ret glove_package_decode(string data, string ID, void * content);

private:

	Glove_USB * device_;

};

} /* namespace glove */

#endif /* SRC_GLOVE_COMM_H_ */
