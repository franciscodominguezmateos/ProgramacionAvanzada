/*
 * xml_parser.h
 *
 *  Created on: 24 Mar 2020
 *      Author: Francisco Dominguegz
 */
#pragma once
#include <regex>
#include "util.h"
/*
 * I am trying to avoid polymorphism in order to make it as easy as possible
 * ELEMENT_CONTAINER, ELEMENT_LEAF both are XMLNode not difference in runtime just in parser time.
 * TEXT this is used as a temporal token from scanner to parser
 * UNKNOW comments, declarations and others to discard
 * ELEMENT_END used as temporal token to detect end of ELEMENT_CONTAINER
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
	XMLNode(XMLNodeType t=UNKNOW,string name="root"):type(t),name(name){}
	/*     CHILDREN      */
	inline int size() {return children.size();}
	void addchild(XMLNode n){
		string name=n.getName();
		// name it isn't on the map
		if(children.count(name)==0){
			children[name]=vector<XMLNode>();
		}
		children[name].push_back(n);
	}
	inline map<string, vector<XMLNode> >& getChildren() {return children;	}
	vector<XMLNode> &getChildren(string name) {
		if(hasChild(name))
			return children[name];
		else
			throw runtime_error("Error in XMLNode::getchildren(): not children named "+name+" in node "+getName());
	}
	//friendly access to children with name
	inline vector<XMLNode> &operator[](string name){return getChildren(name);}
	//access to first child in the collection by name
	inline XMLNode &getChild(string name){return getChildren(name)[0];}
	//friendly access to first child or look for a child with a attribute value
	inline XMLNode &operator()(string name,string aname="",string vname=""){
		vector<XMLNode> &vn=getChildren(name);
		if(aname!="" && vname!=""){
			for(XMLNode &n:vn)
				if(n.attributes.count(aname)>0)
					if(n.attributes[aname]==vname)
						return n;
			throw runtime_error("Error in XMLNode::operator(): not attribute="+aname+" with value="+vname+" found in node="+name);
		}
		return vn[0];
	}
	inline bool      hasChild(string name){return children.count(name)>0;}
	inline const int getChildrenSize() const {return children.size();}
	inline void      setChildren(const map<string, vector<XMLNode> > &children) {this->children = children;}
    /*      ACCESSORS    */
	const string& getName() const {return name;	}
	void          setName(const string &name) {this->name = name;}
	string &getText() {return text;}
	void    setText(const string &text) {this->text = text;}
	XMLNodeType getType() const {return type;}
	void        setType(XMLNodeType type) {this->type = type;}
	/*      ATTRIBUTES     */
	void setAttributes(string key_value_pairs){
		string key,value;
		//take away any space at beginning or end
		trim(key_value_pairs);
		vector<string> pairs=split(key_value_pairs);
		for(string pair:pairs){
			vector<string> keyvalue=split(pair,'=');
			if(keyvalue.size()>0){
				key=keyvalue[0];
				if(keyvalue.size()==2){
					value=removeQuotes(keyvalue[1]);
				}else value="";
				attributes[key]=value;
			}
		}
	}
	inline map<string, string>& getAttributes() {return attributes;}
	inline bool    hasAttribute(string name){return attributes.count(name)>0;}
	inline string &getAttribute(string name){
		if(attributes.count(name)>0)
			return attributes[name];
		else
			throw runtime_error("Error in XMLNode::getAttibute(): not children named '"+name+"' in node "+getName());
	}
	inline int   getAttributeInt(string name){return stoi(getAttribute(name));}
	inline float getAttributeFloat(string name){return stof(getAttribute(name));}
	friend ofstream &operator<<(ofstream &ofxml,XMLNode &n);
};
/*         STREAM I/O         */
ostream &operator<<(ostream &ofxml,XMLNode &n){
	ofxml << "<" << n.getName() << " " ;
	// send attributes
	for(auto &pair:n.getAttributes()){
		ofxml << pair.first << "=\"" << pair.second << "\" ";
	}
	// ELEMENT_CONTAINER
	map<string,vector<XMLNode>> &children=n.getChildren();
	if(n.getType()==ELEMENT_CONTAINER){
		//end with >
		ofxml << ">" << endl;
		//if(n.getText()!="") ofxml << n.getText() << endl;
		for(auto &pair:children){
			for(XMLNode &nh:pair.second)
				ofxml << nh;
		}
		ofxml << "</" << n.getName() << ">" << endl;
	}
	// ELEMENT LEAF
	else{
		// end with />
		ofxml <<"/>" << endl;
	}
	return ofxml;
}
