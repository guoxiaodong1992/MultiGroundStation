#ifndef _MSGCODE_H
#define _MSGCODE_H

#include "msg/GPS.h"
#include "msg/ShapeConfig.h"
#include "msg/Posi.h"
#include "msg/Attitude.h"


using namespace zigbee;

#define msgID_InitShake         'A'
#define msgID_InitShakeAck      'A'
#define msgID_LocalFrame        'B'
#define msgID_LocalFrameAck     'B'
#define msgID_ShapeConfig       'C'
#define msgID_ShapeConfigAck    'C'
#define msgID_TakeOff           'D'
#define msgID_TakeOffAck        'D'

#define msgID_Meet              'E'
#define msgID_MeetAck           'E'
#define msgID_Fly               'F'
#define msgID_FlyAck            'F'
#define msgID_Return            'G'
#define msgID_ReturnAck         'G'
#define msgID_Land              'H'
#define msgID_LandAck           'H'
#define msgID_Stop              'I'
#define msgID_StopAck           'I'

#define msgID_Posi              'U'
#define msgID_Vel               'V'
#define msgID_Attitude          'W'

 typedef struct MAP_ID
{
  unsigned char UAV_ID;
  uint16_t zigbeeID;
}MAP_ID;

        //短地址为0x143E，则此处应该填写0x3E14


struct SDKFilter
{
  int recvIndex;
  unsigned char recvBuf[1024];
};

struct msgHeader
{
  unsigned char head;      //0xFD
  unsigned char length;
  unsigned char targetID_1;
  unsigned char targetID_2;

  unsigned char msgID;
  unsigned char reserve1;
  unsigned char reserve2;
  unsigned char reserve3;
};

#endif


