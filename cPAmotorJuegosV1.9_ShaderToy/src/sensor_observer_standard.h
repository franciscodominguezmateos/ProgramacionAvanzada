/*
 * sensor_observer_standard.h
 *
 *  Created on: 17 Jan 2021
 *      Author: Francisco Dominguez
 *  This class forward local keyboard and mouse events
 */

#pragma once

#include "sensor_observer.h"

class SensorObserverStandard: public SensorObserver {
	virtual void mouseMoved(int x, int y)=0;
	virtual void mousePress(int button, int state, int x, int y)=0;
	virtual void keyPressed(char key,int x,int y)=0;
	virtual void onSensorEvent(SensorEventData &e){
		if(e["device"]=="mouse" && e["id"]=="0"){
			if(e["event"]=="MouseMoved")
				mouseMoved(e.getInt("x"),e.getInt("y"));
			if(e["event"]=="MousePress")
				mousePress(e.getInt("button"),e.getInt("state"),e.getInt("x"),e.getInt("y"));
		}
		if(e["device"]=="keyboard" && e["id"]=="0")
			keyPressed(e.getChar("key"),e.getInt("x"),e.getInt("y"));
	}
};

