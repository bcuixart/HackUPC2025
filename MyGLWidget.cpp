// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {



}

void MyGLWidget::StartWidget() 
{
    cout << "Caca" << endl;

  connect(&timer, SIGNAL(timeout()), this, SLOT(heartbeat()));
  timer.start(10);

  connect(&timerNextQuestion, SIGNAL(timeout()), this, SLOT(QuestionProcedure()));
  timerNextQuestion.setSingleShot(true);
}

void MyGLWidget::QuestionProcedure() 
{
    TestQuestionProcedure(0);
}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

void MyGLWidget::heartbeat() 
{
    makeCurrent();
    time += 0.025;

    if (testActive) {
        posicioCarretera.z = fmod(posicioCarretera.z + velocitatCarretera, 4);
        CameraApproachExpected();
    }

    ini_camera();
    update();
}

void MyGLWidget::mousePressEvent(QMouseEvent* event) 
{
    makeCurrent();
    if (event->buttons() == Qt::LeftButton && !(event->modifiers() & (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier))) 
    {
        mousePosPrevX = event->pos().x();
        mousePosPrevY = event->pos().y();

        ini_camera();
        update();
    }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent* event) 
{
/*
    makeCurrent();
    if (!testActive && event->buttons() == Qt::LeftButton && !(event->modifiers() & (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier))) 
    {
        cam_Rot_Euler_Y += event->pos().x() - mousePosPrevX;

        float nextCamX = cam_Rot_Euler_X + event->pos().y() - mousePosPrevY;
        if (nextCamX > 90 && nextCamX < 180) cam_Rot_Euler_X = nextCamX;

        std::cerr << cam_Rot_Euler_X << ' ' << cam_Rot_Euler_Y << std::endl;

        mousePosPrevX = event->pos().x();
        mousePosPrevY = event->pos().y();

        cam_Rot_Euler_X = fmod(cam_Rot_Euler_X, 360);
        cam_Rot_Euler_Y = fmod(cam_Rot_Euler_Y, 360);
        cam_Rot_Euler_Z = fmod(cam_Rot_Euler_Z, 360);

        ini_camera();
        update();
    }
*/
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Space: {
        //if (!testActive) {
            testActive = true;

            cam_Expected_Rot_Euler_X = 173;
            cam_Expected_Rot_Euler_Y = 346;

            timerNextQuestion.start(1000);

            StartTest();
        //}
      break;
    }
    default: event->ignore(); break;
  }

  update();
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::paintGL () 
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniform1i(teBoira, 1);
  glUniform1i(faOndulacions, 0);
  glUniform1f(ondulacionsTime, time);

  modelTransformCupra ();
  glBindVertexArray (VAO_Cupra);
  glDrawArrays(GL_TRIANGLES, 0, mCupra.faces().size() * 3);

  modelTransformTerra ();
  glBindVertexArray (VAO_Terra);
  glDrawArrays(GL_TRIANGLES, 0, 24);

  modelTransformCarretera ();
  glBindVertexArray (VAO_Carretera);
  glDrawArrays(GL_TRIANGLES, 0, mCarretera.faces().size() * 3);

  glUniform1i(teBoira, 0);
  glUniform1i(faOndulacions, 1);

  glUniform1f(ondulacionsAmplitude, .5);
  glUniform1f(ondulacionsFrequency, 10);
  glUniform1f(ondulacionsWaveSpeed, 1);

  modelTransformBackground (posicioBackgroundBase, escalaBackgroundBase);
  glm::vec3 wC = glm::vec3(0.105, 0.058, 0.5);
  glUniform3fv(ondulacionsWaveColor, 1, &wC[0]);

  glBindVertexArray (VAO_Background);
  glDrawArrays(GL_TRIANGLES, 0, mBackground.faces().size() * 3);

  // 0.105, 0.058, 0.5
  wC = glm::vec3(0.105, 0.058, 0.5);
  glUniform1f(ondulacionsAmplitude, 1);
  glUniform1f(ondulacionsFrequency, 5);
  glUniform1f(ondulacionsWaveSpeed, -1);
  glUniform3fv(ondulacionsWaveColor, 1, &wC[0]);

  modelTransformBackground (posicioBackgroundBase2, escalaBackgroundBase2);
  glDrawArrays(GL_TRIANGLES, 0, mBackground.faces().size() * 3);

  glBindVertexArray (0);
}

void MyGLWidget::creaBuffers () 
{
    mCupra.load("./Models/Cupra.obj");

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Cupra);
    glBindVertexArray(VAO_Cupra);

    GLuint VBO_Cupra[3];
    glGenBuffers(3, VBO_Cupra);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cupra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCupra.faces().size() * 3 * 3, mCupra.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cupra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCupra.faces().size() * 3 * 3, mCupra.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cupra[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCupra.faces().size(), mCupra.VBO_normals(), GL_STATIC_DRAW);

    // Activem l'atribut normalLoc
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);


    mBackground.load("./Models/BackgroundLine.obj");

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Background);
    glBindVertexArray(VAO_Background);

    GLuint VBO_Background[3];
    glGenBuffers(3, VBO_Background);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Background[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mBackground.faces().size() * 3 * 3, mBackground.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Background[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mBackground.faces().size() * 3 * 3, mBackground.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Background[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mBackground.faces().size(), mBackground.VBO_normals(), GL_STATIC_DRAW);

    // Activem l'atribut normalLoc
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);

    mCarretera.load("./Models/Road.obj");

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Carretera);
    glBindVertexArray(VAO_Carretera);

    GLuint VBO_Carretera[3];
    glGenBuffers(3, VBO_Carretera);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Carretera[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCarretera.faces().size() * 3 * 3, mCarretera.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Carretera[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCarretera.faces().size() * 3 * 3, mCarretera.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Carretera[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCarretera.faces().size(), mCarretera.VBO_normals(), GL_STATIC_DRAW);

    // Activem l'atribut normalLoc
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);

    glBindVertexArray (0);

    // Dades del terra
    // Dos VBOs, un amb posició i l'altre amb color

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Terra);
    glBindVertexArray(VAO_Terra);

    GLuint VBO_Terra[2];
    glGenBuffers(2, VBO_Terra);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLocTerra, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLocTerra);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLocTerra, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLocTerra);

    glBindVertexArray (0);
}

void MyGLWidget::initializeGL() {
    initializeOpenGLFunctions(); 

    glClearColor(0.105, 0.058, 0.5, 1.0); // defineix color de fons (d'esborrat)

    glEnable(GL_DEPTH_TEST);

    carregaShaders();
    creaBuffers();
    ini_camera();
}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();

    projLoc = glGetUniformLocation(program->programId(), "PM");
    viewLoc = glGetUniformLocation(program->programId(), "VM");
    vertexLocTerra = glGetAttribLocation (program->programId(), "vertex");
    colorLocTerra = glGetAttribLocation (program->programId(), "color");
    normalLoc = glGetAttribLocation (program->programId(), "normal");
    teBoira = glGetUniformLocation (program->programId(), "usesFog");
    faOndulacions = glGetUniformLocation (program->programId(), "doesWaves");
    ondulacionsTime = glGetUniformLocation (program->programId(), "time");
    ondulacionsAmplitude = glGetUniformLocation (program->programId(), "waveAmplitude");
    ondulacionsFrequency = glGetUniformLocation (program->programId(), "waveFrequency");
    ondulacionsWaveSpeed = glGetUniformLocation (program->programId(), "waveSpeed");
    ondulacionsWaveColor = glGetUniformLocation (program->programId(), "waveColor");
}

void MyGLWidget::ini_camera() 
{
    VRP = centre;
    OBS = centre + glm::vec3(0, 0, cam_Distance);

    D = glm::distance(VRP, OBS);
    radi = D / 2;

    viewTransform(OBS, VRP);

    if (RA > 1) {
        FOV = 2 * asin(radi/D);
    } else {
        FOV = 2 * atan(tan(FOV_ORIGINAL/2)/RA);
    }

    if (FOV_ORIGINAL == -1) FOV_ORIGINAL = FOV;
    
    projectTransform();
}

void MyGLWidget::CameraApproachExpected() 
{
    const float epsilon = 0.1f;

    float diff = fmod((cam_Expected_Rot_Euler_X - cam_Rot_Euler_X + 540.0f), 360.0f) - 180.0f;
    if (abs(diff) <= rotationExpectedSpeed + epsilon) cam_Rot_Euler_X = cam_Expected_Rot_Euler_X;
    else cam_Rot_Euler_X = fmod(cam_Rot_Euler_X + rotationExpectedSpeed * (diff > 0 ? 1.0f : -1.0f) + 360.0f, 360.0f);

    diff = fmod((cam_Expected_Rot_Euler_Y - cam_Rot_Euler_Y + 540.0f), 360.0f) - 180.0f;
    if (abs(diff) <= rotationExpectedSpeed + epsilon) cam_Rot_Euler_Y = cam_Expected_Rot_Euler_Y;
    else cam_Rot_Euler_Y = fmod(cam_Rot_Euler_Y + rotationExpectedSpeed * (diff > 0 ? 1.0f : -1.0f) + 360.0f, 360.0f);

    diff = fmod((cam_Expected_Rot_Euler_Z - cam_Rot_Euler_Z + 540.0f), 360.0f) - 180.0f;
    if (abs(diff) <= rotationExpectedSpeed + epsilon) cam_Rot_Euler_Z = cam_Expected_Rot_Euler_Z;
    else cam_Rot_Euler_Z = fmod(cam_Rot_Euler_Z + rotationExpectedSpeed * (diff > 0 ? 1.0f : -1.0f) + 360.0f, 360.0f);
}

void MyGLWidget::modelTransformTerra () 
{
  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformCupra () 
{
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, rotationYCupra, glm::vec3(0,1,0));
  transform = glm::translate(transform, posicioCupra);
  transform = glm::scale(transform, glm::vec3(escalaCupra));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformBackground(glm::vec3 pos, glm::vec3 scale) 
{
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, pos);
  transform = glm::scale(transform, scale);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformCarretera () 
{
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, posicioCarretera);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}


void MyGLWidget::projectTransform() {
    glm::mat4 Proj = glm::perspective(FOV, RA, ZNEAR, ZFAR);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform(glm::vec3 OBS, glm::vec3 VRP) {

    glm::mat4 view (1.0f);
    view = glm::translate(view, glm::vec3(0,0,-glm::distance(VRP, OBS)));
    view = glm::rotate(view, glm::radians(cam_Rot_Euler_Z), glm::vec3(0,0,1));    
    view = glm::rotate(view, glm::radians(cam_Rot_Euler_X), glm::vec3(1,0,0));
    view = glm::rotate(view, glm::radians(cam_Rot_Euler_Y), glm::vec3(0,1,0));
    view = glm::translate(view, -VRP);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}

void MyGLWidget::resizeGL (int width, int height) 
{
    RA = float(width)/float(height);

    ini_camera();
}
