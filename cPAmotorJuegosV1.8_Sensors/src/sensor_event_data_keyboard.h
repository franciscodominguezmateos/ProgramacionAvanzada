/*
 * sensor_event_data_keyboard.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 */

#pragma once
class SensorEventDataKeyboard:public SensorEventDataMouseMoved{
	unsigned char key;
public:
	SensorEventDataKeyboard(unsigned char key,int x,int y):
			SensorEventDataMouseMoved(x,y),
			key(key){}
	inline unsigned char getKey() const {return key;}
	inline void          setKey(unsigned char key) {this->key = key;}
	void setData(const SensorEventData &event){
		SensorEventDataMouseMoved::setData(event);
		setKey(event.getChar("key"));
	}
};



