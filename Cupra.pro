TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h MyLabel.h

SOURCES += main.cpp MyForm.cpp Test.cpp MyLabel.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp model.cpp
