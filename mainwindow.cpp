#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //大小、图标、标题
    //原来为1200 , 800
    setFixedSize(1400,900);
    setWindowIcon(QIcon(":/res/back.png"));
    setWindowTitle("UNO");


    //菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);
    QMenu *startMenu = bar->addMenu("开始");
    QAction *quitAction = startMenu->addAction("退出");
    //点击退出实现退出游戏
    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });

    //胜利 停止定时器
    connect(this,&MainWindow::victory,[=](){
        timer->stop();
    });

    /**
      主要功能部分
      */

    this->Start();

    btn = new QPushButton;
    btn->setParent(this);
    btn->setText("出牌");
    btn->setFixedSize(80,50);
    btn->move(1000,700);

    connect(this, &MainWindow::closeButton,[=](){
        for(int i =0; i<players[0]->getHand().size();i++)
        {
            //players[0]->getHand()[i]->setEnabled(false);
            players[0]->getHand()[i]->setUnvaild();
            btn->setEnabled(false);
        }
    });
    connect(this, &MainWindow::openButton,[=](){
        for(int i =0; i<players[0]->getHand().size();i++)
        {
            //players[0]->getHand()[i]->setEnabled(true);
            players[0]->getHand()[i]->setVaild();
            btn->setEnabled(true);
        }
    });





    //正式开始计时

    this->autoPlayCardTime = new QTimer(this);
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, [=](){
        this->GameLoop();
    });
    this->GameLoop();
    timer->start(2500);



    //计时自动出牌
    connect(autoPlayCardTime,&QTimer::timeout,[=](){
        CardButton *cardtmp = nullptr;
        autoPlayCardTime->stop();
        cardtmp = players[cur]->VaildCard(this->colorState,this->numberState);
        if(cardtmp != nullptr)
        {
            //出牌动画
            this->AnimapalyCard(cardtmp);
            //改变当前状态
            this->changeState(cardtmp);

            switch (this->colorState) {
            case red:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：红色"); break;
            case blue:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：蓝色"); break;
            case green:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：绿色"); break;
            case yellow:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：黄色"); break;
            default:
                break;
            }

            //删除出的牌
            players[cur]->deleteCard(cardtmp);
            qDebug()<<"剩余牌"+QString::number(players[cur]->getNumberOfCard());

            cardCache.push_back(cardtmp);
            //关闭按钮
            emit this->closeButton();

        }

        if(players[cur]->getNumberOfCard()==0)
            emit victory();
        int next = (cur + 1) % 2;
        cur = next;
    });


    //人工出牌按钮连接
    connect(btn,&QPushButton::clicked,[=](){
        int check = 0;

        CardButton* putCard =NULL;
        //检查是不是只点了一张牌
        for(int i=0;i<players[cur]->getHand().size();i++)
        {
            if(players[0]->getHand()[i]->flag==true)
            {
                check++;
                putCard=players[0]->getHand()[i];
            }
        }
        //只点一张牌时才出牌 否则无反应
        if(check == 1)
        {
            //出牌动画
            this->AnimapalyCard(putCard);

            //出的牌放入牌缓存
            cardCache.push_back(putCard);
            //改变当前出牌状态
            this->changeState(putCard);

            switch (this->colorState) {
            case red:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：红色"); break;
            case blue:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：蓝色"); break;
            case green:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：绿色"); break;
            case yellow:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：黄色"); break;
            default:
                break;
            }


            //删除手牌
            players[cur]->deleteCard(putCard);
            //关闭可点击的按钮
            emit closeButton();
            //并且关闭计时出牌计时器
            this->autoPlayCardTime->stop();

            //检查是否胜利
            if(players[cur]->getNumberOfCard()==0)
                emit victory();
            int next = (cur + 1) % 2;
            cur = next;
        }

    });



}

MainWindow::~MainWindow()
{
    delete ui;
}
//规定 0为操作者， 1为上，2为左，3为右
void MainWindow::InitBackCard(int cur)
{
    QVector<CardBack*> temp ;
    CardBack* tmp =NULL;
    for(int i=0;i<30;i++){
        tmp=new CardBack;
        tmp->setParent(this);
        tmp->move(700,50);
        temp.append(tmp);
        tmp->lower();
    }
    for(int i=0;i<7;i++){
        if(cur==1){
            temp[i]->move(700+30*i,50);
        }
    }
    this->PlayerBackCard.insert(cur,temp);

}

void MainWindow::PlayBackCard(int cur, int num)
{
    //500 250
    QVector<CardBack*> temp = this->PlayerBackCard[cur];
    if(cur==1){
        temp[num]->move(700,50);
    }

}
void MainWindow::DrawBackCard(int cur, int num)
{
    QVector<CardBack*> temp = this->PlayerBackCard[cur];
    if(cur==1){
        int i = num-1;
        temp[i]->move(500,-50);
        temp[i]->moveBack(700+30*i,50);
        temp[i]->move(700+30*i,50);
    }
}
void MainWindow::Start()
{
    initCards();
    this->cur=0;
    CardButton* inCard = new NumberButton(red, qrand()%10);
    this->changeState(inCard);

    this->State = new QLabel(this);

    //x，y，长，宽
    State->setGeometry(50,50,250,100);

    State->setLineWidth(1);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    State->setStyleSheet("border:2px solid white;");
    State->setPalette(pe);

    switch (this->colorState) {
    case red:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：红色"); break;
    case blue:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：蓝色"); break;
    case green:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：绿色"); break;
    case yellow:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：黄色"); break;
    default:
        break;
    }


    //Player* p0 = new Player("xiaotu", true, initHand(7));
    Player* p0 = new Player("xiaotu", false, initHand(7));
    Player* p1 = new Player("Computer", true, initHand(7));

    players.push_back(p0);
    players.push_back(p1);
    this->InitBackCard(1);
    this->showMessage(1,"Computer");

    QVector<CardButton*> tmp = players[0]->getHand();
    qDebug()<<tmp.size();
    qDebug()<<players[0]->getNumberOfCard();
    for(int i=0;i<tmp.size();i++)
    {
        qDebug()<<i;
        //层次上升
        tmp[i]->raise();
        tmp[i]->move(100+i*50,500);
    }
    qDebug() << "游戏开始";

}

void MainWindow::changeState(CardButton *card)
{
    if(card->getSpecificInfo().size() == 1)
    {
        this->colorState = card->getColor();
        this->numberState = card->getSpecificInfo().toInt();
        this->cardState = card;
    }



}

void MainWindow::GameLoop()
{
    if(players[cur]->getIsAI()==false){
        this->PlayerTurn();

    }else{
        //AI玩家
        this->AiTurn();
     }

}
void MainWindow::PlayerTurn()
{
    //人类玩家
    CardButton *cardtmp = nullptr;
    emit openButton();
    //获取一张可以出的牌
    cardtmp = players[cur]->VaildCard(this->colorState,this->numberState);
    //计时自动出牌
    autoPlayCardTime->start(2*1000);
    //如果没有可以出的牌
    if(cardtmp == nullptr)
    {
        if (cards.empty()) {randomCards();}
        if (!cards.empty()) {
            int pos = getMaxRight();
            //抽取一张牌
            players[cur]->DrawCard(cards.front());

            //抽牌动画
            cards.front()->moveCard(pos+50,500);
            cards.front()->raise();

            cards.dequeue();
        }
        //展示动画？
        //再次获取一张可以出的牌
        cardtmp = players[cur]->VaildCard(this->colorState,this->numberState);
        //抽完牌后遍历重新设置可以出的牌 禁用解除
        for(int i=0;i<players[cur]->getHand().size();i++)
        {
            if(!players[cur]->abledPut(players[cur]->getHand()[i],this->colorState,this->numberState))
            {
                //players[cur]->getHand()[i]->setEnabled(false);
                players[cur]->getHand()[i]->setUnvaild();
            }

        }
    }else
     {
        //如果有可以出的牌  和上面的代码可以简化
        for(int i=0;i<players[cur]->getHand().size();i++)
        {
            if(!players[cur]->abledPut(players[cur]->getHand()[i],this->colorState,this->numberState))
            {
                //players[cur]->getHand()[i]->setEnabled(false);
                players[cur]->getHand()[i]->setUnvaild();

            }

        }

    }
}
void MainWindow::AiTurn()
{
    //AI玩家
   this->sleep(1000);

        CardButton *cardtmp = nullptr;
        emit closeButton();
        cardtmp = players[cur]->playCard(this->colorState,this->numberState);
        if(cardtmp == nullptr)
        {
            //抽牌
            if (cards.empty()) {randomCards();}
            if (!cards.empty()) {
                players[cur]->DrawCard(cards.front());

                cards.dequeue();
                DrawBackCard(cur,this->players[cur]->getNumberOfCard());
                this->AImessage[cur]->setText("Computer剩余牌数："+QString::number(this->players[cur]->getNumberOfCard()));
            }
            cardtmp = players[cur]->playCard(this->colorState,this->numberState);
        }
        if(cardtmp!=nullptr)
        {
            cardCache.push_back(cardtmp);

            this->changeState(cardtmp);

            switch (this->colorState) {
            case red:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：红色"); break;
            case blue:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：蓝色"); break;
            case green:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：绿色"); break;
            case yellow:State->setText("当前数字:"+QString::number(this->numberState)+" 当前颜色：黄色"); break;
            default:
                break;
            }

            //出牌动画
           this->sleep(1000);

                this->PlayBackCard(cur,this->players[cur]->getNumberOfCard());
                cardtmp->raise();
                cardtmp->move(670,100);
                cardtmp->moveCard(500,250);


        }



        this->AImessage[cur]->setText("Computer剩余牌数："+QString::number(this->players[cur]->getNumberOfCard()));
        if(players[cur]->getNumberOfCard()==0)
            emit victory();
        int next = (cur + 1) % 2;
        cur = next;


}

// 初始化游戏卡片
// 数字牌（76张）、功能牌（24张）、万能牌（8张），合计108张
void MainWindow::initCards()
{
    //放入数字牌
        for (int i = 1; i <= 19; i++) {
            for (int j = 0; j < 4; j++) {
                NumberButton *pc = new NumberButton((Color)j, i % 10);
                pc->setParent(this);
                //放在窗口之外
                pc->move(500,800);
                connect(pc, &CardButton::clicked,[=](){
                    //qDebug()<<card->flag;
                    if(pc->flag == false)
                    {
                        pc->zoom1();
                    }
                    else
                    {
                        pc->zoom2();
                    }

                });
                cardCache.push_back(pc);
            }
        }
        this->randomCards();
}

//初始化玩家手牌，一开始每人发numOfCards张牌
//返回生成的卡牌向量
QVector<CardButton*> MainWindow::initHand(unsigned int numOfCards)
{
    QVector<CardButton*> hand;
     qDebug()<<"hewai";
        for (int i = 0; i < numOfCards; i++) {
            if (!cards.empty()) {
                qDebug()<<"he";
                hand.push_back(cards.front());
                cards.dequeue();
            }
        }
    return hand;
}


//将卡片的顺序打乱
void MainWindow::randomCards()
{
    while (!cardCache.empty()) {
        int i = rand() % cardCache.size();
        cards.enqueue(cardCache[i]);
        cardCache.erase(cardCache.begin()+i);
    }
}
//重写绘图事件
void MainWindow::paintEvent(QPaintEvent *event)
{
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建pixmap对象
    QPixmap pix;
    bool ret = pix.load(":/res/bg_welcome.png");
    if(!ret)
    {
        qDebug()<<"加载失败";
    }
    painter.drawPixmap(0,0,this->width(),this->height(),pix);


    //背景图标
    //pix.load(":/res/Title.png");
   // painter.drawPixmap(0,0,pix);

}

//辅助函数 创建AI玩家的信息
void MainWindow::showMessage(int n, QString name)
{
    QLabel *label = new QLabel(this);

    //x，y，长，宽
    label->setGeometry(250+400*n,100,200,100);

    label->setLineWidth(1);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    label->setStyleSheet("border:2px solid white;");
    label->setPalette(pe);
    label->setText(name+"剩余牌数："+QString::number(this->players[n]->getNumberOfCard()));

    this->AImessage.insert(n,label);
}
//出牌动画
void MainWindow::AnimapalyCard(CardButton* card)
{

    QVector<CardButton*> MyCards = players[0]->getHand();
    //MyCards[val]->move(500,250);
   card->moveCard(500,250);
   card->raise();
   int x = card->x();
   int y = card->y();
   for(int i=0; i<MyCards.size();i++)
   {
       if(MyCards[i]!=card)
       {
           if(MyCards[i]->x()<x)
           {
               MyCards[i]->moveCard(MyCards[i]->x()+25,500);
               MyCards[i]->flag = false;
           }else
           {
               MyCards[i]->moveCard(MyCards[i]->x()-25,500);
               MyCards[i]->flag = false;
           }
       }

   }

}
