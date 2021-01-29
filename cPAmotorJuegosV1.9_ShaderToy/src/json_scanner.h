/*
 * json_scanner.h
 *
 *  Created on: 29 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
#include <string>
using namespace std;
class Token{
	string id;
	string lexem;
public:
	Token(char c):id(c){}
	Token(string t,string lex=""):id(t),lexem(lex){}
	inline bool operator==(char   c){return id==string(c);}
	inline bool operator==(string s){return id==s;}
	inline bool operator!=(char   c){return id!=string(c);}
	inline bool operator!=(string s){return id!=s;}
	string getLexem()    {return lexem;}
	int   getLexemInt()  {return stoi(lexem);}
	float getLexemFloat(){return stof(lexem);}
};
class JSONScanner {
	string s;
	unsigned int state;
	unsigned int idx;
	bool verbose;
public:
	JSONScanner(string s=""):s(s),state(0),idx(0),verbose(false){}
	inline void setString(string &rs){s=rs;}
	inline string &getString(){return s;}
	char getChar(){
		string &s=getString();
		if(idx>=s.length())
			throw runtime_error("EOF in JSONScanner::getChar()");
		char c=s[idx++];
		return c;
	}
	char returnChar(char rc){
		string &s=getString();
		if(idx<=0)
			throw runtime_error("BOF in JSONScanner::returnChar()");
		char c=s[--idx];
		if(c!=rc)
			throw runtime_error("Not matching chars in JSONScanner::returnChar()");
		return c;
	}
	inline bool isDigit(char c){return c>='0' && c<='9';}
	string scanString(){
		string str;
		char c=getChar();
		if(c!='"') throw runtime_error("Waiting for a '\"' but a '"+c+"' found in JSONScanner::scanString()");
        c=getChar();
		while(c!='"'){
			str+=c;
			c=getChar();
		}
		return str;
	}
	string scanNumber(){
		string number;
		char c=getChar();
		while(isDigit(c)){
			number+=c;
			c=getChar();
		}
		//decimals
		if(c=='.'){
			number+=c;
			char c=getChar();
			while(isDigit(c)){
				number+=c;
				c=getChar();
			}
		}
		returnChar(c);
		return number;
	}
	Token getNextToken(){
		while(true) try{
			char c=getChar();
			if(c=='[')     return Token(c);
			if(c==']')     return Token(c);
			if(c=='{')     return Token(c);
			if(c=='}')     return Token(c);
			if(c==',')     return Token(c);
			if(c==':')     return Token(c);
			if(c=='"')     {returnChar(c); return Token("string",scanString());}
			if(isDigit(c)) {returnChar(c); return Token("number",scanNumber());}
		}
		catch(exception &e){
			return Token("EOF");
		}
	}
};

