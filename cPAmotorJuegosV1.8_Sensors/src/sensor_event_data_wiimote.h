/*
 * sensor_event_data_wiimote.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 */

#pragma once
#include "sensor_event_data.h"

//This class abstracts the information sent from a Wiimote as SensorEventData
class SensorEventDataWiiMote:public SensorEventAbstract{
	static const int CWIID_BTN_2    =   1;
	static const int CWIID_BTN_1    =   2;
	static const int CWIID_BTN_B    =   4;
	static const int CWIID_BTN_A    =   8;
	static const int CWIID_BTN_MINUS=  16;
	static const int CWIID_BTN_HOME = 128;
	static const int CWIID_BTN_LEFT = 256;
	static const int CWIID_BTN_RIGHT= 512;
	static const int CWIID_BTN_DOWN =1024;
	static const int CWIID_BTN_UP   =2048;
	static const int CWIID_BTN_PLUS =4096;
	//old state of buttons
	int obuttons;
	//new state of buttons
	int ebuttons;
	double x,y,z;
public:
	SensorEventDataWiiMote():obuttons(0),ebuttons(0){}
	void setData(const SensorEventData &e){
		SensorEventAbstract::setData(e);
		obuttons=ebuttons;
		ebuttons=e.getInt("buttons");
		id=e.getInt("id");
		//Accelerometer data
		x=e.getDouble("x");
		y=e.getDouble("y");
		z=e.getDouble("z");
	}
	inline unsigned long buttons(){return ebuttons;}
	inline bool isPressed(int b){return  BIT_STATUS(buttons(),b);}
	inline bool isDown(int b)   {return !BIT_STATUS(obuttons,b) &&  BIT_STATUS(buttons(),b);}
	inline bool isUp(int b)     {return  BIT_STATUS(obuttons,b) && !BIT_STATUS(buttons(),b);}
	bool is1pressed()    {return isPressed(CWIID_BTN_1);}
	bool is2pressed()    {return isPressed(CWIID_BTN_2);}
	bool isApressed()    {return isPressed(CWIID_BTN_A);}
	bool isBpressed()    {return isPressed(CWIID_BTN_B);}
	bool isMINUSpressed(){return isPressed(CWIID_BTN_MINUS);}
	bool isHOMEpressed() {return isPressed(CWIID_BTN_HOME);}
	bool isLEFTpressed() {return isPressed(CWIID_BTN_LEFT);}
	bool isRIGHTpressed(){return isPressed(CWIID_BTN_RIGHT);}
	bool isDOWNpressed() {return isPressed(CWIID_BTN_DOWN);}
	bool isUPpressed()   {return isPressed(CWIID_BTN_UP);}
	bool isPLUSpressed() {return isPressed(CWIID_BTN_PLUS);}
	bool is1down()    {return isDown(CWIID_BTN_1);}
	bool is2down()    {return isDown(CWIID_BTN_2);}
	bool isAdown()    {return isDown(CWIID_BTN_A);}
	bool isBdown()    {return isDown(CWIID_BTN_B);}
	bool isMINUSdown(){return isDown(CWIID_BTN_MINUS);}
	bool isHOMEdown() {return isDown(CWIID_BTN_HOME);}
	bool isLEFTdown() {return isDown(CWIID_BTN_LEFT);}
	bool isRIGHTdown(){return isDown(CWIID_BTN_RIGHT);}
	bool isDOWNdown() {return isDown(CWIID_BTN_DOWN);}
	bool isUPdown()   {return isDown(CWIID_BTN_UP);}
	bool isPLUSdown() {return isDown(CWIID_BTN_PLUS);}
	//TODO same with idUp
	double getX() {return  x;}
	double getY() {return  y;}
	double getZ() {return  z;}
};




