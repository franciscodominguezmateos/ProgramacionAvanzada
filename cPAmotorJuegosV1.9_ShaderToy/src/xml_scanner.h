/*
 * xml_parser.h
 *
 *  Created on: 24 Mar 2020
 *      Author: Francisco Dominguegz
 */
#pragma once
#include "xml_node.h"
class XMLScanner{
	//how to use regex to scann xml tags not used due to limitation in token size to 1000char
	//from: https://rextester.com/WGBY40008
	string s;
	unsigned int state;
	unsigned int idx;
	bool verbose;
public:
	XMLScanner(string s=""):s(s),state(0),idx(0),verbose(false){}
	inline void setString(string &rs){s=rs;}
	inline string &getString(){return s;}
	char getChar(){
		string &s=getString();
		if(idx>=s.length())
			throw runtime_error("EOF in XMLScannerStrong::getChar()");
		char c=s[idx++];
		return c;
	}
	//tiny automata to scan tags and text in XML
	string getNextString() try{
		string rs;
		while(true){
			char c=getChar();
			if(state==0){
				if(c=='<'){
					state=1;
					//avoid all spaces or similar
					if(trim(rs)!="")
						return rs;
					continue;
				}
			}
			if(state==1){
				if(c=='>'){
					state=0;
					return "<"+rs+">";
				}
			}
			rs+=c;
		}
		return "";//to avoid compiler warning
	}
	catch(exception &e){
		return "";
	}
	inline bool isTag(const string &s){return s[0]=='<' && s[s.length()-1]=='>';}
    inline bool isText(const string &s){return s[0]!='<' && s[s.length()-1]!='>';}
	inline bool isElementLeaf(const string &s){return isTag(s) && s[s.length()-2]=='/';}
	inline bool isElementEnd (const string &s){return isTag(s) && s[1]=='/';}
	inline bool isElementContainer(const string &s){return isTag(s) && s[1]!='/' && s[1]!='!' && s[1]!='?' && s[1]!=':' && s[s.length()-2]!='/';}
	inline void setNameAndAttributes(XMLNode &r,string &ns){
		vector<string> vs=split_first(ns);
		if(vs.size()>0){
			r.setName(vs[0]);
			if(vs.size()==2)
				r.setAttributes(vs[1]);
		}
	}
	//node act as token
	XMLNode getNextNode(){
		XMLNode r;
		string ns=getNextString();
		if(verbose) cout << ns << endl;
		if(ns==""){ //End Of Nodes
			r.setType(EON);
		}
		else if(isText(ns)){//just text
			r.setType(TEXT);
			string txt=ns;
			r.setText(txt);
		}
		else if(isTag(ns)){
			if(isElementContainer(ns)){
				r.setType(ELEMENT_CONTAINER);
				ns=ns.substr(1,ns.length()-2);
				setNameAndAttributes(r,ns);
			}
			else if(isElementLeaf(ns)){
				r.setType(ELEMENT_LEAF);
				ns=ns.substr(1,ns.length()-3);
				setNameAndAttributes(r,ns);
			}
			else if(isElementEnd(ns)){
				r.setType(ELEMENT_END);
				ns=ns.substr(2,ns.length()-3);
				r.setName(ns);
			}
		}
		else throw runtime_error("Error in XMLScanner::getNexNode(): Bad tag="+ns);
		return r;
	}
};
