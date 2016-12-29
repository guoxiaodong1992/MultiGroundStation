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
    quadNum=0;
    takeoffHeight=5.0;
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
    if(CountSum(filter.recvBuf,filter.recvIndex-3) == filter.recvBuf[filter.recvIndex-3])
    {
        //cout<<"check sum ok"<<endl;
        return 0;
    }
    else
    {
        //cout<<"check sum error"<<endl;
        return 1;
    }

}



void MyGroundStation::byteHandler(const uint8_t in_data)
{
    std::cout<<' '<<hex<<(u16)in_data;
    if(filter.recvIndex == 0)
        {
            if(in_data == 0xFD)
            {
                cout<<"receive 0xFD"<<endl;
                filter.recvBuf[filter.recvIndex] = in_data;
                filter.recvIndex++;
            }
        }
        else	//已经顺利收到包头
        {
            filter.recvBuf[filter.recvIndex] = in_data;//
            filter.recvIndex++;
            if(filter.recvIndex >= 2)
                //已收到数据包长度位
            {
            if(filter.recvIndex == filter.recvBuf[1]+6)     //在Zigbee点对点模式下，真实的数据长度为数据去长度+6,(前面4+后面2）长度收满,长度校验完成
                {
                    handleData();		//数据处理 校验 过滤 解码 消息发布等
                    cout<<"Finish Receive"<<endl;
                    filter.recvIndex = 0;		//清空
                }
            }
        }

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
            case msgID_LocalFrame:
            decodeBodyFrameAck();
            break;
            default:
                break;
        }
    }
}



//*******************************************************************************************//
void MyGroundStation::decodeInitShake()
{
  int length = 5;
  u16 zigbeeID;
  unsigned char *a=filter.recvBuf;
  DecodeU16Data(&zigbeeID,a+30);//30 is reserved to be changed to a var
  myQuad[zigbeeID].zigbeeID= zigbeeID;//UAV ID
  DecodeDoubleData(&myQuad[zigbeeID].global_position.latitude,a+length);length+=8;
  DecodeDoubleData(&myQuad[zigbeeID].global_position.longitude,a+length);length+=8;
  DecodeDoubleData(&myQuad[zigbeeID].global_position.altitude,a+length);length+=8;
  myQuad[zigbeeID].quadStatus=INIT_SHAKE;
  if(myQuad.size()>quadNum)
  {
      quadNum=myQuad.size();
      QString str;
      str.setNum(quadNum);
      emit setConsoleText(QString("INFO: Quad Add In: ")+str+QString(" !"));
  }
  unsigned char b[50];//50 Reserved to be define
  int len;
  len=encodeCmdAck(msgID_InitShakeAck,zigbeeID,b);
  for(int i=0;i<len;i++)
  {
      std::cout<<' '<<hex<<u16(b[i]);
      send_queue->EnQueue(b[i]);
  }
}

void MyGroundStation::decodeBodyFrameAck()
{
    u16 zigbeeID;
    unsigned char *a=filter.recvBuf;
    DecodeU16Data(&zigbeeID,a+9);//9 is reserved to be changed to a var
    std::cout<<"RECEIVE BODY FRAME! ID： "<<hex<<zigbeeID<<std::endl;
    myQuad[zigbeeID].quadStatus= BODY_FRAME;//UAV ID
    QString str;
    str.setNum(zigbeeID);
    emit setConsoleText(QString("INFO: Body Frame Ack,ID: ")+str+QString(" !"));
}


void MyGroundStation::decodePosi()
{
  int length = 5;
  u16 zigbeeID;
  unsigned char *a=filter.recvBuf;
  DecodeU16Data(&zigbeeID, a+18);//18 is reserved to be changed to a var
  DecodeFloatData(&myQuad[zigbeeID].local_position.x,a+length);length+=4;
  DecodeFloatData(&myQuad[zigbeeID].local_position.y,a+length);length+=4;
  DecodeFloatData(&myQuad[zigbeeID].local_position.z,a+length);length+=4;

}


void MyGroundStation::decodeVel()
{
  int length = 5;
  u16 zigbeeID;
  unsigned char *a=filter.recvBuf;
  DecodeU16Data(&zigbeeID, a+18);//18 is reserved to be changed to a var
  DecodeFloatData(&myQuad[zigbeeID].local_vel.x,a+length);length+=4;
  DecodeFloatData(&myQuad[zigbeeID].local_vel.y,a+length);length+=4;
  DecodeFloatData(&myQuad[zigbeeID].local_vel.z,a+length);length+=4;
}


void MyGroundStation::decodeAttitude()
{
  int length = 5;
  u16 zigbeeID;
  unsigned char *a=filter.recvBuf;
  DecodeU16Data(&zigbeeID, a+22);//18 is reserved to be changed to a var
  DecodeFloatData(&myQuad[zigbeeID].atti.q0,a+length);length+=4;
  DecodeFloatData(&myQuad[zigbeeID].atti.q1,a+length);length+=4;
  DecodeFloatData(&myQuad[zigbeeID].atti.q2,a+length);length+=4;
  DecodeFloatData(&myQuad[zigbeeID].atti.q3,a+length);length+=4;
}


//**********************************************************************************************//
//LocalFrame message is sent from gs to uav
//send local frame to specific uav.if uavID is 0xff,that means broadcasting local frame to all uav.
//In general, we use 0xff when configurate
int MyGroundStation::encodeLocalFrame(u16 zigbeeID,unsigned char *a)
{
    int length = 0;

    a[length] = 0xFD;  length++;							//包头
    a[length] = 0;   	length++;								//data length

  if(zigbeeID!=0xffff)
  {
    EncodeU16Data(&zigbeeID,a+length);length+=2;		//uav对应的zigbee短地址
  }
    else
  {
    a[length] = 0xff;  length++;
    a[length] = 0xff; length++;
  }

    a[length] = msgID_LocalFrame; length++;		//message ID
    //////数据拆分解码////////////
    EncodeDoubleData(&myQuad[zigbeeID].global_position.latitude, a+length);length+=8;
    EncodeDoubleData(&myQuad[zigbeeID].global_position.longitude, a+length);length+=8;
    EncodeDoubleData(&myQuad[zigbeeID].global_position.altitude, a+length);length+=8;
  /**补上data length**/
    a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

  return length;
}


/*ShapeConfig message is sent from gs/UAV to uav
  Send ShapeConfig to specific uav.if uavID is 0xff,that means broadcasting ShapeConfig to all uav.
  In general, We use 0xff when configurate
*/

int encode_ShapeConfig(u16 zigbeeID,unsigned char *a)
{

}


//Posi数据只使用高度数据设置起飞高度
int MyGroundStation::encodeTakeoff(u16 zigbeeID,unsigned char *a)
{
    int length = 0;

    a[length] = 0xFD;  length++;							//包头
    a[length] = 0;   	length++;								//data length

  if( zigbeeID!=0xffff)
  {
    EncodeU16Data(&zigbeeID, a+length);length+=2;		//uav对应的zigbee短地址
  }
  else
  {
    a[length] = 0xff;  length++;
    a[length] = 0xff; length++;
  }

    a[length] = msgID_TakeOff; length++;		//message ID

    EncodeFloatData(&takeoffHeight, a+length);length+=4;

  /**补上data length**/
  a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

    return length;
}


//无参数命令编码
int MyGroundStation::encodeNoArguCmd(unsigned char msgID,u16 zigbeeID,unsigned char *a)
{
  int length = 0;

  a[length] = 0xFD;  length++;							//包头
    a[length] = 0;   	length++;								//data length

  if(zigbeeID!=0xffff)
  {
    EncodeU16Data(&zigbeeID, a+length);length+=2;		//uav对应的zigbee短地址
  }
  else
  {
    a[length] = 0xff;  length++;
    a[length] = 0xff; length++;
  }

  a[length] = msgID; length++;		//message ID
  /**补上data length**/
  a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

    return length;
}



int MyGroundStation::encodeCmdAck(unsigned char msgID,u16 zigbeeID,unsigned char *a)
{
    int length = 0;

    a[length] = 0xFD;  length++;							//包头
    a[length] = 0;   	length++;								//data length

    if(zigbeeID!=0xffff)
    {
       EncodeU16Data(&zigbeeID, a+length);length+=2;		//uav对应的zigbee短地址
    }
    else
    {
        a[length] = 0xff;  length++;
        a[length] = 0xff; length++;
    }

    a[length] = msgID; length++;		//message ID
    //////数据////////////
  a[length] = 'a';length++;
  a[length] = 'c';length++;
  a[length] = 'k';length++;
    /**补上data length**/
    a[1] = (unsigned char)length-3;//

    ////计算校验和//////
    a[length]=CountSum(a,length);length++;

    return length;
}



void MyGroundStation::setNumCfm()
{
    QString str;
    str.setNum(quadNum);
    std::cout<<"Body Frame"<<std::endl;
    for(int i=0;i<5;i++)
    {
        map<u16,MyQuad>::iterator it=myQuad.begin();
        while(it!=myQuad.end())
        {

            if(it->second.quadStatus!=BODY_FRAME)
            {
            unsigned char b[50];//50 Reserved to be define
            int len=encodeLocalFrame(it->second.zigbeeID,b);
            for(int i=0;i<len;i++)
            {
                std::cout<<' '<<hex<<u16(b[i]);
                send_queue->EnQueue(b[i]);
            }
            }
            sleep(0.5);
            it++;
        }
    }
    emit setConsoleText(QString("INFO: ")+str+QString(" Quads Online! Num Confirm! Ready!"));
}




void MyGroundStation::setTakingoff()
{
    map<u16,MyQuad>::iterator it=myQuad.begin();
    while(it!=myQuad.end())
    {

        if(it->second.quadStatus!=BODY_FRAME)
        {
        unsigned char b[50];//50 Reserved to be define
        int len=encodeTakeoff(it->second.zigbeeID,b);
        for(int i=0;i<len;i++)
        {
            std::cout<<std::endl;
            std::cout<<' '<<hex<<u16(b[i]);
            send_queue->EnQueue(b[i]);
        }
        }
        it++;
    }
    emit setConsoleText(QString("INFO: Taking off!"));
}

void MyGroundStation::setMeet()

{
    unsigned char msgID=msgID_Meet ;
    u16 zigbeeID=0xffff;
    unsigned char b[50];//50 Reserved to be define
    int len=encodeNoArguCmd(msgID,zigbeeID,b);
    for(int i=0;i<len;i++)
    {
        std::cout<<std::endl;
        std::cout<<' '<<hex<<u16(b[i]);
        send_queue->EnQueue(b[i]);
    }
    emit setConsoleText(QString("INFO: Meet Start!"));

}

void MyGroundStation::setCruise()
{
    unsigned char msgID=msgID_Fly  ;
    u16 zigbeeID=0xffff;
    unsigned char b[50];//50 Reserved to be define
    int len=encodeNoArguCmd(msgID,zigbeeID,b);
    for(int i=0;i<len;i++)
    {
        std::cout<<std::endl;
        std::cout<<' '<<hex<<u16(b[i]);
        send_queue->EnQueue(b[i]);
    }
    emit setConsoleText(QString("INFO: Cruse Start!"));
}


void MyGroundStation::setTask()
{
    emit setConsoleText(QString("INFO: Task Start!"));
}

void MyGroundStation::setHover()
{
    unsigned char msgID=msgID_Stop  ;
    u16 zigbeeID=0xffff;
    unsigned char b[50];//50 Reserved to be define
    int len=encodeNoArguCmd(msgID,zigbeeID,b);
    for(int i=0;i<len;i++)
    {
        std::cout<<std::endl;
        std::cout<<' '<<hex<<u16(b[i]);
        send_queue->EnQueue(b[i]);
    }
    emit setConsoleText((QString("INFO: HOVER！")));
}

void MyGroundStation::setLanding()
{
    unsigned char msgID=msgID_Land  ;
    u16 zigbeeID=0xffff;
    unsigned char b[50];//50 Reserved to be define
    int len=encodeNoArguCmd(msgID,zigbeeID,b);
    for(int i=0;i<len;i++)
    {
        std::cout<<std::endl;
        std::cout<<' '<<hex<<u16(b[i]);
        send_queue->EnQueue(b[i]);
    }
    emit setConsoleText(QString("INFO: Landing!"));
}

void MyGroundStation::setRtl()
{
    unsigned char msgID=msgID_Return  ;
    u16 zigbeeID=0xffff;
    unsigned char b[50];//50 Reserved to be define
    int len=encodeNoArguCmd(msgID,zigbeeID,b);
    for(int i=0;i<len;i++)
    {
        std::cout<<std::endl;
        std::cout<<' '<<hex<<u16(b[i]);
        send_queue->EnQueue(b[i]);
    }
    emit setConsoleText(QString("INFO: Return Home!"));
}

void MyGroundStation::setState()
{
    emit setStateText(QString("Ready to Take Off"));
}


unsigned char MyGroundStation::CountSum(unsigned char *a,int length)
{
    unsigned char sum = 0x00;
    for(int i=0;i<length;i++)
    {
        sum+=a[i];
    }
    return sum;
}







