#include "MyLabel.h"
#include <iostream>
// constructor
MyLabel::MyLabel(QWidget *parent)
: QLabel(parent) {
//connect(this,SIGNAL(returnPressed()),this,SLOT(tractaReturn()));
// Inicialització d'atributs si cal
ans = false;
}

// implementació slots

void MyLabel::tractarBut() {
    ans = true;
    //std::cout << b << std::endl; 
}
void MyLabel::tractarBut2() {
    ans = false;
    //std::cout << b << std::endl; 
}

void MyLabel::tractarConf() {
    std::cout << ans << std::endl; 
    if (ans) setText("Congratulations!!!");
    else setText("Fail");
}