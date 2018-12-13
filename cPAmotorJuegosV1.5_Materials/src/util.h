/*
 * util.h
 *
 *  Created on: 11 Dec 2018
 *      Author: francisco
 */

#ifndef UTIL_H_
#define UTIL_H_
#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<string> split(string &s,char delimiter=' ');
std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
string takeAwayExtension(string &s);
#endif /* UTIL_H_ */
