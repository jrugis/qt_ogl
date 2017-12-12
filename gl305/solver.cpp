#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <QDebug>

#include "solver.h"

CSolver::CSolver()
{
}

CSolver::~CSolver()
{
  close(sock);
}

void CSolver::die(char const *mess)
{
  qDebug() << mess;
  exit(1);
}

void CSolver::init(char* ip, char* port)
{
  struct sockaddr_in echoserver;

  /* Create the TCP socket */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    die("Failed to create socket.");
  }

  /* Construct the server sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = inet_addr(ip);       /* IP address */
  echoserver.sin_port = htons(atoi(port));          /* server port */

  /* Establish connection */
  qDebug() << "Connecting to server.";
  if (connect(sock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
    die("Failed to connect with server.");
  }
}

char* CSolver::calc(double settings[6])
{
  char buffer[BUFFSIZE];

  // Send data to the server
  strcpy(buffer, ".L01");
  double* vals = (double*)(buffer+8);
  for(int i=0; i<6; i++) vals[i] = settings[i]; // real & imagingary parameters
  int n = vals[2] * vals[5];                    // number of complex values to compute
  *(int*)(buffer+4) = n;
  unsigned int bytes = 16*n;   // expected number of computed bytes
  if(bytes > BUFFSIZE) {
    die("Results will exceed buffer size.");
  }

  //qDebug() << "Sending bytes: " << 7*8 << " ";
  if (send(sock, buffer, 7*8, 0) != 7*8) {
    die("Mismatch in number of sent bytes.");
  }

  // Receive results back from the server
  unsigned int total = 0;
  unsigned int ret;
  while(total < bytes){
    if ((ret=recv(sock, buffer+total, BUFFSIZE-1, 0)) < 1) {
      die("Failed to receive bytes from server.");
    }
    total += ret;
  }
  return buffer;
}

