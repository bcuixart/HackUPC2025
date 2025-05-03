#include "MyForm.h"
#include <QPixmap>

MyForm::MyForm (QWidget* parent) : QWidget(parent), timer(new QTimer(this))
{
  ui.setupUi(this);
  // Connexions           nou
  //connect(ui.btnNext1, &QPushButton::clicked, this, &MyForm::goToImagePage);  // Botó següent
  connect(timer, &QTimer::timeout, this, &MyForm::goToEndPage);
  timer->setSingleShot(true);
  pagina = 1;
  //timer->start(3000);
  QPixmap pix("./Imagenes/a.jpg");
  ui.imageLabel->setPixmap(pix.scaled(600, 343, Qt::KeepAspectRatio));
  QPixmap pix2("./Imagenes/ini.jpg");
  ui.imageLabel2->setPixmap(pix2.scaled(600, 343, Qt::KeepAspectRatio));
}

void MyForm::goToImagePage() {
    //ui.pages->setCurrentIndex(1);  

    //QPixmap pix("./Imagenes/a.jpg");
    //ui.imageLabel->setPixmap(pix.scaled(600, 343, Qt::KeepAspectRatio));
    pagina = 3;
    timer->setSingleShot(true);
    timer->start(3000);  // Espera 3 segons
}

void MyForm::goToEndPage() {
    ui.pages->setCurrentIndex(pagina); 
}

