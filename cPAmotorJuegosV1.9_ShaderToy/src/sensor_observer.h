/*
 * sensor_observer.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "sensor_event_data.h"
#include "sensor_event_data_keyboard.h"
#include "sensor_event_data_mouse_pressed.h"
#include "sensor_event_data_mouse_moved.h"
#include "sensor_event_data_wiimote.h"


template<typename SensorEventDataType>
class SensorObserverX{
public:
	virtual void onSensorEvent(SensorEventDataType &sed)=0;
};

typedef SensorObserverX<SensorEventData>             SensorObserver;
typedef SensorObserverX<SensorEventDataKeyboard>     SensorObserverKeyboard;
typedef SensorObserverX<SensorEventDataMousePressed> SensorObserverMousePressed;
typedef SensorObserverX<SensorEventDataMouseMoved>   SensorObserverMouseMoved;
typedef SensorObserverX<SensorEventDataWiiMote>      SensorObserverWiiMote;

