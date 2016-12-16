#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    GPS gps;
    gps.altitude=10;
    gps.longitude=131;
    gps.latitude=31;
    unsigned char al[100];
    int length=encode_InitShake(gps,al);
    for(int i = 0;i<length;i++)
    {
        std::cout<<std::hex<<static_cast<int>(al[i]);
    }

    return a.exec();
}
