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
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

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
template<class T=double>
inline vector<T> split_numbers(string s,char delimiter=' '){
	   vector<T> tokens;
	   string token;
	   istringstream iss(s);
	   while (getline(iss, token, delimiter))
		  tokens.push_back((T)stod(token));
	   return tokens;
	}
inline vector<string> split_first(string s,char delimiter=' '){
	   vector<string> tokens;
	   if(s.size()==0) return tokens;
	   if(s.size()==1){tokens.push_back(s);return tokens;}
	   string token;
	   istringstream iss(s);
	   getline(iss, token, delimiter);
	   tokens.push_back(token);
	   if(getline(iss, token,'\r'))
		   tokens.push_back(token);
	   return tokens;
	}

/* STRING TOOLS: */
/* TRIMMING */
inline string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
	    str.erase(0, str.find_first_not_of(chars));return str;}
inline string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
    str.erase(str.find_last_not_of(chars) + 1);return str;}
inline string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
	return ltrim(rtrim(str, chars), chars);}
inline string toLower(string s){
	//string &su=vs[1];
	//transform(su.begin(), su.end(), su.begin(), ::tolower);
	string r;
	for(char c:s) r+=tolower(c);
	return r;
}
inline string& removeQuotes(string &s){
	return trim(s,"\t\"\n\v\f\r ");
}
string toString(ifstream &f){
	string s;
	if(f.is_open()){
		std::stringstream sstr;
		sstr << f.rdbuf();
		s = sstr.str();
		f.close();
	}else{
		throw runtime_error("File not opened in util.h::toString");
	}
	return s;
}
void operator>>(ifstream &f,string &s){
	s=toString(f);
}
void operator>>(ifstream &f,vector<string> &v){
	if(f.is_open()){
		string s;
		while(getline(f,s))
			v.push_back(s);
		f.close();
	}else{
		throw runtime_error("File not opened in util.h::operator>> to vector<string>");
	}
}

/* EXTENSIONS */
inline string takeAwayExtension(string &s){
	vector<string> vs=split(s,'.');	return vs[0];}
inline string getExtension(string &s){
	vector<string> vs=split(s,'.');	return vs[vs.size()-1];}

/* NUMBERS TOOLS */
inline bool nearZero(double d){return fabs(d)<0.001;}
inline double linearMap(double x,double i0,double i1,double o0,double o1){
	double difi=i1-i0;
	double difo=o1-o0;
	double v=(x-i0)/difi;
	double r=o0+difo*v;
	// Clamp values
	if(r>o1) r=o1;
	if(r<o0) r=o0;
	return r;
}

/* ANGULAR CONVERSION */
static const double DEG2RAD=M_PI/180.0;
static const double RAD2DEG=180.0/M_PI;
inline double deg2rad(double a){return a*DEG2RAD;}
inline double rad2deg(double r){return r*RAD2DEG;}

#endif /* UTIL_H_ */
