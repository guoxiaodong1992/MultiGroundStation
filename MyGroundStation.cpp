/** @file MyGroundStation.cpp
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
    quadNum=0;
    test=31;
}

void MyGroundStation::setQuadNum(int n)//Reserved to be deleted
{
    quadNum=n;
    test+=1;
}

void MyGroundStation::decode_InitShake()
{
  int length = 5;
  u16 temp;
  DecodeU16Data(&temp,a+29);//29 is reserved to be changed to a var
  myQuad[temp].global_position.ID = temp;
  DecodeDoubleData(&myQuad[temp].global_position.latitude,a+length);length+=8;
  DecodeDoubleData(&myQuad[temp].global_position.longitude,a+length);length+=8;
  DecodeDoubleData(&myQuad[temp].global_position.altitude,a+length);length+=8;
  if(myQuad.size()>quadNum)
  {
      quadNum=myQuad.size();
      QString str;
      str.setNum(quadNum);
      emit setQuadText(QString("Info:Quad Add In: ")+str+QString(" !"));
  }
}

