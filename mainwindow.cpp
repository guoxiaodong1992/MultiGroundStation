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
    connect(this->myGrdStn,SIGNAL(setQuadText(QString)),ui->textBrowser,SLOT(append(QString)));//刷新界面文本框信息
    connect(ui->webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),this, SLOT(addJavaScriptObject()));//地图刷新调取暴露类参数
    connect(ui->pushButtonNum,SIGNAL(clicked(bool)),ui->pushButtonTakingoff,SLOT(setDisabled(bool)));//数量确认之后允许起飞
    connect(ui->pushButtonNum,SIGNAL(clicked()),this->myGrdStn,SLOT(setNumCfm()));//数量确认信息台信息
    connect(this->myGrdStn,SIGNAL(setNumCfmText(QString)),ui->textBrowser,SLOT(append(QString)));
    connect(ui->pushButtonTakingoff,SIGNAL(clicked()),this->myGrdStn,SLOT(setTakingoff()));
    connect(this->myGrdStn,SIGNAL(setTakingoffText(QString)),ui->textBrowser,SLOT(append(QString)));
    connect(ui->pushButtonCruise,SIGNAL(clicked()),this->myGrdStn,SLOT(setCruise()));
    connect(this->myGrdStn,SIGNAL(setCruiseText(QString)),ui->textBrowser,SLOT(append(QString)));
    connect(ui->pushButtonTask,SIGNAL(clicked()),this->myGrdStn,SLOT(setTask()));
    connect(this->myGrdStn,SIGNAL(setTaskText(QString)),ui->textBrowser,SLOT(append(QString)));
    connect(ui->pushButtonLanding,SIGNAL(clicked()),this->myGrdStn,SLOT(setLanding()));
    connect(this->myGrdStn,SIGNAL(setLandingText(QString)),ui->textBrowser,SLOT(append(QString)));
    connect(ui->pushButtonRtl,SIGNAL(clicked()),this->myGrdStn,SLOT(setRtl()));
    connect(this->myGrdStn,SIGNAL(setRtlText(QString)),ui->textBrowser,SLOT(append(QString)));
    connect(ui->pushButtonTakingoff,SIGNAL(clicked(bool)),ui->pushButtonRtl,SLOT(setDisabled(bool)));//起飞之后允许返航
    connect(ui->leftList,SIGNAL(currentRowChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(ui->pushButtonNum,SIGNAL(clicked()),this->myGrdStn,SLOT(setState()));//等待修改
    connect(this->myGrdStn,SIGNAL(setStateText(QString)),ui->textBrowserState,SLOT(append(QString)));//等待修改
}

void MainWindow::initUi()
{
    ui->setupUi(this);
    ui->pushButtonNum->setEnabled(true);//数量确认按钮，默认是开的
    ui->pushButtonTakingoff->setEnabled(false);//起飞按钮，默认是灰的
    ui->pushButtonRtl->setEnabled(false);//返航按钮，默认是灰的
    ui->pushButtonCruise->setEnabled(false);//开始巡航按钮，默认灰
    ui->pushButtonLanding->setEnabled(false);//降落按钮，默认灰
    ui->pushButtonTask->setEnabled(false);//开始任务按钮，默认灰
    ui->webView->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);//使能javascript暴露
    ui->webView->settings()->setAttribute(QWebSettings::PluginsEnabled,true);
    addJavaScriptObject();
    ui->webView->setUrl(QUrl("/home/gxd/Qt/MultiGroundStation/htmlapi/baiduapi.html"));
    ui->webView->show();

    ui->leftList->insertItem(0,tr("Review"));
    ui->leftList->insertItem(1,tr("Quads"));
    ui->leftList->insertItem(2,tr("Plan"));


}


void MainWindow::addJavaScriptObject()
{;
    this->ui->webView->page()->mainFrame()->addToJavaScriptWindowObject(QString("myGrdStn"),this->myGrdStn);
}

void MainWindow::slotTest()
{
    std::cout<<"SUCCEED!"<<std::endl;
}


MainWindow::~MainWindow()
{
    delete ui;
    delete myGrdStn;
}
