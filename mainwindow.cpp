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
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),ui->pushButton,SLOT(setDisabled(bool)));//数量确认之后允许起飞
    connect(ui->pushButton_3,SIGNAL(clicked()),this->myGrdStn,SLOT(setNumCfm()));//数量确认之后允许起飞
    connect(this->myGrdStn,SIGNAL(setNumCfmText(QString)),ui->textBrowser,SLOT(append(QString)));
    connect(ui->pushButton,SIGNAL(clicked(bool)),ui->pushButton_2,SLOT(setDisabled(bool)));//起飞之后允许返航
    connect(ui->leftList,SIGNAL(currentRowChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
}

void MainWindow::initUi()
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);//起飞按钮，默认是灰的
    ui->pushButton_2->setEnabled(false);//返航按钮，默认是灰的
    ui->pushButton_3->setEnabled(true);//数量确认按钮，默认是开的
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
