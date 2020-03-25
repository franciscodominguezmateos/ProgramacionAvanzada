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
	const map<string, vector<XMLNode> >& getChildren() const {return children;	}
	vector<XMLNode> &getChildren(string name) {
		if(hasChild(name))
			return children[name];
		else
			throw runtime_error("Error in XMLNode::getchildren(): not children named "+name+" in node "+getName());
	}
	inline XMLNode &getChild(string name){return getChildren(name)[0];}
	//friendly access
	inline XMLNode &operator[](string name){return getChild(name);}
	inline bool hasChild(string name){return children.count(name)>0;}
	inline const int getChildrenSize() const {return children.size();}
	inline void setChildren(const map<string, vector<XMLNode> > &children) {this->children = children;}
    /*      ACCESSORS    */
	const string& getName() const {return name;	}
	void setName(const string &name) {this->name = name;}
	const string& getText() const {return text;}
	void setText(const string &text) {this->text = text;}
	XMLNodeType getType() const {return type;}
	void setType(XMLNodeType type) {this->type = type;}
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
	inline const map<string, string>& getAttributes() const {return attributes;}
	inline string &getAttibute(string name){
		if(attributes.count(name)>0)
			return attributes[name];
		else
			throw runtime_error("Error in XMLNode::getAttibute(): not children named "+name+" in node "+getName());
	}
	//friendly access
	inline string &operator()(string name){return getAttibute(name);}
	friend ofstream &operator<<(ofstream &ofxml,const XMLNode &n);
};
//this class has a limitation on the length of token
class XMLScanner{
	//from: https://rextester.com/WGBY40008
	string s;
	smatch m;
	regex e;
public:
	XMLScanner(string s=""):s(s),e("<(/*)(\\S+?)\\b(.*?)(/*)>|([^<]+)"){}
	inline void setString(string &rs){s=rs;}
	inline string &getString(){return s;}
	//this function has a problem when the patter is bigger than 1000 characters approximately
	virtual string getNextString() try{
		cout << "getNextString ";
		bool ok=true;
		while(regex_search (s,m,e, regex_constants::match_any)){
			string r= m[0];
			s = m.suffix();
			ok=trim(r)!="";
			if(ok)
				return r;
		}
		return "";
	}
	catch (exception &e){
		cout <<"Error in XMLscanner::getNextString():" <<e.what() << endl;
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
		cout << ns << endl;
		if(ns==""){ //End Of Nodes
			r.setType(EON);
			return r;
		}
		//special case that regx_search can't cope with
		//it is suppose that a so long string is for XML text
		if(ns.length()>500){
			r.setType(TEXT);
			string txt=ns;
			r.setText(txt);
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
//this class does NOT have a limitation on the length of token
//it just redefine getNextString()
// it could be straightforward extended to work from files
class XMLScannerStrong: public XMLScanner{
	unsigned int state;
	unsigned int idx;
public:
	XMLScannerStrong(string s=""):XMLScanner(s),state(0),idx(0){}
	char getChar(){
		string &s=getString();
		if(idx>=s.length())
			throw runtime_error("EOF in XMLScannerStrong::getChar()");
		char c=s[idx++];
		return c;
	}
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
};
class XMLParser{
	XMLNode n;
	XMLScannerStrong scanner;
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
};
/*         STREAM I/O         */
ostream &operator<<(ostream &ofxml,const XMLNode &n){
	ofxml << "<" << n.getName() << " " ;
	// send attributes
	for(auto &pair:n.getAttributes()){
		ofxml << pair.first << "=\"" << pair.second << "\" ";
	}
	// ELEMENT_CONTAINER
	const map<string,vector<XMLNode>> &children=n.getChildren();
	if(children.size()>0){
		//end with >
		ofxml << ">" << endl;
		if(n.getText()!="") ofxml << n.getText() << endl;
		for(auto &pair:children){
			for(const XMLNode &nh:pair.second)
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
ifstream &operator>>(ifstream &ifxml,XMLNode &n){
	string s;
	ifxml >> s;
	XMLParser p;
	n=XMLNode(p.parse(s));
	return ifxml;
}

#endif /* XML_PARSER_H_ */
