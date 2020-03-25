/*
 * xml_parser.h
 *
 *  Created on: 24 Mar 2020
 *      Author: Francisco Dominguegz
 */

#ifndef XML_PARSER_H_
#define XML_PARSER_H_
#include <regex>
#include "util.h"

/*
 * I am trying to avoid polymorphism in order to make it as easy as posible
 * ELEMENT_CONTAINER, ELEMENT_LEAF both are XMLNode not difference in runtime just in parser time.
 * TEXT this is used as a temporal tokem from scanner to parser
 * UNKNOW comments, declarations and others to discard
 * ELEMENT_END used as temporal tokem to detect end of ELEMENT_CONTAINER
 * EON end of nodes to make parser finish
 */
enum XMLNodeType { ELEMENT_CONTAINER, ELEMENT_LEAF, TEXT, UNKNOW, ELEMENT_END, EON };
class XMLNode{
	XMLNodeType type;
	string name;
	string text;
	map<string,string> attributes;
	map<string,vector<XMLNode>> children;
public:
	XMLNode():type(UNKNOW){}
	void setAttributes(string key_value_pairs){
		//take away any space at beginning or end
		trim(key_value_pairs);
		vector<string> pairs=split(key_value_pairs);
		for(string pair:pairs){
			vector<string> keyvalue=split(pair,'=');
			string key=keyvalue[0];
			string value=keyvalue[1];
			attributes[key]=value;
		}
	}
	void addchild(XMLNode n){
		string name=n.getName();
		// name it isn't on the map
		if(children.find(name) == children.end()){
			children[name]=vector<XMLNode>();
		}
		children[name].push_back(n);
	}
	const map<string, string>& getAttributes() const {return attributes;}
	const map<string, vector<XMLNode> >& getChildren() const {return children;	}
	void setChildren(const map<string, vector<XMLNode> > &children) {this->children = children;}
	const string& getName() const {return name;	}
	void setName(const string &name) {this->name = name;}
	const string& getText() const {return text;}
	void setText(const string &text) {this->text = text;}
	XMLNodeType getType() const {return type;}
	void setType(XMLNodeType type) {this->type = type;}
};
class XMLscanner{
	//from: https://rextester.com/WGBY40008
	string s;
	smatch m;
	regex e;
public:
	XMLscanner(string s):s(s),e("<(/*)(\\S+?)\\b(.*?)(/*)>|([^<]+)"){}
	string getNextString(){
		bool ok=true;
		while(regex_search (s,m,e, regex_constants::match_any)){
			string r= m[0].str();
			s = m.suffix().str();
			ok=trim(r)!="";
			if(ok) return r;
		}
		return "";
	}
	//detect type of Node from smatch data
	bool isElementContainer(smatch &m){
		string slash_begin =m[1];
		string name=m[2];
		trim(name);
		string slash_end =m[4];
		string txt =m[5];
		return  trim(slash_begin)=="" &&
				trim(name)       !="" &&
				name[0]          !='?'&&
				name[0]          !='!'&&
				name[0]          !=':'&&
				trim(slash_end)  =="" &&
				trim(txt)        =="";
	}
	bool isElementLeaf(smatch &m){
		string slash_begin =m[1];
		string name=m[2];
		string slash_end =m[4];
		string txt =m[5];
		return  trim(slash_begin)=="" &&
				trim(name)       !=""  &&
				trim(slash_end)  =="/" &&
				trim(txt)        =="";
	}
	bool isElementEnd(smatch &m){
		string slash_begin =m[1];
		string name=m[2];
		string txt =m[5];
		return trim(slash_begin)=="/" && trim(name)!="" && trim(txt)=="";
	}
	bool isText(smatch &m){
		string name=m[2].str();
		string txt=m[5].str();
		return trim(txt)!="" && trim(name)=="";
	}
	//node act as token
	XMLNode getNextNode(){
		XMLNode r;
		string ns=getNextString();
		if(s==""){ //End Of Nodes
			r.setType(EON);
			return r;
		}
		if(regex_search (ns,m,e)){
			if(isText(m)){//just text
				r.setType(TEXT);
				string txt=m[5];
				r.setText(txt);
				return r;
			}
			string name=m[2];
			if(isElementContainer(m)){
				r.setType(ELEMENT_CONTAINER);
				r.setName(m[2]);
				r.setAttributes(m[3]);
			}
			if(isElementLeaf(m)){
				r.setType(ELEMENT_LEAF);
				r.setName(m[2]);
				r.setAttributes(m[3]);
			}
			if(isElementEnd(m)){
				r.setType(ELEMENT_END);
				r.setName(m[2]);
				r.setAttributes(m[3]);
			}
		}
		return r;
	}
};
class XMLparser{
	XMLscanner scanner;
public:
	XMLparser(string s):scanner(s){}
	XMLNode parse(XMLNode &parent){
		XMLNode n=scanner.getNextNode();
		while(n.getType()!=EON){
			if(n.getType()==ELEMENT_END){
				if(n.getName()==parent.getName())
					return parent;
				else
					throw runtime_error("XML parsing error: begin tag="+parent.getName()+" not match with end tag="+n.getName());
			}
			if(n.getType()==TEXT){
				parent.setText(n.getText());
			}
			if(n.getType()==ELEMENT_LEAF){
				parent.addchild(n);
			}
			if(n.getType()==ELEMENT_CONTAINER){
				n=parse(n);
				parent.addchild(n);
			}
			n=scanner.getNextNode();
		}
		return parent;
	}
};


#endif /* XML_PARSER_H_ */
