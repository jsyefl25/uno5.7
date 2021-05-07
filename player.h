#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include<QString>
#include<cardbutton.h>
#include<QDebug>
#include<QVector>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    Player(QString name, bool isAI, QVector<CardButton*> hand, QObject *parent = nullptr );

    //显示玩家的名称
    QString getPlayerName() { return name; }
    //显示AI
    bool getIsAI(){return this->isAI;}

    QVector<CardButton*>& getHand(){return this->hand;}

    //显示手牌数量
    size_t getNumberOfCard() { return hand.size(); }

    //抽牌
    CardButton* DrawCard(CardButton* card) {
            hand.push_back(card);
            return card;
        }

    //出牌
    CardButton* playCard(Color color, int num);

    //返回一张可以出的牌
    CardButton* VaildCard(Color color, int num);

    //查看是否能出牌
    bool abledPut(CardButton* card, Color color, int num);

    //删除指定牌
    void deleteCard(CardButton* card);

    void showCard(){
        QString str;
        for(int i=0;i<hand.size();i++)
        {
           str+=hand[i]->Cardname;
           str+=" ";

        }qDebug()<<str;
    }


private:
    QString name;   //玩家名称
    bool isAI;  //是否是AI玩家
    QVector<CardButton*> hand;  //玩家持有的手牌

signals:

};

#endif // PLAYER_H
