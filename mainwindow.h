
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStackedWidget>
#include <QListWidget>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebView>
#include <QHBoxLayout>
#include "MsgCode.h"
#include "MyDataProcess.h"
#include "MyGroundStation.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int quadNum;
    int location;
    QLabel *q;
public:
    explicit MainWindow(QWidget *parent = 0);
    void initConnect();
    void initUi();   
    ~MainWindow();
signals:
     void setShapeText(QString text);
public slots:
    void addJavaScriptObject();
    void setShape();
    void addItemsComBox(QString s);


signals:

private:
    Ui::MainWindow *ui;
    MyGroundStation *myGrdStn;
};

#endif // MAINWINDOW_H
