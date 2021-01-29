/*
 * json_parser.h
 *
 *  Created on: 29 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
#include "json_scanner.h"
#include "json_node.h"
/* Gramar:
 *
 * JSON          -> ELEMENT
 * ELEMENT       -> DICTIONARY | VECTOR | STRING | NUMBER
 * DICTIONARY    -> '{' ELEMENT_PAIRS '}'
 * ELEMENT_PAIRS -> ELEMENT_PAIR ',' ELEMENT_PAIRS | ELEMENT_PAIR
 * ELEMENT_PAIR  -> ELEMENT ':' ELEMENT
 * VECTOR        -> '[' ELEMENTS ']'
 * ELEMENTS      -> ELEMENT ',' ELEMENTS | ELEMENT
 * STRING        -> just a quoted string
 * NUMBER        -> just a number with decimals but not scientific notation
 *
 */
class JSONParser {
	JSONScanner scanner;
public:
	JSONParser(JSONScanner s):scanner(s){}
	JSONNodePtr parse(string &s){
		scanner.setString(s);
		return parseElement();
	}
	JSONNodePtr parseElement(){
		Token t=scanner.getNextToken();
		while(t!="EOF"){
			if(t=='[')      return parseVector();
			if(t=='{')      return parseDictionary();
			if(t=="string") return parseString();
			if(t=="number") return parseNumber();
		}
	}
	JSONNodePtr parseVector(){
		JSONNodePtr n=new JSONNodeVector();
		Token t=scanner.getNextToken();
		while(t!=']'){
			JSONNodePtr child=parseStep();
			n->add(child);
			t=scanner.getNetToken();
			if(t!=',' || t!=']') throw runtime_error("Waiting for a ',' or a ']' in in JSONParser::parseVector()");
		};
		return n;
	}
	JSONNodePtr parseDictionary(){
		JSONNodePtr n=new JSONNodeDictionary();
		Token t=scanner.getNextToken();
		while(t!='}'){
			JSONNodePtr key=parseStep();
			t=scanner.getNetToken();
			if(t!=':') throw runtime_error("Waiting for a ':' in in JSONParser::parseDictionary()");
			JSONNodePtr child=parseStep();
			n->add(key,child);
			t=scanner.getNetToken();
			if(t!=',' || t!='}') throw runtime_error("Waiting for a ',' or a '}' in in JSONParser::parseDictionary()");
		};
		return n;
	}
	JSONNodePtr parseString(){

	}
	JSONNodePtr parseNumber(){

	}
};

