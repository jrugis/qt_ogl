#include "constants.h"
#include "plotstrip.h"

CPlotStrip::CPlotStrip(char* ipaddr, char* port, QOpenGLShaderProgram *shader):CPlot(shader)
{
  solver = new CSolver(ipaddr, port);
  transform = false;
  setup_axis();
  setup_data();
}

CPlotStrip::~CPlotStrip()
{
  delete solver;
}

void CPlotStrip::calc()
{
  std::complex<double> *d;
  d = new std::complex<double>[N_VERTS+1];

  char cbuf[BUFFSIZE];
  solver->calc(source_v, cbuf);        // call the remote zeta solver
  for(int i=0; i<N_VERTS; i++){   // repackage the results
    d[i] = std::complex<double>(*(double*)(cbuf+(16*i)), *(double*)(cbuf+(16*i)+8));
  }
  d[N_VERTS] = std::complex<double>(0,0);
  if(show_vals) qDebug() << qSetRealNumberPrecision(12) << "v = " << source_v;

  if(transform){
    std::complex<double> zeta_one = d[N_VERTS-1];
    std::complex<double> zeta_zero = d[0];
    double angle = std::arg(zeta_one - zeta_zero);
    double length = std::abs(zeta_one - zeta_zero);
    for(int i=0; i<(N_VERTS+1); i++){
      d[i] -= zeta_one;
      d[i] = std::polar(abs(d[i]), arg(d[i]) - angle);
      d[i] = d[i] * 10.0 / length;
    }
  }

  // transform and push to vbo
  for(int i=0; i<(N_VERTS+1); i++){
    d[i] = d[i] * plot_scale + plot_offset;
  }
  QVector3D *buf;
  buf = new QVector3D[N_VERTS+1];
  for(int i=0; i<(N_VERTS+1); i++){
    buf[i] = QVector3D(d[i].real(), d[i].imag(), 0.0f);
  }
  vbo_data.bind();
  vbo_data.write(0, buf, (N_VERTS+1)*sizeof(QVector3D));
  vbo_data.release();
  delete[] buf;
  delete[] d;
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
  glDrawArrays(GL_POINTS, 0, N_VERTS); // points
  glDrawArrays(GL_LINE_STRIP, 0, N_VERTS);  // lines
  shader->setUniformValue(u_color, QVector4D(RED, 1.0f));
  glDrawArrays(GL_POINTS, N_VERTS/2, 1); // point
  shader->setUniformValue(u_color, QVector4D(BLACK3, 1.0f));
  glDrawArrays(GL_POINTS, N_VERTS, 1); // point
  vao_data.release();
}

void CPlotStrip::move(bool increase, double amount)
{
  if(increase) source_v += amount;
  else source_v -= amount;
  calc();
}

void CPlotStrip::moveto(double target)
{
  source_v = target;
  calc();
}

void CPlotStrip::plot_tick()
{
  source_v += 0.01;
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
  calc();
}

void CPlotStrip::reset()
{
  source_v = 1005.0;
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
}

void CPlotStrip::setup_data()
{
  // data vbo, vao
  const QVector3D data_verts[N_VERTS+1] = {};
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
