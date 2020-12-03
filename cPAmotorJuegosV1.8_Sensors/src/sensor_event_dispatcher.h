/*
 * sensor_event_dispatcher.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include "sensor_observer.h"

class SensorDispatcherStandard:public SensorObserver{
	SensorEventDataWiiMote wiim[2];
public:
	void onSensorEvent(SensorEventData &e){
		if(e["device"]=="WiiMote"){
			int id=e.getInt("id");
		}
		// custom generate mobil python3 device
		if(e["device"]=="qpy3"){
			double x=e.getDouble("roll");
			double y=e.getDouble("pitch");
		}
/*		if(e["device"]=="mouse"){
			if(e["event"]=="MouseMoved")
				mouseMoved(e.getInt("x"),e.getInt("y"));
			if(e["event"]=="MousePress")
				mousePress(e.getInt("button"),e.getInt("state"),e.getInt("x"),e.getInt("y"));
		}
		if(e["device"]=="keyboard")
			keyPressed(e.getChar("key"),e.getInt("x"),e.getInt("y"));*/
	}
};
