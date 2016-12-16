#ifndef _MSGCODE_H
#define _MSGCODE_H

#include "msg/GPS.h"
#include "msg/ShapeConfig.h"
#include "msg/Posi.h"

using namespace zigbee;

#define msgID_InitShake         'A'
#define msgID_InitShakeAck      'A'
#define msgID_LocalFrame        'B'
#define msgID_LocalFrameAck     'B'
#define msgID_ShapeConfig       'C'
#define msgID_ShapeConfigAck    'C'
#define msgID_TakeOff           'D'
#define msgID_TakeOffAck        'D'

typedef struct MsgHeader
{
  unsigned char head;      //0xFD
  unsigned char length;
  unsigned char targetID_1;
  unsigned char targetID_2;

  unsigned char msgID;
  unsigned char reserve1;
  unsigned char reserve2;
  unsigned char reserve3;
}MsgHeader;

int encode_InitShake(GPS &Tmp, unsigned char *a);

int encode_CmdAck(unsigned char msgID,unsigned char targetID,unsigned char *a);

void decode_InitShake(GPS &Tmp,unsigned char *a);
void decode_LocalFrame(GPS &Tmp,unsigned char *a);
void decode_ShapeConfig(ShapeConfig tmp,unsigned char *a);
void decode_TakeOff(Posi tmp,unsigned char *a);

unsigned char CountSum(unsigned char *a,int length);


#endif
