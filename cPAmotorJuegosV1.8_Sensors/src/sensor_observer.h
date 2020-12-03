/*
 * sensor_observer.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "sensor_event_data.h"


template<typename SensorEventDataType>
class SensorObserverX{
public:
	virtual void onSensorEvent(SensorEventDataType &sed)=0;
};

typedef SensorObserverX<SensorEventData> SensorObserver;
fype
