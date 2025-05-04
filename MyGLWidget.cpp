// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {
  connect(&timer, SIGNAL(timeout()), this, SLOT(heartbeat()));
  timer.start(10);

  connect(&legowalk2Timer, SIGNAL(timeout()), this, SLOT(LegoManWalk2()));
  legowalk2Timer.setSingleShot(true);

  connect(&legowalk3Timer, SIGNAL(timeout()), this, SLOT(LegoManWalk3()));
  legowalk3Timer.setSingleShot(true);  
  
  connect(&carMoveTimer, SIGNAL(timeout()), this, SLOT(CarMoveTimer()));
  carMoveTimer.setSingleShot(true);
}

void MyGLWidget::StartWidget() 
{
  switch (signalsRebuts) {

    case 0:
        posicioLegoman = glm::vec3(10,0,0);
        escalaLegoman = 1;

        testActive = true;
        legomanMoving = true;

        cam_Expected_Rot_Euler_X = -173 + 180;
        cam_Expected_Rot_Euler_Y = 256;

        break;

    case 1:
        legowalk2Timer.start(2000);
        break;

    case 2:
        legowalk3Timer.start(2000);
        break;
    case 3:
        carMoveTimer.start(2000);

  }

  ++signalsRebuts;

  //timerLegoManStart.start(2000);
}


void MyGLWidget::LegoManWalk2() 
{
    rotacioYLegoman = 180;

    cam_Expected_Rot_Euler_X = -173 + 180;
    cam_Expected_Rot_Euler_Y = 346 + 180;

    legomanMoving2 = true;
}

void MyGLWidget::LegoManWalk3() 
{
    rotacioYLegoman = 0;

    cam_Expected_Rot_Euler_X = 11;
    cam_Expected_Rot_Euler_Y = 329;

    legomanMoving3 = true;
}

void MyGLWidget::CarMoveTimer() 
{
    cam_Expected_Rot_Euler_X = 14;
    cam_Expected_Rot_Euler_Y = 145;

    roadMoving = true;
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
        CameraApproachExpected();
    }

    if (roadMoving) {
        posicioCarretera.z = fmod(posicioCarretera.z + velocitatCarretera, 4);
    }

    if (legomanMoving) {
        posicioLegoman.x -= legomanSpeed;

        /*
        if (abs(posicioLegoman.x) < 2) {
            posicioLegoman.y += legomanSpeed * 2;
            escalaLegoman -= 0.03;
        }*/

        if (abs(posicioLegoman.x) < 2.5) {
            legomanMoving = false;
            emit Siguiente();
        }
    }

    if (legomanMoving2) {
        posicioLegoman.z -= legomanSpeed;

        if (abs(posicioLegoman.z) > 4) {
            legomanMoving2 = false;
            rotacioYLegoman = -90;
            emit Siguiente();
        }
    }

    if (legomanMoving3) {
        posicioLegoman.z += legomanSpeed;

        if (posicioLegoman.z > .5) {
            legomanMoving3 = false;
            legomanMoving4 = true;
            rotacioYLegoman = -90;
        }
    }

    if (legomanMoving4) {
        posicioLegoman.x -= legomanSpeed;

        posicioLegoman.y += legomanSpeed * 2;
        escalaLegoman -= 0.03;

        if (abs(posicioLegoman.x) < 1.1) {
            legomanMoving4 = false;
            emit Siguiente();
            posicioLegoman = glm::vec3(100,100,100);
        }
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
    makeCurrent();
    if (!testActive && event->buttons() == Qt::LeftButton && !(event->modifiers() & (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier))) 
    {
        cam_Rot_Euler_Y += event->pos().x() - mousePosPrevX;

        float nextCamX = cam_Rot_Euler_X + event->pos().y() - mousePosPrevY;
        if (nextCamX > 0 && nextCamX < 90) cam_Rot_Euler_X = nextCamX;

        //std::cerr << cam_Rot_Euler_X << ' ' << cam_Rot_Euler_Y << std::endl;

        mousePosPrevX = event->pos().x();
        mousePosPrevY = event->pos().y();

        cam_Rot_Euler_X = fmod(cam_Rot_Euler_X, 360);
        cam_Rot_Euler_Y = fmod(cam_Rot_Euler_Y, 360);
        cam_Rot_Euler_Z = fmod(cam_Rot_Euler_Z, 360);

        ini_camera();
        update();
    }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Space: {
        //if (!testActive) {

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
  glUniform1i(getsLighting, 1);

  modelTransformCupra ();
  glBindVertexArray (VAO_Cupra);
  glUniform3fv(lightposLoc, 1, &focus.pos[0]);
  glUniform3fv(lightcolLoc, 1, &focus.col[0]);
  glUniform3fv(amblightLoc, 1, &amb_l_col[0]);
  glUniform3fv(obsLoc, 1, &OBS[0]);
  glDrawArrays(GL_TRIANGLES, 0, mCupra.faces().size() * 3);


  modelTransformLegoman ();
  glUniform1i(teBoira, 1);
  glBindVertexArray (VAO_Legoman);
  glUniform3fv(lightposLoc, 1, &focus.pos[0]);
  glUniform3fv(lightcolLoc, 1, &focus.col[0]);
  glUniform3fv(amblightLoc, 1, &amb_l_col[0]);
  glUniform3fv(obsLoc, 1, &OBS[0]);
  glDrawArrays(GL_TRIANGLES, 0, mLegoman.faces().size() * 3);

  glUniform1i(getsLighting, 0);

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

struct Vec3Hash {
    std::size_t operator()(const glm::vec3& v) const {
        std::hash<float> hasher;
        size_t h1 = hasher(v.x);
        size_t h2 = hasher(v.y);
        size_t h3 = hasher(v.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

struct Vec3Equal {
    bool operator()(const glm::vec3& a, const glm::vec3& b) const {
        return glm::distance(a,b) < 1e-6f;
    }
};

float* compute_vertex_normals(float* vertices, float* normals, int size) {
    std::unordered_map<glm::vec3, glm::vec3, Vec3Hash, Vec3Equal> vertex_normal;
    std::unordered_map<glm::vec3, int, Vec3Hash, Vec3Equal> vertex_count;
    
    float* result = (float*) malloc(size * sizeof(float));

    for (int i = 0; i < size; i += 3) {
        glm::vec3 v(vertices[i], vertices[i+1], vertices[i+2]);
        glm::vec3 n(normals[i], normals[i+1], normals[i+2]);
        vertex_normal[v] += n;
        vertex_count[v] += 1;
    }

    for (int i = 0; i < size; i += 3) {
        glm::vec3 v(vertices[i], vertices[i+1], vertices[i+2]);
        glm::vec3 n = glm::normalize(vertex_normal[v] / (float)vertex_count[v]);
        result[i+0] = n.x;
        result[i+1] = n.y;
        result[i+2] = n.z;
    }

    return result;
}

void MyGLWidget::creaBuffersCupra() {
    
    mCupra.load("./Models/Cupra.obj");

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Cupra);
    glBindVertexArray(VAO_Cupra);

    GLuint VBO_Cupra[6];
    glGenBuffers(6, VBO_Cupra);

    // Vertices:
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cupra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCupra.faces().size() * 3 * 3, mCupra.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Normals:
    float* normals = compute_vertex_normals(mCupra.VBO_vertices(), mCupra.VBO_normals(), mCupra.faces().size()*3*3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cupra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCupra.faces().size()* 3 * 3, normals, GL_STATIC_DRAW);
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);
    
    // MatAmb:
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cupra[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCupra.faces().size() * 3 * 3, mCupra.VBO_matamb(), GL_STATIC_DRAW);
    glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matambLoc);
    
    // MatDiff:
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cupra[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCupra.faces().size() * 3 * 3, mCupra.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    // MatSpec:
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cupra[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCupra.faces().size() * 3 * 3, mCupra.VBO_matspec(), GL_STATIC_DRAW);
    glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matspecLoc);

    // MatShin:
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cupra[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mCupra.faces().size() * 3, mCupra.VBO_matshin(), GL_STATIC_DRAW);
    glVertexAttribPointer(matshinLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matshinLoc);
}

void MyGLWidget::creaBuffersLegoman() {
    mLegoman.load("./Models/legoman.obj");

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Legoman);
    glBindVertexArray(VAO_Legoman);

    GLuint VBO_Legoman[6];
    glGenBuffers(6, VBO_Legoman);

    // Vertices:
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Legoman[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mLegoman.faces().size() * 3 * 3, mLegoman.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Normals:
    float* normals = compute_vertex_normals(mLegoman.VBO_vertices(), mLegoman.VBO_normals(), mLegoman.faces().size()*3*3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Legoman[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mLegoman.faces().size()* 3 * 3, normals, GL_STATIC_DRAW);
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);
    
    // MatAmb:
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Legoman[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mLegoman.faces().size() * 3 * 3, mLegoman.VBO_matamb(), GL_STATIC_DRAW);
    glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matambLoc);
    
    // MatDiff:
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Legoman[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mLegoman.faces().size() * 3 * 3, mLegoman.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matdiffLoc);

    // MatSpec:
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Legoman[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mLegoman.faces().size() * 3 * 3, mLegoman.VBO_matspec(), GL_STATIC_DRAW);
    glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(matspecLoc);

    // MatShin:
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Legoman[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mLegoman.faces().size() * 3, mLegoman.VBO_matshin(), GL_STATIC_DRAW);
    glVertexAttribPointer(matshinLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

void MyGLWidget::creaBuffers () 
{
    creaBuffersCupra();
    creaBuffersLegoman();
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

    focus.pos = glm::vec3(0.0, 10.0, 0.0);
    focus.col = glm::vec3(1.0, 1.0, 1.0);
    amb_l_col = glm::vec3(0.05, 0.05, 0.05);

    carregaShaders();
    creaBuffers();
    ini_camera();
}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();

    projLoc = glGetUniformLocation(program->programId(), "PM");
    viewLoc = glGetUniformLocation(program->programId(), "VM");
    
    vertexLocTerra = glGetAttribLocation (program->programId(), "vertex");
    colorLocTerra = glGetAttribLocation (program->programId(), "matdiff");
    
    normalLoc = glGetAttribLocation (program->programId(), "normal");
    matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
    matambLoc = glGetAttribLocation (program->programId(), "matamb");
    matspecLoc = glGetAttribLocation (program->programId(), "matspec");
    matshinLoc = glGetAttribLocation (program->programId(), "matshin");

    getsLighting = glGetUniformLocation (program->programId(), "getsLighting");
    lightposLoc = glGetUniformLocation (program->programId(), "l_pos");
    lightcolLoc = glGetUniformLocation (program->programId(), "l_col");
    amblightLoc = glGetUniformLocation (program->programId(), "amb_l_col");
    obsLoc = glGetUniformLocation (program->programId(), "obs");

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

void MyGLWidget::modelTransformLegoman() 
{
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, posicioLegoman);
  transform = glm::rotate(transform, glm::radians(rotacioYLegoman), glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(escalaLegoman));
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