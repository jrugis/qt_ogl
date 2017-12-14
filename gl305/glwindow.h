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
  GLWindow(char* ipaddr, char* port);
  ~GLWindow();

protected:
  void initializeGL();
  void keyPressEvent(QKeyEvent *e);
  void paintGL();
  void resizeGL(int width, int height);

private slots:
  void animation_tick();
signals:
  void timeout();

private:
  bool animate;
  QTimer *animation_timer;
  QTime elapsed_time;
  QPoint lastPos;
  CScene *m_scene01;
  char* ipaddr;
  char* port;
};

#endif // GLWINDOW_H
