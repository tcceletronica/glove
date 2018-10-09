/*
 * Glove_Comm.cpp
 *
 *  Created on: 5 de jul de 2018
 *      Author: lucascesarfd
 */

#include "Glove_Comm.h"

namespace glove {

Glove_Comm::Glove_Comm(string comm_port)
{
	device_ = new Glove_USB(comm_port, 115200);

	Glove_Ret ret = device_->glove_usb_open();

	if (ret)
	{
		cout << "bu";
	}
}

Glove_Ret Glove_Comm::glove_package_send(string ID, string Payload, int size)
{
	string package = "\0";
	char str_size[16];
	sprintf(str_size, "%d",size);

	package.insert(package.begin(),1,SOH);
	package.append(ID);
	package.append(str_size);
	package.insert(package.end(),1,STX);
	package.append(Payload);
	package.insert(package.end(),1,ETX);

	return device_->glove_usb_write(package, (int)package.size());
}

Glove_Ret Glove_Comm::glove_package_decode(string data, string ID, void * content)
{
	char* pch;
	if(!ID.compare("POS"))
	{
		Pose * position = (Pose *) content;
		pch = strtok((char *)data.c_str(),";");
		position->x = atoi(pch);
		pch = strtok(NULL,";");
		position->y = atoi(pch);
		pch = strtok(NULL,";");
		position->z = atoi(pch);

		return RETURN_OK;

	}
	if(!ID.compare("ANG"))
	{
		Rot * rotation = (Rot *) content;
		pch = strtok((char *)data.c_str(),";");
		rotation->roll = atoi(pch);
		pch = strtok(NULL,";");
		rotation->pitch = atoi(pch);
		pch = strtok(NULL,";");
		rotation->yaw = atoi(pch);

		return RETURN_OK;

	}
	if(!ID.compare("FNG"))
	{
		Fingers * hand = (Fingers *) content;
		pch = strtok((char *)data.c_str(),";");
		hand->fng1 = atoi(pch);
		pch = strtok(NULL,";");
		hand->fng2 = atoi(pch);
		pch = strtok(NULL,";");
		hand->fng3 = atoi(pch);
		pch = strtok(NULL,";");
		hand->fng4 = atoi(pch);
		pch = strtok(NULL,";");
		hand->fng5 = atoi(pch);

		return RETURN_OK;
	}
	if(!ID.compare("MEC"))
	{
		Fingers * motors = (Fingers *) content;
		pch = strtok((char *)data.c_str(),";");
		motors->fng1 = atoi(pch);
		pch = strtok(NULL,";");
		motors->fng2 = atoi(pch);
		pch = strtok(NULL,";");
		motors->fng3 = atoi(pch);
		pch = strtok(NULL,";");
		motors->fng4 = atoi(pch);
		pch = strtok(NULL,";");
		motors->fng5 = atoi(pch);

		return RETURN_OK;
	}

	return RETURN_OK;

}

Glove_Ret Glove_Comm::glove_package_encode(string ID, void * content, string & payload)
{
	payload.clear();
	if(!ID.compare("MEC"))
	{
		char str_fng[4];
		Fingers * motor = (Fingers *) content;

		sprintf(str_fng, "%d",motor->fng1);
		payload.append(str_fng);
		payload.insert(payload.end(),1,DELIMITER);
		sprintf(str_fng, "%d",motor->fng2);
		payload.append(str_fng);
		payload.insert(payload.end(),1,DELIMITER);
		sprintf(str_fng, "%d",motor->fng3);
		payload.append(str_fng);
		payload.insert(payload.end(),1,DELIMITER);
		sprintf(str_fng, "%d",motor->fng4);
		payload.append(str_fng);
		payload.insert(payload.end(),1,DELIMITER);
		sprintf(str_fng, "%d",motor->fng5);
		payload.append(str_fng);

	}
	if(!ID.compare("STM"))
	{
		char str_fng[4];
		int * result = (int*) content;

		sprintf(str_fng, "%d",result);
		payload.append(str_fng);

		return RETURN_OK;
	}

	return RETURN_OK;

}

Glove_Ret Glove_Comm::glove_package_receive(string ID, void * content)
{
	string payload;
	string size_str;
	string ID_rec;
	string buffer;
	int size = 1;
	size_t start = 0, end = 0, aux = 0;


	do
	{
		payload.clear();
		size_str.clear();
		ID_rec.clear();
		buffer.clear();

		do
		{
			device_->glove_usb_read(buffer,size);
			start = buffer.find(SOH);
		}while(start==string::npos);

		start++;

		do
		{
			device_->glove_usb_read(buffer,size);
			end = buffer.find(ETX);
		}while(end==string::npos);

		ID_rec = buffer.substr(start,3);

	}while(ID != ID_rec);

	aux = buffer.find(STX);
	aux = aux - (start + 3);
	size_str = buffer.substr((start + 3), aux);
	size = atoi(size_str.c_str());
	aux = buffer.find(STX) +1;
	payload = buffer.substr(aux, (end-aux));

	return glove_package_decode(payload, ID_rec, content);
}

Glove_Ret Glove_Comm::glove_get_position(int* x, int* y, int* z)
{
	Glove_Ret ret = RETURN_OK;
	Pose position = {0};
	ret = glove_package_receive("POS", &position);

	(*x) = position.x;
	(*y) = position.y;
	(*z) = position.z;

	return ret;
}

Glove_Ret Glove_Comm::glove_get_accel(int* roll, int* pitch, int* yaw)
{
	Glove_Ret ret = RETURN_OK;
	Rot rotation = {0};
	ret = glove_package_receive("ANG", &rotation);

	(*roll) =  rotation.roll;
	(*pitch) = rotation.pitch;
	(*yaw) =   rotation.yaw;

	return ret;
}

Glove_Ret Glove_Comm::glove_get_flex(int* fng1, int* fng2, int* fng3, int* fng4, int* fng5)
{
	Glove_Ret ret = RETURN_OK;
	Fingers hand = {0};
	ret = glove_package_receive("FNG", &hand);

	(*fng1) = hand.fng1;
	(*fng2) = hand.fng2;
	(*fng3) = hand.fng3;
	(*fng4) = hand.fng4;
	(*fng5) = hand.fng5;

	return ret;
}

Glove_Ret Glove_Comm::glove_set_motors(int fng1, int fng2, int fng3, int fng4, int fng5)
{
	Glove_Ret ret = RETURN_OK;
	Fingers motor;
	string payload;

	payload.clear();

	motor.fng1 = fng1;
	motor.fng2 = fng2;
	motor.fng3 = fng3;
	motor.fng4 = fng4;
	motor.fng5 = fng5;

	glove_package_encode("MEC", &motor, payload);

	glove_package_send("MEC", payload, payload.size());

	return ret;
}


Glove_Ret Glove_Comm::glove_send_stim(int state)
{
	Glove_Ret ret = RETURN_OK;
	string payload;

	payload.clear();

	glove_package_encode("STM", &state, payload);

	glove_package_send("STM", payload, payload.size());


	return ret;
}

Glove_Ret Glove_Comm::glove_startup()
{
	Glove_Ret ret = RETURN_OK;



	return ret;
}

Glove_Comm::~Glove_Comm() {
	device_->glove_usb_close();
	// TODO Auto-generated destructor stub
}

} /* namespace glove */
