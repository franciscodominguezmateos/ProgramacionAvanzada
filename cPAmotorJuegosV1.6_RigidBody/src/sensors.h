/*
 * sensors.h
 *
 *  Created on: 19 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include <mutex>
#include <chrono>
#include "socket_TCP_server.h"
#include "sensor_event_data.h"

using namespace std;

class SensorObserver{
public:
	virtual void onSensorEvent(SensorEventData &sed)=0;
};

typedef map<string,SensorEventData> TSensorEventIDs;
typedef map<string,TSensorEventIDs> TSensorEventDevices;
class SensorEventProcessor:public StringProcessor{
	TSensorEventDevices sensorEvents;
	mutex mtxSensorEvents;
	vector<SensorObserver*> observers;
	SocketTCPStringServer string_svr;
	chrono::time_point<chrono::system_clock> before;
	int speed;//in milliseconds
public:
	SensorEventProcessor(int port=8881):string_svr(this,port),before(chrono::system_clock::now()),speed(10){}
	void addSensorObserver(SensorObserver* so){observers.push_back(so);}
	void dispatchSensorObserverEvent(SensorEventData &e){
		for(SensorObserver* &so:observers)
			so->onSensorEvent(e);
	}
	void setSpeed(int s){speed=s;}
	string process(string &si){
		CGI event(si);
		string device=event["device"];
		string id=event["id"];
		//cout << si << endl;
		mtxSensorEvents.lock();
		sensorEvents[device][id]=si;
		mtxSensorEvents.unlock();
	    chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
	    int elapsed_milliseconds = chrono::duration_cast<chrono::milliseconds>(now-before).count();
	    if(elapsed_milliseconds>speed){
	    	before=now;
	    	for(pair<string,TSensorEventIDs> pe:sensorEvents)
	    		for(pair<string,SensorEventData> pi:pe.second)
	    			//dispatchSensorObserverEvent(sensorEvents[device][id]);
	    	        dispatchSensorObserverEvent(pi.second);
	    }
	    return "OK";
	}
};
