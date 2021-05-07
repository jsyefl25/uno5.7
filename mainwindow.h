#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<cardbutton.h>
#include<player.h>
#include<QLabel>
#include<QQueue>
#include<QPainter>
#include<QMenuBar>
#include<QMenu>
#include<QAction>
#include<QTimer>
#include<QMap>
#include<cardback.h>
 #include<QTime>
#include<QCoreApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //msec为毫秒
    //延迟函数
    void sleep(unsigned int msec)
    {
        QTime reachTime = QTime::currentTime().addMSecs(msec);
        while (QTime::currentTime() < reachTime)
         {
             QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
         }
    }


    //重写绘图事件
    void paintEvent(QPaintEvent *event);

    void Start();

private:
        // 初始化游戏卡片
        // 数字牌（76张）、功能牌（24张）、万能牌（8张），合计108张
        void initCards();

        //初始化玩家手牌，一开始每人发numOfCards张牌
        //返回生成的卡牌向量
        QVector<CardButton*> initHand(unsigned int numOfCards);


        //将卡片的顺序打乱
        void randomCards();

        QVector<Player*> players;  //参加游戏的玩家

        QQueue<CardButton*> cards;  //用于抽卡的主卡片组

        QVector<CardButton*> cardCache;  //用于缓存将要进入主卡片组的副卡片组

        //AI玩家的手牌信息
        QMap <int,QLabel*> AImessage;

        QMap <int, QVector<CardBack*>> PlayerBackCard;
        //初始化显示背面牌 玩家坐标
        void InitBackCard(int);
        //打出背面牌 玩家坐标 当前牌的数量 动画
        void PlayBackCard(int,int);
        //抽牌 背面牌 动画
        void DrawBackCard(int,int);

        //初始化显示玩家姓名和剩余牌数等信息
        void showMessage(int , QString);

        QLabel *State;

        //记录状态信息
        int numberState;
        Color colorState;
        //当前牌
        CardButton* cardState = NULL;
        //
        void changeState(CardButton*);

        QTimer *timer;

        //玩家坐标
        int cur = 0;

        //按钮
        QPushButton *btn;

        //得到牌的最右位置  指操作者
        int getMaxRight()
        {
            QVector<CardButton*> MyCards = players[0]->getHand();
            int left = MyCards[0]->x();
            for(int i=1;i<MyCards.size();i++)
            {
                if(MyCards[i]->x()>left)
                {
                    left = MyCards[i]->x();
                }
            }
            return left;

        }
        //出牌动画
        void AnimapalyCard(CardButton*);

        //计时器 自动出牌
        QTimer *autoPlayCardTime;


        //游戏
        void GameLoop();

        //AI
        void AiTurn();

        //player
        void PlayerTurn();



        //UI
        Ui::MainWindow *ui;


signals:
    void closeButton();
    void openButton();
    void victory();
};
#endif // MAINWINDOW_H
