/** @file MyGroundStation.h
 *  @version 1.0.0
 *  @date Sep 15th, 2016
 *  @author Guo Xiaodong
 *  @brief
 *  Ground Station for Multi Quads. Header File.
 */

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
        void handleData();
        void init(string device, unsigned int baudrate);
        int  checkData();
        void readPoll();
        void sendPoll();

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





