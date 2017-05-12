#ifndef PLOTSTRIP_H
#define PLOTSTRIP_H

#define N_SPIRAL_VERTS 21
#define MAX_DATA_VERTS 100

#include "plot.h"

class CPlotStrip : public CPlot
{
public:
  CPlotStrip(QOpenGLShaderProgram *shader);
  ~CPlotStrip();
  void draw();
  void move_source(int x, int y);
  void move(bool increase);
  void moveto(double target);
  void plot_tick();
  void range(bool increase);
  void reset();
  void toggle_show_vals();
  void toggle_transform();

private:
  std::complex<double> *source;
  int n_source_verts, n_dest_verts;
  bool transform;

  void calc();
  void setup_axis();
  void setup_data();
};

#endif // PLOTSTRIP_H
