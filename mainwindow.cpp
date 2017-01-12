#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)


{
    ui=new Ui::MainWindow();
    myGrdStn=new MyGroundStation(this);
    initUi();//初始化界面参数
    initConnect();//初始化信号和槽连接
}

void MainWindow::initConnect()
{

    connect(ui->webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),this, SLOT(addJavaScriptObject()));//地图刷新调取暴露类参数 
    connect(ui->pushButtonNum,SIGNAL(clicked()),this->myGrdStn,SLOT(setNumCfm()));//数量确认信息
    connect(ui->pushButtonTakingoff,SIGNAL(clicked()),this->myGrdStn,SLOT(setTakingoff()));
    connect(ui->pushButtonMeet,SIGNAL(clicked()),this->myGrdStn,SLOT(setMeet()));//会和
    connect(ui->pushButtonCruise,SIGNAL(clicked()),this->myGrdStn,SLOT(setCruise()));//开始巡航
    connect(ui->pushButtonHover,SIGNAL(clicked()),this->myGrdStn,SLOT(setHover()));//一键悬停
    connect(ui->pushButtonTask,SIGNAL(clicked()),this->myGrdStn,SLOT(setTask()));//开始任务信息台信息
    connect(ui->pushButtonLanding,SIGNAL(clicked()),this->myGrdStn,SLOT(setLanding()));//降落信息台信息
    connect(ui->pushButtonRtl,SIGNAL(clicked()),this->myGrdStn,SLOT(setRtl()));//返航信息台信息
    connect(ui->leftList,SIGNAL(currentRowChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(this->myGrdStn,SIGNAL(setStateText(QString)),ui->textBrowserState,SLOT(setText(QString)));//状态栏函数激发
    connect(this->myGrdStn,SIGNAL(setConsoleText(QString)),ui->textBrowser,SLOT(append(QString)));//信息台函数激发
    connect(this->myGrdStn,SIGNAL(setComBoxText(QString)),this,SLOT(addItemsComBox(QString)));//下拉框函数激发
    connect(this->myGrdStn,SIGNAL(setLeaderText(QString)),ui->textBrowserLeader,SLOT(setText(QString)));//头机编队数字栏函数激发
    connect(this,SIGNAL(setShapeText(QString)),ui->textBrowserShape,SLOT(append(QString)));//编队信息栏函数激发
    //connect(ui->pushButtonNum,SIGNAL(clicked(bool)),ui->pushButtonTakingoff,SLOT(setDisabled(bool)));//数量确认之后允许起飞
    connect(this->myGrdStn,SIGNAL(setReadyTakeoffBool(bool)),ui->pushButtonTakingoff,SLOT(setEnabled(bool)));//全部队形发送成功之后可以起飞，未测试
    connect(ui->pushButtonTakingoff,SIGNAL(clicked(bool)),ui->pushButtonRtl,SLOT(setDisabled(bool)));//起飞之后允许返航
    connect(ui->pushButtonTakingoff,SIGNAL(clicked(bool)),ui->pushButtonHover,SLOT(setDisabled(bool)));//起飞之后允许悬停
    connect(ui->pushButtonTakingoff,SIGNAL(clicked(bool)),ui->pushButtonLanding,SLOT(setDisabled(bool)));//起飞之后允许降落
    connect(ui->pushButtonTakingoff,SIGNAL(clicked(bool)),ui->pushButtonTask,SLOT(setDisabled(bool)));//起飞之后允许执行任务
    connect(ui->pushButtonTakingoff,SIGNAL(clicked(bool)),ui->pushButtonCruise,SLOT(setDisabled(bool)));//起飞之后允许巡航
    connect(ui->pushButtonTakingoff,SIGNAL(clicked(bool)),ui->pushButtonMeet,SLOT(setDisabled(bool)));//起飞之后允许会和
    connect(ui->pushButtonShape,SIGNAL(clicked(bool)),this,SLOT(setShape()));//发送编队文件


}

void MainWindow::initUi()
{
    ui->setupUi(this);
    //初始化为亮的按钮
    ui->pushButtonNum->setEnabled(true);//数量确认按钮
    //初始化为灰色的按钮
    ui->pushButtonTakingoff->setEnabled(false);//起飞按钮
    ui->pushButtonRtl->setEnabled(false);//返航按钮
    ui->pushButtonMeet->setEnabled(false);//会和按钮
    ui->pushButtonCruise->setEnabled(false);//开始巡航按钮
    ui->pushButtonLanding->setEnabled(false);//降落按钮
    ui->pushButtonTask->setEnabled(false);//开始任务按钮
    ui->pushButtonHover->setEnabled(false);//悬停按钮
    ui->webView->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);//使能javascript暴露
    ui->webView->settings()->setAttribute(QWebSettings::PluginsEnabled,true);
    addJavaScriptObject();
    ui->webView->setUrl(QUrl("/home/gxd/Qt/MultiGroundStation/htmlapi/baiduapi.html"));
    ui->webView->show();

    ui->leftList->insertItem(0,tr("Review"));
    ui->leftList->insertItem(1,tr("Plan"));
    ui->leftList->insertItem(2,tr("Quans Info"));


}


void MainWindow::addJavaScriptObject()
{
    this->ui->webView->page()->mainFrame()->addToJavaScriptWindowObject(QString("myGrdStn"),this->myGrdStn);
}

void MainWindow::setShape()
{
    static int firstSetShape;
    if(!firstSetShape)//发送i=0,j=0队形提示消息
    {
     ShapeConfig sapConfig;
     sapConfig.j=0;
     sapConfig.x=0;
     sapConfig.y=0;
     sapConfig.z=0;
     sapConfig.fi=0;
     unsigned char b[50];//50 Reserved to be define
     int len= myGrdStn->encodeShapeConfig(sapConfig,0xff,b);//队形信息广播给所有uav
     for(int i=0;i<len;i++)
     {
         std::cout<<' '<<hex<<u16(b[i]);
         myGrdStn->send_queue->EnQueue(b[i]);
     }
      std::cout<<" Before ShapConfig"<<std::endl;
      firstSetShape=1;//标志位置为1
    }
    else
    {
    int uavID =ui->comboBoxFollower->currentText().toInt();
     ShapeConfig sapConfig;
     sapConfig.j=uavID;
     sapConfig.x=ui->doubleSpinBoxShapeX->value();
     sapConfig.y=ui->doubleSpinBoxShapeY->value();
     sapConfig.z=ui->doubleSpinBoxShapeZ->value();
     sapConfig.fi=ui->doubleSpinBoxFi->value();
     unsigned char b[50];//50 Reserved to be define
     int len= myGrdStn->encodeShapeConfig(sapConfig,0xff,b);//队形信息广播给所有uav
     for(int i=0;i<len;i++)
     {
         std::cout<<' '<<hex<<u16(b[i]);
         myGrdStn->send_queue->EnQueue(b[i]);
     }
      std::cout<<" ShapConfig"<<std::endl;
      QString str,strx,stry,strz;
      str.setNum(uavID);
      strx=QString("%1").arg(sapConfig.x);
      stry= QString("%1").arg(sapConfig.y);
      strz= QString("%1").arg(sapConfig.z);
      emit setShapeText(QString("SetShape! ID: ")+str+QString(" X:")+strx+QString(" Y:")+stry+QString(" Z:")+strz);
    }
}

void MainWindow::addItemsComBox(QString s)
{
    ui->comboBoxFollower->addItem(s);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete myGrdStn;
}
