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
#include "sensor_observer.h"

using namespace std;
/**
 * This should be a queue of SensorEventData
 * There must be a process taking SensorEventData from the queues
 * and dispatching them. BUT...
 */
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
	void addEvent(SensorEventData event){
		string device=event["device"];
		string id=event["id"];
		//cout << si << endl;
		mtxSensorEvents.lock();
		sensorEvents[device][id]=event;
		mtxSensorEvents.unlock();
	}
	string process(string &si){
		CGI event(si);
		addEvent(event);
	    chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
	    int elapsed_milliseconds = chrono::duration_cast<chrono::milliseconds>(now-before).count();
	    if(elapsed_milliseconds>speed){
	    	before=now;
			mtxSensorEvents.lock();
	    	for(pair<string,TSensorEventIDs> pe:sensorEvents)
	    		for(pair<string,SensorEventData> pi:pe.second)
	    			//dispatchSensorObserverEvent(sensorEvents[device][id]);
	    	        dispatchSensorObserverEvent(pi.second);
			mtxSensorEvents.unlock();
	    }
	    return "OK";
	}
};
