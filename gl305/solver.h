#ifndef SOLVER_H
#define SOLVER_H

#define N_VERTS 21
#define BUFFSIZE 4096

// parameters for the solver server
struct parms{
  char token[4];
  int count;
  double u_base;
  double u_increment;
  double u_count;
  double v_base;
  double v_increment;
  double v_count;
};

// the solver client
class CSolver
{
public:
  CSolver(char* ip, char* port);
  ~CSolver();
  void calc(double source_v, char* buffer);
private:
  void die(char const *mess);
  int sock;
};

#endif // SOLVER_H
