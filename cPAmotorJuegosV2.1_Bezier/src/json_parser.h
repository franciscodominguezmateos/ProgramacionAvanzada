/*
 * json_parser.h
 *
 *  Created on: 29 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
#include "json_scanner.h"
#include "json_node.h"
/* Grammar:
 *
 * JSON          -> ELEMENT
 * ELEMENT       -> DICTIONARY | VECTOR | string | number
 * DICTIONARY    -> '{' ELEMENT_PAIRS '}'
 * ELEMENT_PAIRS -> ELEMENT_PAIR ',' ELEMENT_PAIRS | ELEMENT_PAIR
 * ELEMENT_PAIR  -> string ':' ELEMENT
 * VECTOR        -> '[' ELEMENTS ']'
 * ELEMENTS      -> ELEMENT ',' ELEMENTS | ELEMENT
 * string        -> just a quoted string
 * number        -> just a number with decimals but not scientific notation
 *
 */
class JSONParser {
	JSONScanner scanner;
	Token t;//current token
	JSONNode root;
public:
	JSONParser(JSONScanner s=JSONScanner()):scanner(s){}
	JSONNode &parse(string &s){
		scanner.setString(s);
		t=scanner.getNextToken();
		root= parseElement();
		return root;
	}
	JSONNode parseElement(){
		if(t=='[')      return parseVector();
		if(t=='{')      return parseDictionary();
		if(t=="string") return parseString();
		if(t=="number") return parseNumber();
	}
	JSONNode parseVector(){
		JSONNode n;
		t=scanner.getNextToken();
		while(t!=']'){
			JSONNode child=parseElement();
			n.add(child);
			t=scanner.getNextToken();
			if(t!=',' || t!=']') throw runtime_error("Waiting for a ',' or a ']' in in JSONParser::parseVector()");
		};
		return n;
	}
	JSONNode parseDictionary(){
		JSONNode n;
		t=scanner.getNextToken();
		while(t!='}'){
			if(t!="string") throw runtime_error("Waiting a string as key in JSONParser::parseDictionary()");
			string k=t.getLexem();
			t=scanner.getNextToken();
			if(t!=':') throw runtime_error("Waiting for a ':' in JSONParser::parseDictionary()");
			JSONNode child=parseElement();
			n.add(k,child);
			t=scanner.getNextToken();
			if(t!=',' || t!='}') throw runtime_error("Waiting for a ',' or a '}' in in JSONParser::parseDictionary()");
		};
		return n;
	}
	JSONNode parseString(){return JSONNode(t.getLexem());}
	JSONNode parseNumber(){return JSONNode(t.getLexemNumber());}
};
/*         STREAM I/O         */
ifstream &operator>>(ifstream &ifxml,XMLNode &n){
	string s;
	ifxml >> s;
	JSONParser p();
	n=p.parse(s);
	return ifxml;
}
