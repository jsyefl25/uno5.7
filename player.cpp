#include "player.h"

Player::Player(QObject *parent) : QObject(parent)
{

}
Player::Player(QString name, bool isAI, QVector<CardButton*> hand, QObject *parent) : QObject(parent)
{
    this->name = name;
    this->isAI = isAI;
    this->hand = hand;
}

//查看是否能出牌
bool Player::abledPut(CardButton* card, Color color, int num)
{
    bool abled = false;
    if (card->getColor() == black || color == black) {
        abled = true;
    }
    else if (card->getColor() == color) {
        abled = true;
    }
    else if (card->getSpecificInfo() == QString::number(num)) {
        abled = true;
    }
    return abled;
}

//出牌
CardButton* Player::playCard(Color color, int num)
{
    CardButton* putCards = nullptr;
    //bool abled = false;

    for (QVector<CardButton*>::iterator it = hand.begin(); it != hand.end(); ++it)
    {
        if (abledPut(*it,color,num)) {
            //abled = true;
            putCards = *it;
            hand.erase(it);
            break;
        }
    }
    return putCards;
}
void Player::deleteCard(CardButton* card)
{
    qDebug()<<"删除";
    if(hand.removeOne(card))
    qDebug()<<"删除，当前卡牌数量为"+QString::number(this->getNumberOfCard());

}
//返回一张可以出的牌
CardButton* Player::VaildCard(Color color, int num)
{
    CardButton* putCards = nullptr;
    //bool abled = false;

    for (QVector<CardButton*>::iterator it = hand.begin(); it != hand.end(); ++it)
    {
        if (abledPut(*it,color,num)) {
            //abled = true;
            putCards = *it;
            break;
        }
    }
    return putCards;
}
