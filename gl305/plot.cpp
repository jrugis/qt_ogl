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
}

CPlot::~CPlot()
{
  vao_axis.destroy(); vbo_axis.destroy();
  vao_data.destroy(); vbo_data.destroy();
}

void CPlot::draw()
{
  vbo_data.bind();
  QVector3D temp[] = {
    QVector3D(0.1f, 0.1f, 0.0f),
    QVector3D(0.2f, 0.2f, 0.0f),
    QVector3D(0.2f, 0.3f, 0.0f),
    QVector3D(0.5f, 0.5f, 0.0f)};
  n_data_verts = sizeof(temp) / sizeof(temp[0]);
  vbo_data.write(0, &temp, sizeof(temp));
  vbo_data.release();

  vao_axis.bind();
  //glLineWidth(12.0f);   // not supported on mac air
  shader->setUniformValue(u_color, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
  glDrawArrays(GL_LINE_LOOP, 0, 4);
  vao_axis.release();

  vao_data.bind();
  shader->setUniformValue(u_color, QVector4D(0.0f, 0.0f, 1.0f, 1.0f));
  glPointSize(6.0f);
  glDrawArrays(GL_POINTS, 0, n_data_verts);
  glDrawArrays(GL_LINE_STRIP, 0, n_data_verts);
  vao_data.release();
}
