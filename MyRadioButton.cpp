#include "MyRadioButton.h"
// constructor
MyRadioButton::MyRadioButton(QWidget *parent)
: QRadioButton(parent) {
//connect(this,SIGNAL(setNum(const int &)),this,SLOT(tractarInt(const int &)));
// Inicialització d'atributs si cal
}
// implementació slots

void MyRadioButton::tractarBut1() {
    setStyleSheet("background-color: green;");
}