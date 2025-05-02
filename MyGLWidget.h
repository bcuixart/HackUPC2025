// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

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

    virtual void modelTransform();
    void modelTransformModel();

    void projectTransform();
    void viewTransform(glm::vec3 OBS, glm::vec3 VRP, glm::vec3 UP);

    void modelMinMax();
    void calcularCentreRadi(glm::vec3 min, glm::vec3 max);

    void ini_camera();

    Model m;
    
    GLuint projLoc;
    GLuint viewLoc;
    GLuint VAO_Model;
    GLuint VAO_Terra;
    GLuint vertexLocTerra;
    GLuint colorLocTerra;

    //Terra
    glm::vec3 posicio[6] = {
        glm::vec3(2.5, 0, 2.5),
        glm::vec3(-2.5, 0, -2.5),
        glm::vec3(-2.5, 0, 2.5),

        glm::vec3(2.5, 0, 2.5),
        glm::vec3(2.5, 0, -2.5),
        glm::vec3(-2.5, 0, -2.5)
    }; 

    glm::vec3 centre;
    float radi;
    
    glm::vec3 modelMin;
    glm::vec3 modelMax;

    float RA = 1.f;

    glm::vec3 VRP;
    glm::vec3 OBS;
    glm::vec3 UP;

    float FOV_ORIGINAL = -1;
    float FOV;
    float ZNEAR;
    float ZFAR;
    float D = 0;

    float zoomValue = 1;

    int mousePosPrevX = 0;
    int mousePosPrevY = 0;

    float cam_Rot_Euler_X = 0;
    float cam_Rot_Euler_Y = 0;
    float cam_Rot_Euler_Z = 0;

    float rotationY = 0;
};
