/*
 * json_node.h
 *
 *  Created on: 29 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
#include <vector>
#include <map>
using namespace std;
/* I am trying to avoid polymorphism in order to make it as easy as possible
 * This Abstract class try to avoid casting joining all methods
 */
class JSONNode{
	string s;
	map<string,int> m;
	vector<JSONNode> v;
public:
	JSONNode(){}
	JSONNode(string s):s(s){}
	JSONNode(double d):s(to_string(d)){}
	inline JSONNode &operator[](int    i){return v[i];}
	inline JSONNode &operator[](string s){return v[m[s]];}
	inline string &getString(){return s;}
	inline double getNumber(){return stod(s);}
	inline int size(){return v.size();}
	inline void add(JSONNode &n){v.push_back(n);}
	inline void add(string k,JSONNode &n){v.push_back(n);m[k]=v.size()-1;}
	inline bool isNodeNumber(){return false;}
	inline bool isNodeString(){return !isNodeNumber() && s!="";}
	inline bool isNodeDictionary(){return !isNodeString() && m.size()>0;}
	inline bool isNodeVector(){return !isNodeDictionary() && v.size()>0;}
};



