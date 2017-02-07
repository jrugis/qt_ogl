#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QTime>

#include "scene.h"

class QExposeEvent;

class GLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
  Q_OBJECT // required for signals and slots

public:
  GLWindow();
  ~GLWindow();

protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

protected slots:
  void animation_tick();
signals:
  void timeout();

private:
  QTimer *animation_timer;
  QTime m_elapsed_time;
  Scene *m_scene01;
};

#endif // GLWINDOW_H
