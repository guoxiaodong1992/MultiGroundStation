/** @file MyGroundStation.h
 *  @version 1.0.0
 *  @date Sep 15th, 2016
 *  @author Guo Xiaodong
 *  @brief
 *  Ground Station for Multi Quads. Header File.
 */
#ifndef ZIGBEE_H
#define ZIGBEE_H
#include <iostream>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <vector>
#include <map>
#include "MyQuad.h"
#include "MyDataProcess.h"

using namespace std;


class MyGroundStation: public QObject
{
   Q_OBJECT
   public:
        explicit MyGroundStation(QObject *parent=0);
        void decode_InitShake();
   signals:
        void setQuadText(QString text);
   public slots:
        void setQuadNum(int);
        int  js(){return test;}
   private:
        int quadNum;
        float test;
        map<u16,MyQuad> myQuad;
        unsigned char a[100];
};




#endif

