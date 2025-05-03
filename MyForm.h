#include "ui_MyForm.h"
#include <QTimer> // nou

class MyForm : public QWidget
{
  Q_OBJECT

  public:
    MyForm (QWidget *parent=0);
    int pagina;

  public slots:
    void pas();

  private slots:        // nou
    void goToStartPage();
    void goToEndPage();
  
  private:
    Ui::MyForm ui;
    QTimer *timer; // nou
};

