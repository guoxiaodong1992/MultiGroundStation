/** @file MyGroundStation.h
 *  @version 1.0.0
 *  @date Sep 15th, 2016
 *  @author Guo Xiaodong
 *  @brief
 *  Ground Station for Multi Quads. Header File.
 */
#ifndef MY_GROUNDSTATION_
#define MY_GROUNDSTATION_

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
#include "Queue.h"
#include "SerialHardDriver.h"
#include "MsgCode.h"


using namespace std;


class MyGroundStation: public QObject
{
   Q_OBJECT
   public:
        Queue *send_queue;												//for send
        SDKFilter filter;													//for receive
        Serial_HardDriver *m_hd;
        explicit MyGroundStation(QObject *parent=0);
        ~MyGroundStation();

        void decodeInitShake();
        void decodePosi();
        void decodeVel();
        void decodeAttitude();
        void decodeBodyFrameAck();
        int encodeLocalFrame(u16 zigbeeID,unsigned char *a);
        int encodeTakeoff(u16 zigbeeID,unsigned char *a);
        int encodeCmdAck(unsigned char msgID,u16 zigbeeID,unsigned char *a);
        int encodeNoArguCmd(unsigned char msgID,u16 zigbeeID,unsigned char *a);
        int encodeShapeConfig(ShapeConfig tmp,unsigned char uavID,unsigned char *a);
        unsigned char CountSum(unsigned char *a,int length);

        void handleData();
        void init(string device, unsigned int baudrate);
        int  checkData();
        void readPoll();
        void sendPoll();

   signals:
        void setLeaderText(QString text);
        void setComBoxText(QString text);
        void setStateText(QString text);
        void setConsoleText(QString text);
   public slots:
        void setMeet();
        void setCruise();
        void setTakingoff();
        void setTask();
        void setRtl();
        void setLanding();
        void setNumCfm();
        void setHover();
        int  js(){return test;}
   private:
        int quadNum;
        float test;
        float takeoffHeight;
        int leaderID;
        map<u16,MyQuad> myQuad;
        unsigned char a[100];
        MAP_ID map_id[10] = {	{0x00,0x0000},
                              {0x01,0x0100},
                              {0x02,0x3E14},
                              {0x03,0x7B28},
                              {0x04,0x0004},
                              {0x05,0x0005},
                              {0x06,0x0006},
                              {0x07,0x0007},
                              {0x08,0x0008},
                              {0x09,0x0009}};

        pthread_t m_Tid;
        void byteHandler(const uint8_t in_data);

};


 static void* SerialThread(void* param)
{
    MyGroundStation* p_mygs = (MyGroundStation*)(param);
    cout<<endl<<"Thread"<<endl;

    while(true)
    {
        p_mygs->readPoll();
        p_mygs->sendPoll();
        usleep(100000);
    }
}

#endif





