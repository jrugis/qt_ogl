#define register // used by pari but deprecated in c++11
#include <pari/pari.h>
#undef register

#include "constants.h"
#include "plotstrip.h"

CPlotStrip::CPlotStrip(QOpenGLShaderProgram *shader):CPlot(shader)
{
  solver = new CSolver();
  solver->init("192.168.0.115", "2000");
  transform = false;
  setup_axis();
  setup_data();
  //reset();
}

CPlotStrip::~CPlotStrip()
{
  delete[] source;
  delete solver;
}

void CPlotStrip::calc()
{
  // transform the source
  std::complex<double> *s;
  s = new std::complex<double>[n_source_verts];
  for(int i=0; i<n_source_verts; i++)
    s[i] = source[i] + source_translate;

  // apply source -> destination mapping
  n_dest_verts = n_source_verts;
  std::complex<double> *d;
  d = new std::complex<double>[n_dest_verts];

  double settings[] = {0, 0.05, 21, 0, 0, 1};
  settings[3] = s[0].imag();
  char* cbuf = solver->calc(settings);
  for(int i=0; i<21; i++){
    d[i] = std::complex<double>(*(double*)(cbuf+(16*i)), *(double*)(cbuf+(16*i)+8));
  }

  if(show_vals) qDebug() << "s = (" << s[0].imag()
          << ")   d = (" << d[n_dest_verts/2].real() << "," << d[n_dest_verts/2].imag() << ")";
  if(transform){
    std::complex<double> zeta_one = d[n_dest_verts-1];
    std::complex<double> zeta_zero = d[0];
    double angle = std::arg(zeta_one - zeta_zero);
    double length = std::abs(zeta_one - zeta_zero);
    for(int i=0; i<n_dest_verts; i++){
      d[i] -= zeta_one;
      d[i] = std::polar(abs(d[i]), arg(d[i]) - angle);
      d[i] = d[i] * 10.0 / length;
    }
  }

  // transform and push to vbo
  for(int i=0; i<n_dest_verts; i++){
    d[i] = d[i] * plot_scale + plot_offset;
  }
  QVector3D *buf;
  buf = new QVector3D[n_dest_verts];
  for(int i=0; i<n_dest_verts; i++){
    buf[i]    = QVector3D(d[i].real(),   d[i].imag(),   0.0f);
  }
  vbo_data.bind();
  vbo_data.write(0, buf, n_dest_verts*sizeof(QVector3D));
  vbo_data.release();
  delete[] buf;
  delete[] d;
  delete[] s;
}

void CPlotStrip::draw()
{
  //glLineWidth(12.0f);   // not supported on mac air
  //glEnable(GL_LINE_SMOOTH);
  //glDisable(GL_LINE_SMOOTH);

  // draw axis
  vao_axis.bind();
  // horizontal and vertical grid lines
  shader->setUniformValue(u_color, QVector4D(VERY_LIGHT_GRAY, 1.0f));
  glDrawArrays(GL_LINES, 0, 2*(plot_range+1));
  glDrawArrays(GL_LINES, 2*(plot_range+1), 2*(plot_range+1));
  shader->setUniformValue(u_color, QVector4D(MED_GRAY, 1.0f));
  glDrawArrays(GL_LINES, plot_range, 2); // center
  glDrawArrays(GL_LINES, 3*plot_range+2, 2);
  vao_axis.release();

  // draw data: destination
  vao_data.bind();
  glPointSize(3.0f);
  shader->setUniformValue(u_color, QVector4D(BLUE, 1.0f));
  glDrawArrays(GL_POINTS, 0, n_dest_verts); // points
  glDrawArrays(GL_LINE_STRIP, 0, n_dest_verts);  // lines
  shader->setUniformValue(u_color, QVector4D(RED, 1.0f));
  glDrawArrays(GL_POINTS, n_dest_verts/2, 1); // point
  vao_data.release();
}

void CPlotStrip::move_source(int x, int y)
{
  const float scale = 0.001f / plot_scale;
  source_translate += std::complex<double>(0.0, y * scale);
  calc();
}

void CPlotStrip::move(bool increase)
{
  if(increase) source_translate += std::complex<double>(0.0, 100.0);
  else source_translate -= std::complex<double>(0.0, 100.0);
  calc();
}

void CPlotStrip::moveto(double target)
{
  source_translate = std::complex<double>(0.0, target);
  calc();
}

void CPlotStrip::plot_tick()
{
  source_translate += std::complex<double>(0.0, 0.01);
  calc();
}

void CPlotStrip::range(bool increase)
{
  if(increase){
    plot_range += 2;
    plot_zero += std::complex<double>(1.0, 1.0);
  }
  else{
    if(plot_range > 2){
      plot_range -= 2;
      plot_zero -= std::complex<double>(1.0, 1.0);
    }
  }
  vao_axis.destroy();
  vbo_axis.destroy();
  setup_axis();
  move_source(0, 0);
}

void CPlotStrip::reset()
{
  //source_translate = std::complex<double>(0.0, 0.0);
  source_translate = std::complex<double>(0.0, 1005.0);
  calc();
}

void CPlotStrip::setup_axis()
{
  // axis vertices
  QVector3D *axis_verts = new QVector3D[4*(plot_range+1)];
  const float pr_float = static_cast<float>(plot_range); // convenience typecast
  for(int i=0; i<plot_range+1; i++){
    // horizontal lines
    axis_verts[2*i]            = QVector3D(-10.0f, i/pr_float, 0.0f);
    axis_verts[2*i+1]          = QVector3D(10.0f, i/pr_float, 0.0f);
    // vertical lines
    axis_verts[2*(plot_range+1)+2*i]   = QVector3D(i/pr_float, -10.0f, 0.0f);
    axis_verts[2*(plot_range+1)+2*i+1] = QVector3D(i/pr_float, 10.0f, 0.0f);
  }

  // axis vbo, vao
  vbo_axis.create();  // vbo
  vbo_axis.bind();
  vbo_axis.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_axis.allocate(axis_verts, 4*(plot_range+1) * sizeof(axis_verts[0]));
  vao_axis.create();  // vao
  vao_axis.bind();
  shader->enableAttributeArray(0);
  shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  vao_axis.release();
  vbo_axis.release();
  delete[] axis_verts;

  // set plot scale and calculate clamp limits
  plot_scale = 1.0 / plot_range;
  plot_offset = plot_zero * plot_scale;
  re_lo = -plot_offset.real() / plot_scale;
  re_hi = (1.0 - plot_offset.real()) / plot_scale;
  im_lo = -plot_offset.imag() / plot_scale;
  im_hi = (1.0 - plot_offset.imag()) / plot_scale;
}

void CPlotStrip::setup_data()
{
  // data vbo, vao
  const QVector3D data_verts[MAX_DATA_VERTS] = {};
  vbo_data.create();  // vbo
  vbo_data.bind();
  vbo_data.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  vbo_data.allocate(data_verts, sizeof(data_verts));
  vao_data.create();  // vao
  vao_data.bind();
  shader->enableAttributeArray(0);
  shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  vao_data.release();
  vbo_data.release();

  n_source_verts = N_SPIRAL_VERTS;
  source = new std::complex<double>[n_source_verts];
  for(int i=0; i<n_source_verts; i++)
    source[i] = std::complex<double>(1.00*i/(n_source_verts-1), 0.0);
}

void CPlotStrip::toggle_show_vals()
{
  show_vals ^= 1;
  calc();
}

void CPlotStrip::toggle_transform()
{
  transform ^= 1;
  calc();
}
