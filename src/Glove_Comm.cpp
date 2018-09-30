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
		pch = strtok(pch,";");
		rotation->pitch = atoi(pch);
		pch = strtok(pch,";");
		rotation->yaw = atoi(pch);

		return RETURN_OK;

	}
	if(!ID.compare("STM"))
	{
		bool * result = (bool*) content;
		if(data == "true")
			(*result) = true;
		else
			(*result) = false;

		return RETURN_OK;
	}
	if(!ID.compare("FNG"))
	{
		Fingers * hand = (Fingers *) content;
		pch = strtok((char *)data.c_str(),";");
		hand->fng1 = atoi(pch);
		pch = strtok(pch,";");
		hand->fng2 = atoi(pch);
		pch = strtok(pch,";");
		hand->fng3 = atoi(pch);
		pch = strtok(pch,";");
		hand->fng4 = atoi(pch);
		pch = strtok(pch,";");
		hand->fng5 = atoi(pch);

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

	payload.clear();
	size_str.clear();
	ID_rec.clear();
	buffer.clear();

	do
	{
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

Glove_Ret Glove_Comm::glove_send_stim(bool* state)
{
	Glove_Ret ret = RETURN_OK;

	ret = glove_package_receive("STM", state);

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
