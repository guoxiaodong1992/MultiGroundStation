#include "Queue.h"
#include <cstring>

Queue::Queue(unsigned int size)
{
	this->size = size;
	Data = new unsigned char[size];
}

Queue::~Queue(void)
{
	delete []Data;
	Data = NULL;
}

/*
*********************************************************************************************************
* Function    : SetEmpty
* 
* Description : Çå¿Õ¶ÓÁÐ
*
* Arguments   :
*
* Returns     : 
*********************************************************************************************************
*/
void Queue::SetEmpty()
{
    this->front = 0;        
    this->rear = 0;
}
/*
*********************************************************************************************************
* Function    : EnQueue
* 
* Description : Èë¶Óº¯Êý
*
* Arguments   :
*
* Returns     : 
*********************************************************************************************************
*/
void Queue::EnQueue(unsigned char Data)
{
    if (IsFull())
    {
        return;
    }
	this->Data[rear] = Data;
    rear = (rear + 1) % size;
}
/*
*********************************************************************************************************
* Function    : DeQueue
*
* Description : ³ö¶Óº¯Êý
*
* Arguments   :
*
* Returns     : 
*********************************************************************************************************
*/
void Queue::DeQueue(unsigned char * data)
{
    if (IsEmpty())
    {
        return;
    }
		*data = Data[front];
//    memcpy(Data, &queue->Data[queue->front], sizeof(unsigned char));
    front = (front + 1) % size;
}
/*
*********************************************************************************************************
* Function    : GetQueueLength
*
* Description : »ñÈ¡¶ÓÁÐ³€¶È
*
* Arguments   :
*
* Returns     : 
*********************************************************************************************************
*/
int Queue::GetQueueLength()
{
    if(rear > front) 
        return (rear - front);
    else if (rear < front) 
        return (size - (front - rear) + 1);
    else
        return 0;

} 

/*
*********************************************************************************************************
* Function    : RealignQueue
*
* Description : ÖØÐÂÅÅÁÐ¶ÓÁÐ
*
* Arguments   :
*
* Returns     : 
*********************************************************************************************************
*/
int Queue::RealignQueue()
{
	unsigned char *temp = new unsigned char[size];
	int i,temp1;
	if(front>rear)
	{
		memcpy(temp,Data,(rear)*sizeof(unsigned char));
		temp1 = size - front+1;
		for(i=0;i<temp1;i++)
			Data[i] = Data[i+front];
		memcpy(Data + temp1,temp,(rear)*sizeof(unsigned char));
		rear = temp1 + rear;
		front = 0;
	}
	delete temp;
	temp = 0;
	return 0;

} 
