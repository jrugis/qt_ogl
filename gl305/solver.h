#ifndef SOLVER_H
#define SOLVER_H

#define BUFFSIZE 4096

class CSolver
{
public:
  CSolver();
  ~CSolver();
  void init(char* ip, char* port);
  char* calc(double settings[6]);
private:
  void die(char const *mess);
  int sock;
};

#endif // SOLVER_H
