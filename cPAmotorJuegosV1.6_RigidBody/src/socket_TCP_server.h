//============================================================================
// Name        : cOpenCV3StreamingServer.cpp
// Author      : Francisco Dominguez
// Version     : 2019/11/12
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
// From: https://gist.github.com/Tryptich/2a15909e384b582c51b5
// 10/11/2019 modified to send jpeg compressed images
//============================================================================
#ifndef SOCKET_TCP_SERVER_H_
#define SOCKET_TCP_SERVER_H_
#include <vector>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <thread>
#include "http_messages.h"

using namespace std;
using namespace cv;
/*
int socket_TCP_server_bind(int port=4097){
    //--------------------------------------------------------
    //networking stuff: socket, bind, listen
    //--------------------------------------------------------
    int localSocket;
    struct  sockaddr_in localAddr;
    //int addrLen = sizeof(struct sockaddr_in);

    localSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (localSocket == -1){
        perror("socket() call failed!!");
        return localSocket;
    }

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons( port );

    if( bind(localSocket,(struct sockaddr *)&localAddr , sizeof(localAddr)) < 0) {
        perror("Can't bind() socket");
        return localSocket;
    }

    // Listening
    listen(localSocket , 3);
    std::cout <<  "Waiting for connections...\n"
              <<  "Server Port:" << port << std::endl;
    return localSocket;
}
string receive_string(int socket){
    int bytes = 0;
	//Receive size l max length
    int l=100000;
    vector<char> buf(l);
    bytes = recv(socket, buf.data(), l, 0); // MSG_WAITALL
    if (bytes <= -1) {
        throw(runtime_error("receive_string failed on recv"));
    }
    buf.resize(bytes);
    string s(buf.begin(),buf.end());
    //for(int i=0;i<bytes;i++) s+=buf[i];
    return s;
}
bool send_string(int socket,string buf){
	int bytes=0;
    //send data
	bytes = send(socket, buf.data(), buf.size(), 0);
    if ( bytes< 0){
        throw(runtime_error("send_string failed on send"));
    }
    return true;
}
bool send_uncompressed_image(int socket,Mat& img){
	int bytes=0;
    // send the uncompressed img frame over the network
    int imgSize = img.total() * img.elemSize();
    //std::cout << "Image Size:" << imgSize << std::endl;
	if ((bytes = send(socket, img.data, imgSize, 0)) < 0){
        std::cerr << "send_uncompressed_image bytes = " << bytes << std::endl;
        return false;
    }
    return true;
}
bool send_jpeg_image(int socket,Mat& img){
	int bytes=0;
    vector<uchar> buf;
    // if the image is empty return
    if(img.empty()) return true;
    // compress as jpeg from matrix img to bytes vector buf
    imencode(".jpg",img,buf);
    uint l=buf.size();
    //send size
    if ((bytes = send(socket, &l, sizeof(l), 0)) < 0){
        std::cerr << "send_jpeg_image bytes = " << bytes << std::endl;
        return false;
    }
    //send data
    if ((bytes = send(socket, buf.data(), buf.size(), 0)) < 0){
        std::cerr << "send_jpeg_image bytes = " << bytes << std::endl;
        return false;
    }
    return true;
}
bool send_image(int socket,Mat& img,bool compressed=true){
	if(compressed)
		return send_jpeg_image(socket,img);
	return send_uncompressed_image(socket,img);
}
void send_image_thread(bool* stop,void *ptr){
    int socket = *(int *)ptr;
    //OpenCV Code
    //----------------------------------------------------------
    Mat img, flippedFrame;
    while(!(*stop)){
    	img=global_img;
        // flip the frame
        //flip(img, flippedFrame, 0);
        //resize(img,img,Size(),0.5,0.5);

        if(!send_image(socket,img))
        	break;
		this_thread::sleep_for(chrono::milliseconds(30));
    }
    close(socket);
}
void video_jpeg_stream_server(bool* pstop,int port=4097){
	bool &stop=*pstop;
    //--------------------------------------------------------
    //networking stuff: socket, bind, listen
    //--------------------------------------------------------
    int localSocket,
        remoteSocket;
    struct  sockaddr_in remoteAddr;
    int addrLen = sizeof(struct sockaddr_in);

    thread *s_t;
    //thread *r_t;
    vector<thread*> threads;

    // Start this TCP server
    localSocket=socket_TCP_server_bind(port);
    if(localSocket<0) exit(0);

    //accept connection from incoming clients
    while(!stop){
    	// !!!!!!!!!!!!!!!!!!!!!!!!!!!
    	// problem with thread since this is a blocking function
        remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);
        if (remoteSocket < 0) {
            perror("accept failed!");
            break;
        }
        std::cout << "Connection accepted" << std::endl;
        s_t=new thread(send_image_thread,pstop,&remoteSocket);
        threads.push_back(s_t);
        //r_t=new thread(recv_data,&remoteSocket);
    }
    //join all threads and delete memory
    for(thread* t:threads){
    	if(t->joinable()) t->join();
    	//remove from threads?
    	delete t;
    }
    close(localSocket);
}
void http_server(bool* pstop,HttpProcessor* hp,int port=8881){
    //--------------------------------------------------------
    //networking stuff: socket, bind, listen
    //--------------------------------------------------------
	bool &stop=*pstop;
    int localSocket,
        remoteSocket;
    struct  sockaddr_in remoteAddr;
    int addrLen = sizeof(struct sockaddr_in);

    // Start this TCP server
    localSocket=socket_TCP_server_bind(port);
    if(localSocket<0) exit(0);

    //accept connection from incoming clients
    while(!stop){
    	// !!!!!!!!!!!!!!!!!!!!!!!!!!!
    	// problem with thread since this is a blocking function
        remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);
        if (remoteSocket < 0) {
            perror("accept failed!");
            break;
        }
        std::cout << "Connection accepted" << std::endl;
		string s=receive_string(remoteSocket);
		HttpRequest req(s);
		HttpResponse r=hp->process(req);
		send_string(remoteSocket,r.toString());
		close(remoteSocket);
    }
    close(localSocket);
}
void string_server(bool* pstop,StringProcessor* sp,int port=8881){
    //--------------------------------------------------------
    //networking stuff: socket, bind, listen
    //--------------------------------------------------------
	bool &stop=*pstop;
    int localSocket,
        remoteSocket;
    struct  sockaddr_in remoteAddr;
    int addrLen = sizeof(struct sockaddr_in);

    // Start this TCP server
    localSocket=socket_TCP_server_bind(port);
    if(localSocket<0) exit(0);

    //accept connection from incoming clients
    while(!stop){
    	// !!!!!!!!!!!!!!!!!!!!!!!!!!!
    	// problem with thread since this is a blocking function
        remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);
        if (remoteSocket < 0) {
            perror("accept failed!");
            break;
        }
        //thread string_dispatch_th(string_server_dispatch_thread,remoteSocket,sp);
    	string si=receive_string(remoteSocket);
    	string so=sp->process(si);
    	send_string(remoteSocket,so);
    	close(remoteSocket);
    }
    close(localSocket);
}*/
// This class is just a package of methods
class SocketTCPServer{
public:
	static int socket_TCP_server_bind(int port=4097){
	    //--------------------------------------------------------
	    //networking stuff: socket, bind, listen
	    //--------------------------------------------------------
	    int localSocket;
	    struct  sockaddr_in localAddr;
	    //int addrLen = sizeof(struct sockaddr_in);

	    localSocket = socket(AF_INET , SOCK_STREAM , 0);
	    if (localSocket == -1){
	        perror("socket() call failed!!");
	        return localSocket;
	    }

	    localAddr.sin_family = AF_INET;
	    localAddr.sin_addr.s_addr = INADDR_ANY;
	    localAddr.sin_port = htons( port );

	    if( bind(localSocket,(struct sockaddr *)&localAddr , sizeof(localAddr)) < 0) {
	        perror("Can't bind() socket");
	        return localSocket;
	    }

	    // Listening
	    listen(localSocket , 3);
	    std::cout <<  "Waiting for connections...\n"
	              <<  "Server Port:" << port << std::endl;
	    return localSocket;
	}
	static string receive_string(int socket){
	    int bytes = 0;
		//Receive size l max length
	    int l=100000;
	    vector<char> buf(l);
	    bytes = recv(socket, buf.data(), l, 0 /* MSG_WAITALL*/);
	    if (bytes <= -1) {
	        throw(runtime_error("SocketTCPServerString::receive_string failed on recv"));
	    }
	    buf.resize(bytes);
	    string s(buf.begin(),buf.end());
	    //for(int i=0;i<bytes;i++) s+=buf[i];
	    return s;
	}
	static bool send_string(int socket,string buf){
		int bytes=0;
	    //send data
		bytes = send(socket, buf.data(), buf.size(), 0);
	    if ( bytes< 0){
	        throw(runtime_error("SocketTCPServerString::send_string failed on send"));
	    }
	    return true;
	}
	static bool send_uncompressed_image(int socket,Mat& img){
		int bytes=0;
	    // send the uncompressed img frame over the network
	    int imgSize = img.total() * img.elemSize();
	    //std::cout << "Image Size:" << imgSize << std::endl;
		if ((bytes = send(socket, img.data, imgSize, 0)) < 0){
	        std::cerr << "send_uncompressed_image bytes = " << bytes << std::endl;
	        return false;
	    }
	    return true;
	}
	static bool send_jpeg_image(int socket,Mat& img){
		int bytes=0;
	    vector<uchar> buf;
	    // if the image is empty return
	    if(img.empty()) return true;
	    // compress as jpeg from matrix img to bytes vector buf
	    imencode(".jpg",img,buf);
	    uint l=buf.size();
	    //send size
	    if ((bytes = send(socket, &l, sizeof(l), 0)) < 0){
	        std::cerr << "send_jpeg_image bytes = " << bytes << std::endl;
	        return false;
	    }
	    //send data
	    if ((bytes = send(socket, buf.data(), buf.size(), 0)) < 0){
	        std::cerr << "send_jpeg_image bytes = " << bytes << std::endl;
	        return false;
	    }
	    return true;
	}
	static bool send_image(int socket,Mat& img,bool compressed=true){
		if(compressed)
			return send_jpeg_image(socket,img);
		return send_uncompressed_image(socket,img);
	}};
class SocketTCPStringServer:public SocketTCPServer{
	StringProcessor* sp;
	int port;
	bool stop;
	thread* pstring_th;
public:
	SocketTCPStringServer(StringProcessor* sp,int port=8881):sp(sp),port(port),stop(false){
		pstring_th=new thread(*this);
		 // wait a minute
		 this_thread::sleep_for(chrono::milliseconds(100));
	}
	void start(){
	    //--------------------------------------------------------
	    //networking stuff: socket, bind, listen
	    //--------------------------------------------------------
	    int localSocket,
	        remoteSocket;
	    struct  sockaddr_in remoteAddr;
	    int addrLen = sizeof(struct sockaddr_in);

	    // Start this TCP server
	    localSocket=socket_TCP_server_bind(port);
	    if(localSocket<0)
	    	runtime_error("Error in ServerString::start localSocket error after calling socket_TCP_server_bind(port)");

	    //accept connection from incoming clients
	    while(!stop){
	    	// !!!!!!!!!!!!!!!!!!!!!!!!!!!
	    	// problem with thread since this is a blocking function
	        remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);
	        if (remoteSocket < 0) {
	            perror("accept failed!");
	            break;
	        }
	    	string si=receive_string(remoteSocket);
	    	string so=sp->process(si);
	    	send_string(remoteSocket,so);
	    	close(remoteSocket);
	    }
	    close(localSocket);
	}
	//This is the method that thread invoke
	void operator()(){
		start();
	}
};
class SocketHTTPServer:public SocketTCPServer{
	HttpProcessor* hp;
	int port;
	bool stop;
	thread* pstring_th;
public:
	SocketHTTPServer(HttpProcessor* hp,int port=8880):hp(hp),port(port),stop(false){
		pstring_th=new thread(*this);
		 // wait a minute
		 this_thread::sleep_for(chrono::milliseconds(100));
	}
	void start(){
		//--------------------------------------------------------
		//networking stuff: socket, bind, listen
		//--------------------------------------------------------
		int localSocket,
			remoteSocket;
		struct  sockaddr_in remoteAddr;
		int addrLen = sizeof(struct sockaddr_in);

		// Start this TCP server
		localSocket=socket_TCP_server_bind(port);
		if(localSocket<0)
			runtime_error("Error in SocketHTTPServer::start() localSocket error after calling socket_TCP_server_bind(port)");

		//accept connection from incoming clients
		while(!stop){
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!
			// problem with thread since this is a blocking function
			remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);
			if (remoteSocket < 0) {
				perror("accept failed!");
				break;
			}
			//std::cout << "SocketHTTPServer: Connection accepted" << std::endl;
			string s=receive_string(remoteSocket);
			HttpRequest req(s);
			HttpResponse r=hp->process(req);
			send_string(remoteSocket,r.toString());
			close(remoteSocket);
		}
		close(localSocket);
	}
	//This is the method that thread invoke
	void operator()(){
		start();
	}
};
class SocketMJPEGServer:public SocketTCPServer{
	int port;
	bool stop;
	thread* pstring_th;
	Mat img;
	//mutex mtxImg;

public:
	SocketMJPEGServer(int port=4097):port(port),stop(false){
		pstring_th=new thread(*this,&img);
		 // wait a minute
		 this_thread::sleep_for(chrono::milliseconds(100));
	}
	void setImg(Mat image){
		//mtxImg.lock();
		img=image;
		//mtxImg.unlock();
	}
	static void send_image_thread(bool* stop,void *ptr,Mat* pimg){
	    int socket = *(int *)ptr;
	    //OpenCV Code
	    //----------------------------------------------------------
	    Mat simg, flippedFrame;
	    while(!(*stop)){
	    	simg=*pimg;
	        // flip the frame
	        //flip(img, flippedFrame, 0);
	        //resize(img,img,Size(),0.5,0.5);
            /*if(simg.empty()){
            	cout<< "empty in send_image.."<<endl;
            	break;
            }*/
    		//mtxImg.lock();
    		int ok=send_image(socket,simg);
    		//mtxImg.unlock();
	        if(!ok){
				perror("send_image in send_image_thread failed!");
	        	break;}
			this_thread::sleep_for(chrono::milliseconds(30));
	    }
	    close(socket);
	}
	void start(Mat* pimg){
		//--------------------------------------------------------
		//networking stuff: socket, bind, listen
		//--------------------------------------------------------
		int localSocket,
			remoteSocket;
		struct  sockaddr_in remoteAddr;
		int addrLen = sizeof(struct sockaddr_in);

		thread *s_t;
		//thread *r_t;
		vector<thread*> threads;

		// Start this TCP server
		localSocket=socket_TCP_server_bind(port);
		if(localSocket<0) exit(0);

		//accept connection from incoming clients
		while(!stop){
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!
			// problem with thread since this is a blocking function
			remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);
			if (remoteSocket < 0) {
				perror("accept failed!");
				break;
			}
			std::cout << "Connection accepted at SocketMJPEGServer port=" << port<< std::endl;
			if(img.empty())
				std::cout << "img empty port=" << port<< std::endl;
			s_t=new thread(send_image_thread,&stop,&remoteSocket,pimg);
			threads.push_back(s_t);
			//r_t=new thread(recv_data,&remoteSocket);
		}
		//join all threads and delete memory
		for(thread* t:threads){
			if(t->joinable()) t->join();
			//remove from threads?
			delete t;
		}
		close(localSocket);
	}
	//This is the method that thread invoke
	void operator()(Mat* pimg){
		start(pimg);
	}
};
//Mat SocketMJPEGServer::img;
//mutex SocketMJPEGServer::mtxImg;

#endif /* SOCKET_TCP_SERVER_H_ */
