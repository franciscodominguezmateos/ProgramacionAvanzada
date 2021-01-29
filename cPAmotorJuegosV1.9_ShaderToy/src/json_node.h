/*
 * json_node.h
 *
 *  Created on: 29 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
/* This Abstract class try to avoid casting joining all methods */
class JSONNode;
using JSONNodePtr=JSONNode*;
class JSONNode{
	virtual JSONNodePtr operator[](int    i){return nullptr;}
	virtual JSONNodePtr operator[](string s){return nullptr;}
	virtual string getString(){return string();}
	virtual double getNumber(){return 0;}
	virtual int size(){return 0;}
};
class JSONNodeDictionary:public JSONNode{
	map<string,JSONNodePtr> m;
public:
	virtual JSONNodePtr operator[](string s){return m[s];}
};
class JSONNodeVector:public JSONNode{
	vector<JSONNodePtr> v;
public:
	virtual JSONNodePtr operator[](int    i){return v[i];}
};
class JSONNodeString:public JSONNode{
	string s;
public:
	JSONNodeString(string s):s(s){}
	string getString(){return s;}
};
class JSONNodeNumber:public JSONNode{
	double d;
public:
	JSONNodeNumber(double d):d(d){}
	double getNumber(){return d;}
};




