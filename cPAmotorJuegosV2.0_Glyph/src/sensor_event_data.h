/*
 * sensors.h
 *
 *  Created on: 19 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once

using namespace std;

//for now SensorEventData is just a CGI HTTP protocol parameter wrapper
//this events are low level and high abstraction
typedef CGI SensorEventData;

class SensorEventAbstract{
	int id;
public:
	inline int  getId() const {return id;}
	inline void setId(int id) {this->id = id;}
	virtual void setData(SensorEventData &event){
		setId(event.getInt("id"));
	}
};
