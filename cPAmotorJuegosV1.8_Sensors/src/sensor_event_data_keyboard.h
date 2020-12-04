/*
 * sensor_event_data_keyboard.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 */

#pragma once
#include "sensor_event_data_mouse_moved.h"
class SensorEventDataKeyboard:public SensorEventDataMouseMoved{
	unsigned char key;
public:
	SensorEventDataKeyboard(unsigned char key,int x,int y):
			SensorEventDataMouseMoved(x,y),
			key(key){}
	inline unsigned char getKey() {return key;}
	inline void          setKey(unsigned char key) {this->key = key;}
	void setData(SensorEventData &event){
		SensorEventDataMouseMoved::setData(event);
		setKey(event.getChar("key"));
	}
};



