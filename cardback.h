#ifndef CARDBACK_H
#define CARDBACK_H

#include <QWidget>
#include<QLabel>
#include<QPixmap>
#include<QPropertyAnimation>
class CardBack : public QLabel
{
    Q_OBJECT
public:
    explicit CardBack(QWidget *parent = nullptr);

    void moveBack(int x, int y);

signals:

};

#endif // CARDBACK_H
