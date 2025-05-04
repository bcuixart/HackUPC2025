#include "MyForm.h"
#include <QPixmap>
#include <iostream>

MyForm::MyForm (QWidget* parent) : QWidget(parent), timer(new QTimer(this))
{
  ui.setupUi(this);
  ui.pages->setCurrentIndex(0);
  connect(timer, &QTimer::timeout, this, &MyForm::goToEndPage);
  pagina = 0;
  QPixmap pix("./Imagenes/ini.png");
  ui.imageLabel2->setPixmap(pix.scaled(600, 343, Qt::KeepAspectRatio));
  QPixmap pix2("./Imagenes/HackUPC.png");
  ui.ImageLabelUPC->setPixmap(pix2.scaled(600 / 2, 343 / 2, Qt::KeepAspectRatio));
  punt = 0;
}

void MyForm::goToEndPage() {
    ui.pages->setCurrentIndex(pagina); 
}

void MyForm::pas() {
  ++pagina;
  ui.pages->setCurrentIndex(pagina); 
  if (pagina%2 == 0) {
    if (pagina == 2) {      // pregunta 1
      QPixmap pix(imagenes[0]);
      ui.imageLabel->setPixmap(pix.scaled(600, 200, Qt::KeepAspectRatio));
    }
    else if (pagina == 4) {  // pregunta 2
      QPixmap pix(imagenes[1]);
      ui.imageLabel_2->setPixmap(pix.scaled(600, 200, Qt::KeepAspectRatio));
    }
    else if (pagina == 6) {
      QPixmap pix(imagenes[2]);
      ui.imageLabel_3->setPixmap(pix.scaled(600, 200, Qt::KeepAspectRatio));
    }
  }
  else if (pagina == 7) {  // fin
    ui.label_fin->setNum(punt);
  }
  emit animacion();
}

void MyForm::puntuacion(const int &t) {
  punt += t;
}