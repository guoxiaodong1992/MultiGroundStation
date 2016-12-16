
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebView>
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
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void slotTest();
    void addJavaScriptObject();


signals:

private:
    Ui::MainWindow *ui;
    MyGroundStation *myGrdStn;
};

#endif // MAINWINDOW_H
