#ifndef PLOTSTRIP_H
#define PLOTSTRIP_H

#include "solver.h"
#include "plot.h"

class CPlotStrip : public CPlot
{
public:
  CPlotStrip(char* ipaddr, char* port, QOpenGLShaderProgram *shader);
  ~CPlotStrip();
  void draw();
  void move(bool increase, double amount);
  void moveto(double target);
  void plot_tick();
  void range(bool increase);
  void reset();
  void toggle_show_vals();
  void toggle_transform();

private:
  CSolver *solver;
  bool transform;

  void calc();
  void setup_axis();
  void setup_data();
};

#endif // PLOTSTRIP_H
