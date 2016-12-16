/** @file MyGroundStation.h
 *  @version 1.0.0
 *  @date Sep 15th, 2016
 *  @author Guo Xiaodong
 *
 *  @brief
 *  Ground Station for Multi Quads. CPP File.
 */
#include <iostream>
#include "MyGroundStation.h"


MyGroundStation::MyGroundStation(QObject *parent):
    QObject(parent)
{
    test=31;
}

void MyGroundStation::setQuadNum(int n)
{
    quadNum=n;
    test+=1;
    QString str;
    str.setNum(n);
    emit setQuadNumText(QString("Info:Quad Add In: ")+str+QString(" !"));
}

