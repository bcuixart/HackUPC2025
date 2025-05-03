#include <QRadioButton>
class MyRadioButton: public QRadioButton
{
Q_OBJECT 

public:
MyRadioButton (QWidget *parent);
int ct;
public slots: 
void tractarBut1();
};