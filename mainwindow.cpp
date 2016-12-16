#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->webView->show();
    ui->webView->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
    ui->webView->settings()->setAttribute(QWebSettings::PluginsEnabled,true);

    myGrdStn=new MyGroundStation(this);

    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    connect(ui->spinBox,SIGNAL(valueChanged(int)),this->myGrdStn,SLOT(setQuadNum(int)));
    connect(this->myGrdStn,SIGNAL(setQuadNumText(QString)),ui->textBrowser,SLOT(append(QString)));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),ui->pushButton,SLOT(setDisabled(bool)));//数量确认之后允许起飞
    connect(ui->pushButton,SIGNAL(clicked(bool)),ui->pushButton_2,SLOT(setDisabled(bool)));//起飞之后允许返航
    addJavaScriptObject();
    connect(ui->webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),this, SLOT(addJavaScriptObject()));
    connect(ui->webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),this, SLOT(slotTest()));
    ui->webView->setUrl(QUrl("/home/gxd/htmlapi/baiduapi.html"));


}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addJavaScriptObject()
{
    std::cout<<"SUCCEED!"<<std::endl;
    this->ui->webView->page()->mainFrame()->addToJavaScriptWindowObject(QString("myGrdStn"),this->myGrdStn);
}

void MainWindow::slotTest()
{
    std::cout<<"SUCCEED!"<<std::endl;
}
