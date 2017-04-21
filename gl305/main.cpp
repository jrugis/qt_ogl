#include <QGuiApplication>
#include <QSurfaceFormat>

#define register // register variables deprecated in c++11
#include <pari/pari.h>
#undef register

#include "glwindow.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile); // disallow deprecated ogl
  format.setDepthBufferSize(24);
  format.setSamples(4);
  format.setVersion(3,3);
  QSurfaceFormat::setDefaultFormat(format);

  pari_init(100000,2); // pari initialization

  GLWindow main_window;
  main_window.setTitle("gl305");
  main_window.resize(QSize(600,600));
  main_window.show();

  return app.exec();
}
