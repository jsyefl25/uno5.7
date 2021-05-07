#include "cardbutton.h"
#include<QDebug>
//Card::Card(QWidget *parent) : QWidget(parent)

CardButton::CardButton(bool flag):flag(flag)
{}

CardButton::CardButton(Color col, bool flag)
{
    this->color = col; 
    this->flag = flag;
}


void CardButton::zoom1()
{
    flag=true;

    //创建动画特效
     QPropertyAnimation *animation = new QPropertyAnimation(this,"geometry");
     //设置动画事件间隔
     animation->setDuration(200);
     //起始位置
     animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
     //结束位置
     animation->setEndValue(QRect(this->x(),this->y()-30,this->width(),this->height()));
     //设置弹跳曲线
     animation->setEasingCurve(QEasingCurve::OutBounce);

     //
     animation->start();
}
void CardButton::zoom2()
{
    flag=false;

     QPropertyAnimation *animation = new QPropertyAnimation(this,"geometry");
     animation->setDuration(200);
     animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
     animation->setEndValue(QRect(this->x(),this->y()+30,this->width(),this->height()));
     animation->setEasingCurve(QEasingCurve::OutBounce);

     animation->start();
}
void CardButton::moveCard(int x, int y)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"geometry");
    animation->setDuration(200);
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animation->setEndValue(QRect(x,y,this->width(),this->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start();
}

void CardButton::mousePressEvent(QMouseEvent *e)
{
    if( !this->isVaild )
    {
        return;
    }else
    {
        QPushButton::mousePressEvent(e);
    }
}

void CardButton::setBack()
{
    this->isBack = true;

    QPixmap pix;
    bool ret = pix.load(":/res/back.png");
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

}
NumberButton::NumberButton(bool flag){this->flag = flag;}
NumberButton::NumberButton(Color color, int number,bool flag): CardButton(color,flag)
{
    this->number = number;
    this->Cardname = this->setCardName();

    QPixmap pix;
    bool ret = pix.load(":/res/card/front_"+this->Cardname+".png");
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
}

QString NumberButton::setCardName()
{
    QString str;
    switch (this->getColor()) {
    case red:
        str = QString("r%1").arg(this->number);
        break;
    case blue:
        str = QString("b%1").arg(this->number);
        break;
    case yellow:
        str = QString("y%1").arg(this->number);
        break;
    case green:
        str = QString("g%1").arg(this->number);
        break;
    case black:
        str = QString("kw%1").arg(this->number);
        break;
    default:
        break;
    }
    return str;
}
//明牌
void NumberButton::setVaild()
{
    this->isVaild = true;
    this->Cardname = this->setCardName();

    QPixmap pix;
    bool ret = pix.load(":/res/card/front_"+this->Cardname+".png");
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
}
//暗牌
void NumberButton::setUnvaild()
{
    this->isVaild = false;
    this->Cardname = this->setCardName();

    QPixmap pix;
    bool ret = pix.load(":/res/card_dark/dark_"+this->Cardname+".png");
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
}


FeatureButton::FeatureButton(bool flag){this->flag = flag;}
FeatureButton::FeatureButton(Color color, Feature feature, bool flag):CardButton(color,flag)
{
    this->feature = feature;
    this->Cardname = this->setCardName();

    QPixmap pix;
    bool ret = pix.load(":/res/card/front_"+this->Cardname+".png");
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
}

QString FeatureButton::setCardName()
{
    QString cardName;
    switch (color) {
        case red: cardName = "r"; break;
        case yellow: cardName = "y"; break;
        case green: cardName = "g"; break;
        case blue: cardName = "b"; break;
        }
    switch (feature) {
        case banOne: cardName += "BanOne"; break;
        case rotate: cardName += "Rotate"; break;
        case changeColor: cardName += "ChangeColor"; break;
        case plusTwo: cardName += "PlusTwo"; break;
        case plusFour: cardName = "BlackPlusFour"; break;
        default:
            cardName = "errorCard"; break;
        }
        return cardName;
    return cardName;
}

QString FeatureButton::getSpecificInfo()
{
    QString info;
        switch (feature) {
        case banOne: info += "BanOne"; break;
        case rotate: info += "Rotate"; break;
        case changeColor: info += "ChangeColor"; break;
        case plusTwo: info += "PlusTwo"; break;
        case plusFour: info = "BlackPlusFour"; break;
        default:
            info = "errorCard"; break;
        }
        return info;
}


//void FeatureButton::setVaild(){}
//void FeatureButton::setUnvaild(){}







