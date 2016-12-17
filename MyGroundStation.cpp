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
    string serialName("/dev/ttyUSB0");
    int baudRate(115200);
    init(serialName,baudRate);
}


MyGroundStation::~MyGroundStation()
{
    delete []send_queue;
    delete m_hd;
}

void MyGroundStation::setQuadNum(int n)//Reserved to be deleted
{
    quadNum=n;
    test+=1;
}


void MyGroundStation::init(string device, unsigned int baudrate)
{
    quadNum=0;
    test=31;

    cout<<"---Zigbee connection Info---" << endl;
    cout<<"Serial port: "<< device.c_str() << endl;
    cout<<"baudrate: " << baudrate << endl;
    cout<<"--------"<<endl;

    m_hd = new Serial_HardDriver(device, baudrate);
    m_hd->init();


//发送与接收缓冲清空
    send_queue = new Queue(500);
    send_queue->SetEmpty();
    filter.recvIndex = 0;


    int ret;
    ret = pthread_create(&m_Tid, 0,SerialThread,(void*)this);		//zigbee收发线程
    if(0 != ret)
        cout<<string("Cannot create new thread for zigbee serial!");
    else
        cout<<string("Succeed to create thread for zigbee serial");
}

void MyGroundStation::sendPoll()
{
    size_t a ;

/*	for(int i=0;i<SendMsgNum;i++)
    {
        //此处应禁止修改SendMsgQueue
        if(SendMsgQueue[i].UpdateFlag > 0)		//数据更新了
        {
            m_hd->lockMemory();
            a = SendMsgQueue[i].msg.length();				//get length
            m_hd->send(SendMsgQueue[i].msg,a);												//write
            m_hd->freeMemory();
            SendMsgQueue[i].UpdateFlag = 0;
        }
        //允许修改SendMsgQueue
    }
    */
    unsigned char buf[500];

    m_hd->lockMemory();
    //给队列上锁
    a = send_queue->GetQueueLength();
    for(int i=0;i<a;i++)
        send_queue->DeQueue(buf+i);
    //队列解锁

    m_hd->send(buf,a);
    m_hd->freeMemory();
}

#define BUFFER_SIZE 1024

void MyGroundStation::readPoll()
{
    int read_len;
    uint8_t buf[BUFFER_SIZE];
    read_len = m_hd->readall(buf,BUFFER_SIZE);

    for (int i = 0; i < read_len; i++)
  {
    byteHandler(buf[i]);			//对每一位数据处理函数
  }
}
int MyGroundStation::checkData()//Reserved to be modified
{
    return 0;
}

void MyGroundStation::handleData()
{
    msgHeader *p_head = (msgHeader *)(filter.recvBuf);

    if(!checkData())		//校验正确
    {
        switch(p_head->msgID)
        {
            case msgID_InitShake :
            decodeInitShake();
            break;

            default:
                break;
        }
    }
}

void MyGroundStation::byteHandler(const uint8_t in_data)
{

    if(filter.recvIndex == 0)
    {
        if(in_data == 0xFD)
        {
            cout<<"receive 0xFE"<<endl;
            filter.recvBuf[filter.recvIndex] = in_data;
            filter.recvIndex++;
        }
    }
    else	//已经顺利收到包头
    {
        filter.recvBuf[filter.recvIndex] = in_data;
        filter.recvIndex++;
        if(filter.recvIndex >= 2)												//已收到数据包长度位
        {
            if(filter.recvIndex == filter.recvBuf[1])     //长度收满,长度校验完成
            {
                handleData();		//数据处理 校验 过滤 解码 消息发布等
                cout<<"Finish Receive"<<endl;
                filter.recvIndex = 0;		//清空
            }
        }
    }


}

void MyGroundStation::decodeInitShake()
{
  int length = 5;
  u16 temp;
  unsigned char *a=filter.recvBuf;
  DecodeU16Data(&temp,a+8);//29 is reserved to be changed to a var
  myQuad[temp].global_position.ID = temp;
  DecodeDoubleData(&myQuad[temp].global_position.latitude,a+length);length+=1;
  DecodeDoubleData(&myQuad[temp].global_position.longitude,a+length);length+=1;
  DecodeDoubleData(&myQuad[temp].global_position.altitude,a+length);length+=1;
  if(myQuad.size()>quadNum)
  {
      quadNum=myQuad.size();
      QString str;
      str.setNum(quadNum);
      emit setQuadText(QString("Info: Quad Add In: ")+str+QString(" !"));
  }
}


