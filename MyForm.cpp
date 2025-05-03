#include "MyForm.h"
#include <QPixmap>

MyForm::MyForm (QWidget* parent) : QWidget(parent), timer(new QTimer(this))
{
  ui.setupUi(this);

  // Connexions           nou
  connect(ui.btnNext1, &QPushButton::clicked, this, &MyForm::goToImagePage);  // Botó següent
  connect(timer, &QTimer::timeout, this, &MyForm::goToEndPage);
  timer->setSingleShot(true);
}

void MyForm::goToImagePage() {
    ui.pages->setCurrentIndex(1);  // pagina 2

    QPixmap pix("/root/hackupc25/interaction/animal.jpg");
    ui.imageLabel->setPixmap(pix.scaled(400, 300, Qt::KeepAspectRatio));

    timer->start(3000);  // Espera 3 segons
}

void MyForm::goToEndPage() {
    ui.pages->setCurrentIndex(2); // pagina 3
}

