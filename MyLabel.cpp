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
    std::cout << ans << std::endl; 
    if (ans) setText("Congratulations!!!");
    else setText("Fail");
    time.setSingleShot(true);
    time.start(3000);
}