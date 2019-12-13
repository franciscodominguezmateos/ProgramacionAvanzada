/*
 * util.h
 *
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

float getRand(float max,float min=0);
vector<string> split(string s,char delimiter=' ');
vector<string> split_first(string s,char delimiter);
std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
string takeAwayExtension(string &s);
inline bool nearZero(double d){return fabs(d)<0.001;}
inline double deg2rad(double  a){return a/180*M_PI;}
inline double rad2deg(double r){return r/M_PI*180;}

#endif /* UTIL_H_ */
