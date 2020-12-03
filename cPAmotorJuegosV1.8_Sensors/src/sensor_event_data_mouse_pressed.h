/*
 * sensor_event_data_mouse_pressed.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 */

#pragma once
#include "sensor_event_data_mouse_moved.h"
class SensorEventDataMousePressed:public SensorEventDataMouseMoved{
	int button;
	int state;
public:
	SensorEventDataMousePressed(int button,int state,int x,int y):
		SensorEventDataMouseMoved(x,y),
		button(button),
		state(state){}
	inline int  &getButton() const {return button;}
	inline void setButton(int button) {this->button = button;}
	inline int  &getState() const {return state;}
	inline void setState(int state) {this->state = state;}
	void setData(const SensorEventData &event){
		SensorEventDataMouseMoved::setData(event);
		setButton(event.getInt("button"));
		setState(event.getInt("state"));
	}
};
