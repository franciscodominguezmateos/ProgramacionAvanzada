/*
 * util.h *
 *  Created on: 11 Dec 2018
 *      Author: Francisco Dominguez
 */
#pragma once
#include <cmath>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>

using namespace std;

//This is OpenCV specific it should be in other file? comething like opencv_util.h?
#include "opencv2/opencv.hpp"
using namespace cv;
string cvtype2str(int type) {
 string r;

 uchar depth = type & CV_MAT_DEPTH_MASK;
 uchar chans = 1 + (type >> CV_CN_SHIFT);

 switch ( depth ) {
   case CV_8U:  r = "8U"; break;
   case CV_8S:  r = "8S"; break;
   case CV_16U: r = "16U"; break;
   case CV_16S: r = "16S"; break;
   case CV_32S: r = "32S"; break;
   case CV_32F: r = "32F"; break;
   case CV_64F: r = "64F"; break;
   default:     r = "User"; break;
 }

 r += "C";
 r += (chans+'0');

 return r;
}
/* TIME FUNCTIONS */
template<typename S=double>
S getTimeInSeconds(){
	auto now  =chrono::high_resolution_clock::now();
	typedef chrono::duration<S> scalar_seconds;
	auto s_secs = chrono::duration_cast<scalar_seconds>(now);
	return s_secs.count();
}
template<typename S=double>
S getDurationInSeconds(chrono::steady_clock::time_point &before){
	chrono::steady_clock::time_point now  =chrono::steady_clock::now();
	auto dur=now-before;
	typedef chrono::duration<S> scalar_seconds;
	auto s_secs = chrono::duration_cast<scalar_seconds>(dur);
	return s_secs.count();

}
/* BITWISE FUNCTIONS */
#define BIT_ON(bf,b)  (bf) = ((bf) |  (b))
#define BIT_OFF(bf,b) (bf) = ((bf) & ~(b))
#define BIT_STATUS(bf,b) ((bf) & (b))

/*
Generic function to find an element in vector and also its position.
It returns a pair of bool & int i.e.

bool : Represents if element is present in vector or not.
int : Represents the index of element in vector if its found else -1
FROM: https://thispointer.com/c-how-to-find-an-element-in-vector-and-get-its-index/
*/
template < typename T>
pair<bool, int > findInVector(const std::vector<T>  & vecOfElements, const T  & element)
{
	pair<bool, int > result;
	// Find given element in vector
	auto it = find(vecOfElements.begin(), vecOfElements.end(), element);
	if (it != vecOfElements.end()){
		result.second = distance(vecOfElements.begin(), it);
		result.first = true;
	}
	else{
		result.first = false;
		result.second = -1;
	}
	return result;
}

/* RANDOMNESS */
//TODO change order first min then max
inline float getRand(float max,float min){
	float n=max-min;int ir=rand()%10000; return min+n*(float)ir/10000;}
inline float getRand(float max){return getRand(max,0);}

/* STRING TOOLS: */
inline bool contains(const string &doc,const string &word){return doc.find(word)!=string::npos;}
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
// Replace all chars in param chars for param rchar in text
inline string &replaceChars(string &text,const string &chars="\t\n\v\f\r",char rchar=' '){
	for(const char &c:chars) replace( text.begin(), text.end(), c, rchar);
	return text;
}
inline string& removeQuotes(string &s){
	return trim(s,"\t\"\n\v\f\r ");
}
//TODO
//remove more than one space (To use in GLSLShader::parseUniformNames)
//inline string &leftOneSpace(string &s){
//}
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
void operator>>(stringstream &f,vector<string> &v){
	string s;
	while(getline(f,s))
		v.push_back(s);
}
inline void operator>>(string &s,vector<string> &lines){
	stringstream sstr(s);
	sstr>>lines;
}
/* FILE EXTENSIONS */
inline string takeAwayExtension(string &s){
	vector<string> vs=split(s,'.');	return vs[0];}
inline string getExtension(string &s){
	vector<string> vs=split(s,'.');	return vs[vs.size()-1];}

/* NUMBERS TOOLS */
template<typename S=double>
inline bool nearZero(S d,S threshold=0.0001){return abs(d)<threshold;}
template<typename S=double>
inline bool nearEquals(S d0,S d1,S threshold=0.0001){return nearZero<S>(d1-d0,threshold);}
template<typename S=double>
inline S linearMap(S x,S i0,S i1,S o0=0,S o1=1){
	S difi=i1-i0;
	S difo=o1-o0;
	S v=(x-i0)/difi;
	S r=o0+difo*v;
	// Clamp values
	if(r>o1) r=o1;
	if(r<o0) r=o0;
	return r;
}
/* ANGULAR CONVERSION */
static const double DEG2RAD=M_PI/180.0;
static const double RAD2DEG=180.0/M_PI;
template<typename S=double>
inline S deg2rad(S a){return a*(S)DEG2RAD;}
template<typename S=double>
inline S rad2deg(S r){return r*(S)RAD2DEG;}

/* VECTOR */
// from: http://www.jclay.host/dev-journal/simple_cpp_argmax_argmin.html
template <typename T, typename A>
int arg_max(std::vector<T, A> const& vec) {
  return static_cast<int>(std::distance(vec.begin(), max_element(vec.begin(), vec.end())));
}
template <typename T, typename A>
int arg_min(std::vector<T, A> const& vec) {
  return static_cast<int>(std::distance(vec.begin(), min_element(vec.begin(), vec.end())));
}
