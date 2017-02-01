#include <QApplication>
#include <QSurfaceFormat>

#include "glwindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile); // disallow deprecated ogl
  format.setDepthBufferSize(24);
  format.setSamples(4);
  format.setVersion(3,3);
  QSurfaceFormat::setDefaultFormat(format);

  GLWindow main_window;
  main_window.setTitle("gl301");
  main_window.resize(QSize(800,600));
  main_window.show();

  return app.exec();
}
