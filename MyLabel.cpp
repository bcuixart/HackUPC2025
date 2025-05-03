#include "MyLabel.h"
#include <iostream>
// constructor
MyLabel::MyLabel(QWidget *parent)
: QLabel(parent) {
    connect(&time, SIGNAL(timeout()), this, SLOT(tractarPag()));
//connect(this,SIGNAL(returnPressed()),this,SLOT(tractaReturn()));
// Inicialització d'atributs si cal
ans = false;
}

// implementació slots

void MyLabel::tractarBut() {
    ans = true;
}
void MyLabel::tractarBut2() {
    ans = false; 
}
void MyLabel::tractarPag() {
    emit pasarpag();
}

void MyLabel::tractarConf() {
    int p = 0;
    if (ans) {
        setText("Congratulations!!!");
        p = 1;
    }
    else {
        setText("Fail");
        p = 0;
    }

    emit punt(p);
    time.setSingleShot(true);
    time.start(2000);
}