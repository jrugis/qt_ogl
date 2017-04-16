#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QTimer>

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
  CScene *m_scene01;
};

#endif // GLWINDOW_H
