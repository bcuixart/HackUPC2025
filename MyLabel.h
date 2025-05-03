#include <QLabel>
class MyLabel: public QLabel
{
Q_OBJECT 

public:
MyLabel (QWidget *parent);
bool ans;
public slots: 
void tractarBut();
void tractarBut2();
void tractarConf();
signals:
};