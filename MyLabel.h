#include <QLabel>
#include <QTimer>
class MyLabel: public QLabel
{
Q_OBJECT 

public:
MyLabel (QWidget *parent);
bool ans;
QTimer time;
public slots: 
void tractarBut();
void tractarBut2();
void tractarConf();
void tractarPag();
signals:
void pasarpag();
};