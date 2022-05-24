/*
 * xml_parser.h
 *
 *  Created on: 24 Mar 2020
 *      Author: Francisco Dominguegz
 */
#pragma once
#include "xml_scanner.h"
ifstream &operator>>(ifstream &ifxml,XMLNode &n);
class XMLParser{
	XMLNode n;
	XMLScanner scanner;
public:
	XMLParser(XMLNode n=ELEMENT_CONTAINER,string s=""):n(n),scanner(s){}
	XMLNode &parse(string &s){
		scanner.setString(s);
		n=XMLNode(ELEMENT_CONTAINER);
		return parseStep(n);
	}
	XMLNode &parseStep(XMLNode &parent){
		XMLNode n=scanner.getNextNode();
		while(n.getType()!=EON){
			if(n.getType()==ELEMENT_END){
				if(n.getName()==parent.getName())
					return parent;
				else
					throw runtime_error("Error in XMLParser::parseStep(): begin tag="+parent.getName()+" not match with end tag="+n.getName());
			}
			if(n.getType()==TEXT){
				parent.setText(n.getText());
			}
			if(n.getType()==ELEMENT_LEAF){
				parent.addchild(n);
			}
			if(n.getType()==ELEMENT_CONTAINER){
				XMLNode np=parseStep(n);
				parent.addchild(np);
			}
			n=scanner.getNextNode();
		}
		return parent;
	}
	static XMLNode loadNodeRootFromFileName(string fileName){
		XMLNode root;
	    ifstream ifxml(fileName);
	    ifxml >> root;
		return root;
	}
};
/*         STREAM I/O         */
ifstream &operator>>(ifstream &ifxml,XMLNode &n){
	string s;
	ifxml >> s;
	XMLParser p;
	n=XMLNode(p.parse(s));
	return ifxml;
}
