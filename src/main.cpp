#include <QtGui/QGuiApplication>
#include <iostream>

#include "NGLScene.h"



int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);
  QSurfaceFormat format;

  format.setSamples(4);
  #if defined(__APPLE__)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(2);
  #else
    format.setMajorVersion(4);
    format.setMinorVersion(3);
  #endif

  format.setProfile(QSurfaceFormat::CoreProfile);

  format.setDepthBufferSize(24);

  NGLScene window;

  window.setFormat(format);

  window.resize(1024, 720);
  window.show();

  return app.exec();
}
