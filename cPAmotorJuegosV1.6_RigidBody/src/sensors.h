/*
 * sensors.h
 *
 *  Created on: 19 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include <chrono>
#include "socket_TCP_server.h"

using namespace std;

class Sensors {
};
typedef CGI SensorEventData;
class SensorObserver{
public:
	virtual void onSensorEvent(SensorEventData &sed)=0;
};
class SensorCGIProcessor:public StringProcessor{
	vector<CGI> sensorEvents;
	mutex mtxSensorEvents;
	vector<SensorObserver*> observers;
	SocketTCPStringServer string_svr;
	chrono::time_point<chrono::system_clock> before;
	int speed;//in milliseconds
public:
	SensorCGIProcessor(int port=8881):string_svr(this,port),before(chrono::system_clock::now()),speed(30){}
	void addSensorObserver(SensorObserver* so){observers.push_back(so);}
	void dispatchSensorObserverEvent(SensorEventData &e){
		for(SensorObserver* &so:observers)
			so->onSensorEvent(e);
	}
	string process(string &si){
		CGI event(si);
		//cout << si << endl;
		mtxSensorEvents.lock();
		sensorEvents.clear();
		sensorEvents.push_back(si);
		mtxSensorEvents.unlock();
	    chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
	    int elapsed_milliseconds = chrono::duration_cast<chrono::milliseconds>(now-before).count();
	    if(elapsed_milliseconds>speed){
	    	before=now;
	    	dispatchSensorObserverEvent(sensorEvents.front());
	    }
	    return "OK";
	}
};

