#include <QGuiApplication>
#include <QSurfaceFormat>

#include "glwindow.h"

int main(int argc, char *argv[])
{
  if(argc != 3){
    qDebug() << "Usage: ./gl305 <ip address> <port>";
    exit(1);
  }
  QGuiApplication app(argc, argv);

  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile); // disallow deprecated ogl
  format.setDepthBufferSize(24);
  format.setSamples(4);
  format.setVersion(3,3);
  QSurfaceFormat::setDefaultFormat(format);

  GLWindow main_window(argv[1], argv[2]);
  main_window.setTitle("gl305");
  main_window.resize(QSize(600,600));
  main_window.show();

  return app.exec();
}
