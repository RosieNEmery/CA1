#ifndef NGLSCENE_H__
#define NGLSCENE_H__
#include <ngl/Camera.h>
#include <ngl/Transformation.h>
#include "particleSystem.h"
#include <QOpenGLWindow>
#include <memory>


class NGLScene : public QOpenGLWindow
{
  public:
    NGLScene();
    ~NGLScene();
    void initializeGL();
    void paintGL();
    void resizeGL(QResizeEvent *_event);
    void resizeGL(int _w, int _h);

private:

    void keyPressEvent(QKeyEvent *_event);
    void timerEvent(QTimerEvent *_event);
    void createPoints();
    void createBillboard();

   // void mousePressEvent ( QMouseEvent *_event);
   // void mouseReleaseEvent ( QMouseEvent *_event );

    void loadTexture(const QString &m_image);

    ngl::Mat4 m_mouseGlobalTX;
    ngl::Camera m_cam;
    ngl::Transformation m_transform;
    ngl::Vec3 m_modelPos;

    GLuint m_vao;
    GLuint billboard_vao;

    GLuint model_buffer;

    GLuint tex;

    int m_width;
    int m_height;

    float Timer;

    std::unique_ptr<ParticleSystem> emitter;

};

#endif
