//
// Created by root on 2020/6/21.
//

#ifndef SHRINK_TC_CLION_TCPSERVICE_H
#define SHRINK_TC_CLION_TCPSERVICE_H

#include <stdio.h>          // for printf() and fprintf()
#include <sys/socket.h>     // for socket(), bind(), and connect()
#include <arpa/inet.h>      // for sockaddr_in and inet_ntoa()
#include <stdlib.h>         // for atoi() and exit()
#include <string.h>         // for memset()
#include <unistd.h>         // for close()
#include <sys/time.h>       // for struct timeval {}
#include <fcntl.h>          // for fcntl()
#include <pthread.h>
#include <sys/epoll.h>
#include "shirink_app.h"

#define MAXPENDING 5        // Maximum outstanding connection requests
#define MAXCLIENT  100      // Maximum client connections
#define RCVBUFSIZE 1024     // Size of receive buffer
class TCPService {
 public:
  TCPService();
  virtual ~TCPService();
  int CreateTCPServiceSocket(unsigned short);
  int AcceptTCPConnection(int);
  int HandleTCPClient(int);
  static void *pthread_TCPServiceThread(void *);//
  void tcp_thread_generate();//
  static shirink_app getTcData();
 protected:
 private:
  bool
  parsTCP_JsonV3Content(char *buff, int connfd);
  string StringForSend;
  // shirink_app ShrinkAppDriver;


};
extern TCPService _tcpservice;


#endif //SHRINK_TC_CLION_TCPSERVICE_H
