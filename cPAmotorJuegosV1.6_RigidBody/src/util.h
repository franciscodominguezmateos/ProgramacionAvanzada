/*
 * util.h *
 *  Created on: 11 Dec 2018
 *      Author: francisco
 */

#ifndef UTIL_H_
#define UTIL_H_
#include <cmath>
#include <cassert>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

/* RANDOMNESS */
inline float getRand(float max,float min=0){
	float n=max-min;int ir=rand()%10000; return min+n*(float)ir/10000;}

/* SPLITTING */
inline vector<string> split(string s,char delimiter=' '){
	   vector<string> tokens;
	   string token;
	   istringstream iss(s);
	   while (getline(iss, token, delimiter))
		  tokens.push_back(token);
	   return tokens;
	}
inline vector<string> split_first(string s,char delimiter){
	   vector<string> tokens;
	   if(s.size()<=3)
		   return tokens;
	   string token;
	   istringstream iss(s);
	   getline(iss, token, delimiter);
	   tokens.push_back(token);
	   getline(iss, token,'\r');
	   tokens.push_back(token);
	   return tokens;
	}

/* TRIMMING */
inline string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
	    str.erase(0, str.find_first_not_of(chars));return str;}
inline string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
    str.erase(str.find_last_not_of(chars) + 1);return str;}
inline string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
	return ltrim(rtrim(str, chars), chars);}

/* EXTENSIONS */
inline string takeAwayExtension(string &s){
	vector<string> vs=split(s,'.');	return vs[0];}
inline string getExtension(string &s){
	vector<string> vs=split(s,'.');	return vs[0];}

/* NEAR ZERO */
inline bool nearZero(double d){return fabs(d)<0.001;}

/* ANGULAR CONVERSION */
inline double deg2rad(double  a){return a/180*M_PI;}
inline double rad2deg(double r){return r/M_PI*180;}

#endif /* UTIL_H_ */
