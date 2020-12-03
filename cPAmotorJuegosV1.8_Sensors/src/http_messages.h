/*
 * http_request.h
 *
 *  Created on: 11 Dec 2019
 *      Author: Francisco Dominguez
 */

#ifndef HTTP_MESSAGES_H_
#define HTTP_MESSAGES_H_
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "util.h"

using namespace std;

// Easy class to process CGI parameters. It does NOT make encoding!!
class CGI{
    map<string,string> params;
public:
	CGI(){}
	CGI(string s){
		vector<string> v=split(s,'&');
		for(string sp:v){
			vector<string> vp=split_first(sp,'=');
			params[vp[0]]=vp[1];
		}
	}
	inline map<string,string> &getParams(){return params;}
	inline void add(string k,string v){params[k]=v;}
	inline void add(string k,int v)   {params[k]=to_string(v);}
	inline void add(string k,double v){params[k]=to_string(v);}
	inline void add(string k,char v)  {params[k]=to_string(v);}
	inline string &operator[](string key){return params[key];}
	inline char   getChar  (string key)  const {return (char)stoi(params[key]);}
	inline int    getInt   (string key)  const {return stoi(params[key]);}
	inline float  getFloat (string key)  const {return stof(params[key]);}
	inline double getDouble(string key)  const {return stod(params[key]);}
	inline bool isEmpty(){return params.empty();}
	inline void clear()  {params.clear();}
	inline int  size()  const     {return params.size();}
	inline bool hasKey(string key){return params.count(key)>0;}
	friend ostream &operator<<(ostream &so,const CGI &m){
		// Are there CGI parameters?
		int cnt=0;
		for(pair<string,string> k:m.params){
			if(cnt!=0) so << "&"; //no put & at first position
			so <<  k.first <<"="<< k.second;
			cnt++;
		}
		return so;
	}
	string toString(){
		ostringstream oss;
		oss << *this;
		return oss.str();
	}
};
// Common class for Request and Response HTTP messages
class HttpMessage {
protected:
	string headline;
	map<string,string> header;
	string body;
public:
	HttpMessage(){}
	HttpMessage(string s){
		bool hasBody=false;
		istringstream iss(s);
		string line;
		// First line
		getline(iss,headline);
		vector<string> v;
		// lines of header in format key:value
		while (getline(iss,line)){
			v=split_first(line,':');
			// a empty line very likely line==""
			if(v.size()==0){
					hasBody=true;
					break;
			}
			string &key  =v[0];
			string &value=v[1];
			header[key]=value;
		}
		// just read all lines and put them in the body
		if(hasBody)	while (getline(iss,line)) body+=line;
	}
	string &operator[](string key){return header[key];}
	const string& getBody() const {return body;}
	void setBody(const string &body) {this->body = body;setContentLengthFromBody();}
	void setContentLengthFromBody(){
		int l=body.size();
		string sl=to_string(l);
		header["Content-Lenght"]=sl;
	}
	friend ostream &operator<<(ostream &so,const HttpMessage &m){
		for(pair<string,string> k:m.header)
			so << k.first <<":"<< k.second <<endl;
		so << endl;
		so << m.body;
		return so;
	}
	string toString(){
		ostringstream oss;
		oss << *this;
		return oss.str();
	}
};
class HttpRequest:public HttpMessage {
	string url;
	string method;
    string version;
    CGI params;
public:
	HttpRequest():HttpRequest("/","GET"){}
	HttpRequest(string url,string method/*="GET"*/,string version="HTTP/1.0"):url(url),method(method),version(version){}
	HttpRequest(string s):HttpMessage(s){
		istringstream iss(headline);
		string line;
		// First line
		getline(iss,line,'\r');
		vector<string> v=split(line,' ');
		method =v[0];
		version=v[2];
		if(v[1].find('?')!=string::npos){
			//processing CGI parameters
			v=split_first(v[1],'?');
			url    =v[0];
			params=CGI(v[1]);
		}
		else
			url=v[1];
	}
	const string& getMethod() const {return method;}
	void setMethod(const string &method) {this->method = method;}
	const string& getUrl() const {return url;}
	void setUrl(const string &url) {this->url = url;}
	const string& getVersion() const {return version;}
	void setVersion(const string &version) {this->version = version;}
	const CGI& getParams() const {return params;}
	void setParams(const CGI &params) {this->params = params;}
	const string &getParam (const string &k) {return params[k];}
	void setParam(const string &k,const string &v){ this->params[k]=v;}
	friend ostream &operator<<(ostream &so,const HttpRequest &m){
		so << m.method << " ";
		so << m.url;
		// Are there CGI parameters?
		if(m.params.size()!=0)
			so<<"?"<<m.params;
		so<<" ";
		so << m.version << endl;
		//so << static_cast<HttpMessage>(m) ;
		so << (HttpMessage)m ;
		return so;
	}
	string toString(){
		ostringstream oss;
		oss << *this;
		return oss.str();
	}
};
class HttpResponse:public HttpMessage {
	string status;
    string statusMessage;
	string version;
public:
	HttpResponse():HttpResponse("200","OK"){}
	HttpResponse(string status,string statusMessage,string version="HTTP/1.0"):status(status),statusMessage(statusMessage),version(version){}
    HttpResponse(string s):HttpMessage(s){
		istringstream iss(headline);
		string line;
		// First line
		getline(iss,line,'\r');
		vector<string> v=split(line,' ');
		version      =v[0];
		status       =v[1];
		statusMessage=v[2];
	}
	const string& getStatus() const {return status;}
	void setStatus(const string &status) {this->status = status;}
	const string& getStatusMessage() const {return statusMessage;}
	void setStatusMessage(const string &statusMessage) {this->statusMessage = statusMessage;}
	const string& getVersion() const {return version;}
	void setVersion(const string &version) {this->version = version;}
	friend ostream &operator<<(ostream &so,const HttpResponse &m){
		so << m.version << " ";
		so << m.status << " ";
		so << m.statusMessage << endl;
		//so << static_cast<HttpMessage>(m) ;
		so << (HttpMessage)m ;
		return so;
	}
	string toString(){
		ostringstream oss;
		oss << *this;
		return oss.str();
	}
};
class StringProcessor{
public:
	virtual string process(string &si)=0;
};
// Abstract for http message processor
class HttpProcessor{
public:
	virtual HttpResponse process( HttpRequest &req)=0;
};

#endif /* HTTP_MESSAGES_H_ */
