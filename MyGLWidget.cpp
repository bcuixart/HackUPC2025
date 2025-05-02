// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

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
    makeCurrent();
    if (event->buttons() == Qt::LeftButton && !(event->modifiers() & (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier))) 
    {
        cam_Rot_Euler_Y += event->pos().x() - mousePosPrevX;
        cam_Rot_Euler_X += event->pos().y() - mousePosPrevY;

        //std::cerr << cam_Rot_Euler_X << ' ' << cam_Rot_Euler_Y << std::endl;

        mousePosPrevX = event->pos().x();
        mousePosPrevY = event->pos().y();

        ini_camera();
        update();
    }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_R: {
      rotationY += .1;
      break;
    }
    case Qt::Key_Z: {
        zoomValue -= 0.01;
        if (zoomValue < 0) zoomValue = 0;
        ini_camera();
        break;
    }
    case Qt::Key_X: {
        zoomValue += 0.01;
        ini_camera();
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

  modelTransformModel ();
  glBindVertexArray (VAO_Model);
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size() * 3);

  modelTransform ();
  glBindVertexArray (VAO_Terra);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray (0);
}

void MyGLWidget::creaBuffers () 
{
    m.load("./Models/Cupra.obj");

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Model);
    glBindVertexArray(VAO_Model);

    GLuint VBO_Model[2];
    glGenBuffers(2, VBO_Model);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);

    // Dades del terra
    // Dos VBOs, un amb posició i l'altre amb color

    glm::vec3 color[6] = {
        glm::vec3(1,0,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1),
        glm::vec3(1,0,0),
        glm::vec3(0,0,1),
        glm::vec3(0,1,0),
    };

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

    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)

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
}

void MyGLWidget::ini_camera() 
{
    modelMinMax();
    calcularCentreRadi(glm::vec3(-2.5, 0, -2.5), glm::vec3(2.5, 4, 2.5));

    VRP = centre;
    OBS = centre + glm::vec3(0,0,2*radi);
    UP = glm::vec3(0,1,0);

    D = glm::distance(VRP, OBS);

    viewTransform(OBS, VRP, UP);

    if (RA > 1) {
        FOV = 2 * asin(radi/D * zoomValue);
    } else {
        FOV = 2 * atan(tan(FOV_ORIGINAL/2)/RA * zoomValue);
    }

    if (FOV_ORIGINAL == -1) FOV_ORIGINAL = FOV;
    
    ZNEAR = D - radi;
    ZFAR = D + radi;

    projectTransform();
}

void MyGLWidget::modelMinMax() {
    modelMin = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    modelMax = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
    for (unsigned int i = 0; i < m.vertices().size(); i+=3) {
        if (m.vertices()[i] < modelMin.x) modelMin.x = m.vertices()[i];
        if (m.vertices()[i+1] < modelMin.y) modelMin.y = m.vertices()[i+1];
        if (m.vertices()[i+2] < modelMin.z) modelMin.z = m.vertices()[i+2];

        if (m.vertices()[i] > modelMax.x) modelMax.x = m.vertices()[i];
        if (m.vertices()[i+1] > modelMax.y) modelMax.y = m.vertices()[i+1];
        if (m.vertices()[i+2] > modelMax.z) modelMax.z = m.vertices()[i+2];
    }
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformModel () 
{
  float sizeY = abs(modelMax.y - modelMin.y);

  float centreX = (modelMax.x + modelMin.x) / 2;
  float centreY = (modelMax.y + modelMin.y) / 2;
  float centreZ = (modelMax.z + modelMin.z) / 2;

  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, rotationY, glm::vec3(0,1,0));
  transform = glm::translate(transform, glm::vec3(-centreX, -centreY + (modelMax.y - modelMin.y) / 1.44, -centreZ));
  transform = glm::scale(transform, glm::vec3(4/sizeY));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj = glm::perspective(FOV, RA, ZNEAR, ZFAR);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform(glm::vec3 OBS, glm::vec3 VRP, glm::vec3 UP) {

    glm::mat4 view (1.0f);
    view = glm::translate(view, glm::vec3(0,0,-glm::distance(VRP, OBS)));
    view = glm::rotate(view, glm::radians(cam_Rot_Euler_Z), glm::vec3(0,0,1));    
    view = glm::rotate(view, glm::radians(cam_Rot_Euler_X), glm::vec3(1,0,0));
    view = glm::rotate(view, glm::radians(cam_Rot_Euler_Y), glm::vec3(0,1,0));
    view = glm::translate(view, -VRP);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}

void MyGLWidget::calcularCentreRadi(glm::vec3 min, glm::vec3 max) {
    centre = glm::vec3( (min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);

    radi = glm::distance(min, max) / 2;
}

void MyGLWidget::resizeGL (int width, int height) 
{
    RA = float(width)/float(height);

    ini_camera();
}

