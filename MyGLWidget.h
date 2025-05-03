// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

#define TERRA_SIZE 10
#define TERRA_COLOR_CLAR glm::vec3(1, 1, 1)
#define TERRA_COLOR_FOSC glm::vec3(1, 1, 1) //0.105, 0.058, 0.5

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  private:
    int printOglError(const char file[], int line, const char func[]);

    void carregaShaders();
    virtual void creaBuffers ();
    virtual void initializeGL();
    virtual void paintGL();

    virtual void resizeGL (int width, int height);

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);

    void modelTransformTerra();
    void modelTransformCupra();

    void projectTransform();
    void viewTransform(glm::vec3 OBS, glm::vec3 VRP);

    void ini_camera();

    Model mCupra;
    
    GLuint projLoc;
    GLuint viewLoc;
    GLuint VAO_Model;
    GLuint VAO_Terra;
    GLuint normalLoc;
    GLuint vertexLocTerra;
    GLuint colorLocTerra;
    GLuint normalLocTerra;

    //Terra
    glm::vec3 posicio[24] = {
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, TERRA_SIZE),
        glm::vec3(-TERRA_SIZE, 0, TERRA_SIZE),

        glm::vec3(0, 0, 0),
        glm::vec3(-TERRA_SIZE, 0, 0),
        glm::vec3(-TERRA_SIZE, 0, TERRA_SIZE),

        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, TERRA_SIZE),
        glm::vec3(TERRA_SIZE, 0, TERRA_SIZE),

        glm::vec3(0, 0, 0),
        glm::vec3(TERRA_SIZE, 0, 0),
        glm::vec3(TERRA_SIZE, 0, TERRA_SIZE),

        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, -TERRA_SIZE),
        glm::vec3(TERRA_SIZE, 0, -TERRA_SIZE),

        glm::vec3(0, 0, 0),
        glm::vec3(TERRA_SIZE, 0, 0),
        glm::vec3(TERRA_SIZE, 0, -TERRA_SIZE),

        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, -TERRA_SIZE),
        glm::vec3(-TERRA_SIZE, 0, -TERRA_SIZE),

        glm::vec3(0, 0, 0),
        glm::vec3(-TERRA_SIZE, 0, 0),
        glm::vec3(-TERRA_SIZE, 0, -TERRA_SIZE)
    }; 

    glm::vec3 color[24] = {
        TERRA_COLOR_CLAR,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_CLAR,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_CLAR,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_CLAR,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_CLAR,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_CLAR,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_CLAR,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_CLAR,
        TERRA_COLOR_FOSC,
        TERRA_COLOR_FOSC,
    };

    // Camera
    glm::vec3 centre = glm::vec3(0,0.5,0);
    float cam_Distance = 12;
    
    glm::vec3 VRP;
    glm::vec3 OBS;

    float RA = 1.f;
    float FOV_ORIGINAL = -1;
    float FOV;
    float ZNEAR;
    float ZFAR = 100;
    float D = 0;
    float radi;

    int mousePosPrevX = 0;
    int mousePosPrevY = 0;

    float cam_Rot_Euler_X = 0;
    float cam_Rot_Euler_Y = 0;
    float cam_Rot_Euler_Z = 0;

    glm::vec3 posicioCupra = glm::vec3 (0, 0, 0);
    float rotationYCupra = 0;
    float escalaCupra = 1;
};
