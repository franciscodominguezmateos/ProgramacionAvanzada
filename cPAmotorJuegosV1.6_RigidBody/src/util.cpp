/*
 * util.cpp
 *
 *  Created on: 11 Dec 2018
 *      Author: Francisco
 */
#include "util.h"

vector<string> split(string &s,char delimiter){
   vector<string> tokens;
   string token;
   istringstream iss(s);
   while (getline(iss, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

std::string& ltrim(std::string& str, const std::string& chars )
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars )
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars)
{
    return ltrim(rtrim(str, chars), chars);
}
string takeAwayExtension(string &s){
	vector<string> vs=split(s,'.');
	return vs[0];
}

