#include "MyForm.h"
#include <QPixmap>

// ui.pages->setCurrentIndex(1); // pagina en blanco
// ui.pages->setCurrentIndex(3); // pagina END

MyForm::MyForm (QWidget* parent) : QWidget(parent), timer(new QTimer(this))
{
  ui.setupUi(this);
  ui.pages->setCurrentIndex(0);
  // Connexions           nou
  connect(ui.startButton, &QPushButton::clicked, this, &MyForm::goToStartPage);  // Botó següent
  connect(timer, &QTimer::timeout, this, &MyForm::goToEndPage);
  pagina = 0;
  QPixmap pix("./Imagenes/ini.png");
  ui.imageLabel2->setPixmap(pix.scaled(600, 343, Qt::KeepAspectRatio));
}

void MyForm::goToStartPage() {
    ui.pages->setCurrentIndex(2); // pagina pregunta 1  

    QPixmap pix("./Imagenes/a.jpg");
    ui.imageLabel->setPixmap(pix.scaled(600, 343, Qt::KeepAspectRatio));
    pagina = 2;
    //timer->setSingleShot(true);
    //timer->start(3000);  // Espera 3 segons
}

void MyForm::goToEndPage() {
    ui.pages->setCurrentIndex(pagina); 
}

void MyForm::pas() {
  pagina = 3;
  ui.pages->setCurrentIndex(pagina); 
}
