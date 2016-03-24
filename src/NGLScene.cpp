#include <ngl/NGLInit.h>
#include <ngl/Util.h>

#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>

#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include "particleSystem.h"
#include <iostream>
#include <QString>

const static int nparticles=10000;

NGLScene::NGLScene()
{
  setTitle("Points ");
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL\n";

  //freeing memory
  glDeleteTextures(1,&tex);
  glDeleteVertexArrays(1, &m_vao);
  glDeleteVertexArrays(1, &billboard_vao);
  glDeleteBuffers(1, &model_buffer);
}

void NGLScene::resizeGL(int _w, int _h)
{
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();

  //resetting camera perspective projection
  m_cam.setShape(45.0f,(float)width()/height(),0.05f,350.0f);
}

void NGLScene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();

  //resetting camera perspective projection
  m_cam.setShape(45.0f,(float)width()/height(),0.05f,350.0f);

}

void NGLScene::loadTexture(const QString &m_image)
{

  glGenTextures(1, &tex);

  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  GLuint shaderHandle = shader->getProgramID("TexShader");

  QString file = "textures/";
  QString filepath = file.append(m_image);

  QImage image(filepath);
  if (image.isNull())
    std::cout<<"image coudln't be loaded in"<<std::endl;
  else
  {
    int width = image.width();
    int height = image.height();

    unsigned char *data = new unsigned char [width*height*4];
    unsigned int index = 0;

    QRgb colour;
    for( int y=0; y<height; ++y )
    {
      for( int x=0; x<width; ++x )
      {
        colour = image.pixel(x,y);

        data[index++] = qRed(colour);
        data[index++] = qGreen(colour);
        data[index++] = qBlue(colour);
        data[index++] = qAlpha(colour);
      }
    }


  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  //use for texture atlas?
  //glTexSubImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

  glGenerateMipmap( GL_TEXTURE_2D);

  delete [] data;

  glUniform1i(glGetUniformLocation(shaderHandle, "texSmoke"),0);
  }

}

void NGLScene::initializeGL()
{
  ngl::NGLInit::instance();
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_PROGRAM_POINT_SIZE);

  glViewport(0,0,width(),height());

  float gravity = 0.1;
  ngl::Vec3 wind(0,0,0);

  auto attPtr = std::make_shared<Attractor>(ngl::Vec3(0,0,0), 200);
  emitter.reset(new ParticleSystem(nparticles,ngl::Vec3(0.0f,200.0f,0.0f), wind, gravity, attPtr ));

  //view matrix
  m_cam.set(ngl::Vec3(0,1,1000),
            ngl::Vec3(0,0,0),
            ngl::Vec3(0,1,0));

  m_cam.setShape(45.0f,float(width()/height()),0.1, 200);
  //projection matrix

  //create shader
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  shader->createShaderProgram("Feedback");

  shader->attachShader("Vertex", ngl::ShaderType::VERTEX);
  shader->attachShader("Fragment", ngl::ShaderType::FRAGMENT);

  shader->loadShaderSource("Vertex", "shaders/feedback.glsl");

  shader->compileShader("Vertex");

  shader->attachShaderToProgram("Feedback","Vertex");

  shader->bindAttribute("Feedback", 0, "inPos");

  //need to have second varying of Lifetime so gets passed from transform feedback
  const char *varyings[1] = {"ModelView"};
  GLuint id = shader->getProgramID("Feedback");
  glTransformFeedbackVaryings(id, 1, varyings, GL_INTERLEAVED_ATTRIBS);

  shader->linkProgramObject("Feedback");

  shader->use("Feedback");

  shader->autoRegisterUniforms("Feedback");

  shader->createShaderProgram("TexShader");

  shader->attachShader("TexVert",ngl::ShaderType::VERTEX);
  shader->attachShader("TexFrag",ngl::ShaderType::FRAGMENT);
  shader->loadShaderSource("TexVert","shaders/Vertex.glsl");
  shader->loadShaderSource("TexFrag","shaders/Fragment.glsl");

  shader->compileShader("TexVert");
  shader->compileShader("TexFrag");
  shader->attachShaderToProgram("TexShader","TexVert");
  shader->attachShaderToProgram("TexShader","TexFrag");


  shader->linkProgramObject("TexShader");
  shader->use("TexShader");
  shader->autoRegisterUniforms("TexShader");

  //for Phong Shader
  /*shader->setRegisteredUniform("Material.Ka", 0.1745f,0.01175f,0.01175f);
  shader->setRegisteredUniform("Material.Kd", 0.61424f,0.04136f,0.04136f);
  shader->setRegisteredUniform("Material.Ks", 0.727811f,0.626959f,0.626959f);
  shader->setRegisteredUniform("Material.Shininess", 76.8f);

  shader->setRegisteredUniform("Light.pos", 0.0f,700.0f,0.0f, 1.0f);
  shader->setRegisteredUniform("Light.intensity", 1.0f, 1.0f, 1.0f);

  shader->setRegisteredUniform("Fog.maxDis", 100.0f);
  shader->setRegisteredUniform("Fog.minDis", 50.0f);
  shader->setRegisteredUniform("Fog.colour", 0.5f, 0.5f, 0.5f);

*/



  //texture being loaded in
  QString image = "pastel-smudge.jpg";
  loadTexture(image);

  createBillboard();
  glPointSize(10);
  Timer = 0;
  startTimer(50);

}

void NGLScene::createBillboard()
{

//create data for quad thats instanced
  static const GLfloat billboardVerts[] =
  {
    -0.5f, 0.5f, 0.0f,      //bottom right T1
    0.5f, 0.5, 0.0f,        //top right T1
    0.5f, -0.5f, 0.0f,      //top left T1
    0.5f, -0.5f, 0.0f,      //top left T2
    -0.5f, -0.5f, 0.0f,     //bottom left T2
    -0.5f, 0.5f, 0.0f,      //bottom right T2
  };

  static const GLfloat billboardUVs [] =
  {
    1.0f,0.0f,  //bottom right T1
    1.0f,1.0f,  //top right T1
    0.0f,1.0f,  //top left T1
    0.0f,1.0f,  //top left T2
    0.0f,0.0f,  //bottom left T2
    1.0f,0.0f
   };

  static const GLfloat billboardNormals [] =
  {
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f
  };

  glGenVertexArrays(1, &billboard_vao);
  glBindVertexArray(billboard_vao);

  //vertex array that holds buffers
  GLuint billboardVBO[3];
  glGenBuffers(3, &billboardVBO[0]);

  //verts buffer
  glBindBuffer(GL_ARRAY_BUFFER, billboardVBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(billboardVerts)*sizeof(GLfloat), billboardVerts, GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(0);

  //UVs buffer
  glBindBuffer(GL_ARRAY_BUFFER, billboardVBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(billboardUVs)*sizeof(GLfloat), billboardUVs, GL_STATIC_DRAW);
  glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(1);

  //Normals buffer
  glBindBuffer(GL_ARRAY_BUFFER, billboardVBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(billboardNormals)*sizeof(GLfloat), billboardNormals, GL_STATIC_DRAW);
  glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(2);

  //buffer that holds data for model matrix for each particle
  glGenBuffers(1,&model_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
  glBufferData(GL_ARRAY_BUFFER, nparticles*sizeof(ngl::Mat4), NULL, GL_STATIC_DRAW);
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, model_buffer);

  glEnableVertexAttribArray(3);
  glVertexAttribPointer    (3, 4, GL_FLOAT, GL_FALSE, sizeof(ngl::Mat4), BUFFER_OFFSET(0));
  glVertexAttribDivisor (3, 1);
  glEnableVertexAttribArray(4);
  glVertexAttribPointer    (4, 4, GL_FLOAT, GL_FALSE, sizeof(ngl::Mat4), BUFFER_OFFSET(16));
  glVertexAttribDivisor (4, 1);
  glEnableVertexAttribArray(5);
  glVertexAttribPointer    (5, 4, GL_FLOAT, GL_FALSE, sizeof(ngl::Mat4), BUFFER_OFFSET(32));
  glVertexAttribDivisor (5, 1);
  glEnableVertexAttribArray(6);
  glVertexAttribPointer    (6, 4, GL_FLOAT, GL_FALSE, sizeof(ngl::Mat4), BUFFER_OFFSET(48));
  glVertexAttribDivisor (6, 1);

  //for using Lifetime in same transform feedback varyings?
  //glVertexAttribPointer    (7, 1, GL_FLOAT, GL_FALSE, sizeof(float),BUFFER_OFFSET(52) );
  //glVertexAttribDivisor (7, 1);

}

void NGLScene::paintGL()
{
  Timer++;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);

  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Feedback"]->use();

  std::vector<ngl::Vec4> points = emitter->getVector();

  //draw points
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  GLuint posBuffer;
  glGenBuffers(1, &posBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
  glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(ngl::Vec4), &points[0].m_x, GL_STATIC_DRAW);
  glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(0);


  glBindVertexArray(m_vao);
  shader->setRegisteredUniform("View", m_cam.getViewMatrix());
  glEnable(GL_RASTERIZER_DISCARD);

  glBeginTransformFeedback(GL_POINTS);
  glDrawArrays(GL_POINTS, 0, nparticles);
  glEndTransformFeedback();

  glDisable(GL_RASTERIZER_DISCARD);

  //DRAW INSTANCES

  (*shader)["TexShader"]->use();
  shader->setRegisteredUniformFromMat4("Projection", m_cam.getProjectionMatrix());
  shader->setRegisteredUniform("Timer",Timer);
  shader->setRegisteredUniform("maxLife", 200.0f);
  glBindTexture(GL_TEXTURE_2D, tex);
  glBindVertexArray(billboard_vao);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glDrawArraysInstanced(GL_TRIANGLES,0,6, nparticles);
  glBindVertexArray(0);

  /*ngl::Mat4 MVP=m_cam.getVPMatrix();

  ngl::Mat4 MV = m_cam.getViewMatrix();
  ngl::Mat3 normalMatrix;

  normalMatrix = MV;
  normalMatrix.inverse();

  shader->setRegisteredUniform("MV",MV);
  shader->setRegisteredUniform("MVP",MVP);
  shader->setRegisteredUniform("normalMatrix", normalMatrix);

 //float alpha = 1;
 // shader->setRegisteredUniform("m_alpha", alpha);

   // prim ->draw("plane");
*/
}


void NGLScene::keyPressEvent(QKeyEvent *_event)
{

  switch (_event->key())
  {

    case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;

    case Qt::Key_Left : emitter->setWind(ngl::Vec3(-0.1,0,0)); break;
    case Qt::Key_Right : emitter->setWind(ngl::Vec3(0.1,0,0)); break;

    case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
    case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;

    case Qt::Key_F11 : showFullScreen(); break;


    default : break;
  }

  update();
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  emitter->update();
  update();
}


