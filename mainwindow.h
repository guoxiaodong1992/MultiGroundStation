
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
public slots:
    void slotTest();
    void addJavaScriptObject();


signals:

private:
    Ui::MainWindow *ui;
    MyGroundStation *myGrdStn;
};

#endif // MAINWINDOW_H
