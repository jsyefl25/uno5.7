#include "cardback.h"
#include<QDebug>

CardBack::CardBack(QWidget *parent) : QLabel(parent)
{
    QPixmap pix;
    bool ret = pix.load(":/res/back.png");
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }
    this->setPixmap(pix);

}
void CardBack::moveBack(int x, int y)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"geometry");
    animation->setDuration(200);
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animation->setEndValue(QRect(x,y,this->width(),this->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();
}
