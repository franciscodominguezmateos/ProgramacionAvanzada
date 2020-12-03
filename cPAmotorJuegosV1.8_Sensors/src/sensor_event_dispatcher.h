/*
 * sensor_event_dispatcher.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include <vector>
#include "sensor_observer.h"

//Implement Chain of Resposabilities Design Pattern
//This class observe the general SensorEvent service and
//dispatch to specific SensorEventDataType
template <typename SensorEventDataType>
class SensorDispatcherAbstract:public SensorObserver{
protected:
	vector<SensorObserverX<SensorEventDataType>*> observers;
	SensorDispatcherAbstract* next;
public:
	SensorDispatcherAbstract():next(nullptr){}
	SensorDispatcherAbstract(SensorDispatcherAbstract* n):next(n){}
	virtual ~SensorDispatcherAbstract(){
		for(SensorObserverX<SensorEventDataType>* &o:observers)
			delete o;
		delete next;
	}
	void onSensorEvent(SensorEventData &e){
		if(e["device"]==SensorEventDataType::device &&
		   e["event"] ==SensorEventDataType::event){
			SensorEventDataType eventData;
			eventData.setData(e);
			dispatchEvent(eventData);
		}
		else{
			if(next!=nullptr)
				next->onSensorEvent(e);
		}
	}
	void dispatchEvent(SensorEventDataType e){
		for(SensorObserverX<SensorEventDataType>* &observer:observers)
			observer->onSensorEvent(e);
	}
};
typedef SensorDispatcherAbstract<SensorEventData>             SensorDispatcher;
typedef SensorDispatcherAbstract<SensorEventDataKeyboard>     SensorDispatcherKeyBoard;
typedef SensorDispatcherAbstract<SensorEventDataMousePressed> SensorDispatcherMousePressed;
typedef SensorDispatcherAbstract<SensorEventDataMouseMoved>   SensorDispatcherMouseMoved;
typedef SensorDispatcherAbstract<SensorEventDataWiiMote>      SensorDispatcherWiiMote;

