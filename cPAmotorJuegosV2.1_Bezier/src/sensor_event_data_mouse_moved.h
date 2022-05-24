/*
 * sensor_event_data_mouse_moved.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 */

#pragma once
#include "sensor_event_data.h"
class SensorEventDataMouseMoved:public SensorEventAbstract{
	int x;
	int y;
public:
	SensorEventDataMouseMoved(int x,int y):x(x),y(y){}
	int &getX() {return x;}
	int &getY() {return y;}
	void setX(int x) {this->x = x;}
	void setY(int y) {this->y = y;}
	void setData(SensorEventData &event){
		SensorEventAbstract::setData(event);
		setX(event.getInt("x"));
		setY(event.getInt("y"));
	}
};
