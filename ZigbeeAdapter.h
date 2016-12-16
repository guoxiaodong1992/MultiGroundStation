#ifndef _ZIGBEEADAPTER_H
#define _ZIGBEEADAPTER_H


#include "SerialHardDriver.h"
#include "Queue.h"
#include "TypeDef.h"


#include <cstdlib>
#include <pthread.h>
#include <iostream>


using namespace std;

//#define SendMsgNum  1

class ZigbeeAdapter
{
private:
    pthread_t m_Tid;
    void byteHandler(const uint8_t in_data);

public:
    //SendSession SendMsgQueue[SendMsgNum];			//解码后的send msg存于此处
    Queue *send_queue;												//for send
    SDKFilter filter;													//for receive
    Serial_HardDriver *m_hd;

    ZigbeeAdapter();

    ~ZigbeeAdapter();

    void init(string device, unsigned int baudrate);

    void readPoll();
    void sendPoll();
};

static void* SerialThread(void* param)
{
    ZigbeeAdapter* p_adapter = (ZigbeeAdapter*)(param);

    while(true)
    {
        p_adapter->readPoll();
        p_adapter->sendPoll();
        //cout<<'!'<<endl;
        usleep(100000);
    }
}

extern ZigbeeAdapter *adapter;

#endif
