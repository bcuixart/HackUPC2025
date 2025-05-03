#include "ui_MyForm.h"
#include <QTimer> // nou

class MyForm : public QWidget
{
  Q_OBJECT

  public:
    MyForm (QWidget *parent=0);
    int pagina, punt;

  public slots:
    void pas();
    void puntuacion(const int &t);
  signals:
  void animacion();  

  private slots:        // nou
    void goToEndPage();
  
  private:
    Ui::MyForm ui;
    QTimer *timer; // nou
    QStringList imagenes = {
      "./Imagenes/a.jpg",
      "./Imagenes/b.png",
      "./Imagenes/c.jpg",
    };
};

