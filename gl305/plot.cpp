#include "constants.h"
#include "plot.h"

CPlot::CPlot(QOpenGLShaderProgram *s)
{
  shader = s;
  u_color = shader->uniformLocation("color");

  // axis buffer
  const QVector3D axis_verts[] = {
    QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f),
    QVector3D(1.0f, 1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)};
  vbo_axis.create();
  vbo_axis.bind();
  vbo_axis.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_axis.allocate(axis_verts, sizeof(axis_verts));
  vao_axis.create();
  vao_axis.bind();
  shader->enableAttributeArray(0);
  shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  vao_axis.release();
  vbo_axis.release();

  // data buffer
  const QVector3D data_verts[MAX_DATA_VERTS] = {};
  vbo_data.create();
  vbo_data.bind();
  vbo_data.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  vbo_data.allocate(data_verts, sizeof(data_verts));
  vao_data.create();
  vao_data.bind();
  shader->enableAttributeArray(0);
  shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  vao_data.release();
  vbo_data.release();

  // source data
  source = QVector3D(0.5f, 0.5f, 0.0f);
  n_data_verts = 1;

  reset();
}

CPlot::~CPlot()
{
  vao_axis.destroy(); vbo_axis.destroy();
  vao_data.destroy(); vbo_data.destroy();
}

void CPlot::calc()
{
  // transform the source
  QVector3D s, d;
  QMatrix4x4 transform;
  transform.setToIdentity();
  transform.translate(source_translate);
  s = transform * source;

  // source->destination mapping
  d = 1.1f * s;

  // push to vbo
  vbo_data.bind();
  vbo_data.write(0, &s, sizeof(s));         // source
  vbo_data.write(sizeof(s), &d, sizeof(d)); // destination
  vbo_data.release();
}

void CPlot::draw()
{
  //glLineWidth(12.0f);   // not supported on mac air
  //glEnable(GL_LINE_SMOOTH);
  //glDisable(GL_LINE_SMOOTH);

  // draw axis
  vao_axis.bind();
  shader->setUniformValue(u_color, QVector4D(LIGHT_GRAY, 1.0f));
  glDrawArrays(GL_LINE_LOOP, 0, 4);
  vao_axis.release();

  // draw data
  vao_data.bind();
  // source
  shader->setUniformValue(u_color, QVector4D(BLUE, 1.0f));
  glPointSize(6.0f);
  glDrawArrays(GL_POINTS, 0, n_data_verts);
  glDrawArrays(GL_LINE_STRIP, 0, n_data_verts);
  // destination
  shader->setUniformValue(u_color, QVector4D(GREEN, 1.0f));
  glPointSize(6.0f);
  glDrawArrays(GL_POINTS, n_data_verts, n_data_verts);
  glDrawArrays(GL_LINE_STRIP, n_data_verts, n_data_verts);
  vao_data.release();
}

void CPlot::move_source(int x, int y)
{
  float scale = 0.01f;
  source_translate += QVector3D(x * scale, y * scale, 0.0f);
  calc();
}

void CPlot::reset()
{
  source_translate = QVector3D(0.0f, 0.0f, 0.0f);
  calc();
}
