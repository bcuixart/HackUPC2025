TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h MyLabel.h Test.h MyRadioButton.h

SOURCES += main.cpp MyForm.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp model.cpp MyLabel.cpp Test.cpp MyRadioButton.cpp
