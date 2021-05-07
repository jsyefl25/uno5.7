#ifndef CARD_H
#define CARD_H

#include <QMainWindow>
#include <QWidget>
#include<QPushButton>
#include<QMetaEnum>
#include<QString>
#include<QPixmap>
#include<QPropertyAnimation>

enum Color {red,blue,yellow,green,black};
enum Feature{banOne, rotate, plusTwo, changeColor, plusFour}; //卡片功能

class CardButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit Card(QWidget *parent = nullptr);


    CardButton(bool flag = false);
    CardButton(Color, bool falg = false);

    QString Cardname;

    //判断是否被点击 未被点击false
    bool flag ;

    //向上抽牌
    void zoom1();
    //向下放牌动画
    void zoom2();

    //牌移动动画
    void moveCard(int x, int y);




    Color getColor(){return this->color;}

    virtual QString getSpecificInfo(){return "";}

    //重写按下
    void mousePressEvent(QMouseEvent *e);

    //明牌
    virtual void setVaild()=0;
    //暗牌
    virtual void setUnvaild()=0;
    //背面牌
    void setBack();

protected:
    Color color;
    bool isVaild = true;
    bool isBack = false;
    virtual QString setCardName()=0;


signals:


};

class NumberButton : public CardButton
{
    Q_OBJECT
public:
    NumberButton(bool falg = false);
    NumberButton(Color color, int number, bool falg = false);
    int getNumber(){return number;}
    QString getSpecificInfo(){return QString::number(this->number);}
protected:
    QString setCardName();
    //明牌
    void setVaild();
    //暗牌
    void setUnvaild();
    int number;
};

class FeatureButton : public CardButton
{
    Q_OBJECT
public:
    FeatureButton(bool falg = false);
    FeatureButton(Color, Feature, bool falg = false);
    Feature getFeture(){return getFeture();}
    QString getSpecificInfo();
protected:
    QString setCardName();
    Feature feature;
};

#endif // CARD_H
