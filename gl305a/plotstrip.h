#ifndef PLOTSTRIP_H
#define PLOTSTRIP_H

#include "plot.h"

class CPlotStrip : public CPlot
{
public:
  CPlotStrip(QOpenGLShaderProgram *shader);
  ~CPlotStrip();
  void draw();
  void move_source(int x, int y);
  void range(bool increase);
  void reset();
  void toggle_show_vals();

private:
  std::complex<double> *source;
  int n_source_verts, n_dest_verts;
  bool show_dest;

  void calc();
  void setup_axis();
  void setup_data();
};

#endif // PLOTSTRIP_H
