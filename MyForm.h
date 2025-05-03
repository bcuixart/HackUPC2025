#include "ui_MyForm.h"
#include <QTimer> // nou

class MyForm : public QWidget
{
  Q_OBJECT

  public:
    MyForm (QWidget *parent=0);
    int pagina;

  private slots:        // nou
    void goToImagePage();
    void goToEndPage();
  
  private:
    Ui::MyForm ui;
    QTimer *timer; // nout
};

