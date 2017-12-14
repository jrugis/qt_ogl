#include <unistd.h>
#include <arpa/inet.h>
#include <QDebug>

#include "constants.h"
#include "solver.h"

CSolver::CSolver(char* ip, char* port)
{
  struct sockaddr_in echoserver;

  // Create the TCP socket
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    die("Failed to create socket.");
  }

  // Construct the server sockaddr_in structure
  memset(&echoserver, 0, sizeof(echoserver));       /* clear struct */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = inet_addr(ip);       /* IP address */
  echoserver.sin_port = htons(atoi(port));          /* server port */

  // Establish connection
  qDebug() << "Connecting to server.";
  if (connect(sock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
    die("Failed to connect with server.");
  }
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

void CSolver::calc(double source_v, char* buffer)
{
  // Send data to the server
  // horizontal source line from 0 to 1, N_VERTS complex points
  parms prms = {{'.','L','0','1'}, N_VERTS, 0, 0.05, N_VERTS, source_v, 0, 1};
  unsigned int bytes = 2 * N_VERTS * sizeof(double);
  if(bytes > BUFFSIZE) die("Results will exceed buffer size.");
  if (send(sock, &prms, sizeof(prms), 0) != sizeof(prms)) {
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
  return;
}

