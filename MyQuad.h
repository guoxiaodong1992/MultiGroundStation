/** @file MyQuad.h
 *  @version 1.0.0
 *  @date Sep 15th, 2016
 *  @author Guo Xiaodong
 *  @brief
 *  Quad Class. Header File.
 */

#ifndef MYQUAD_H
#define MYQUAD_H

#include <sensor_msgs/NavSatFix.h>
#include <geometry_msgs/PoseStamped.h>
#include "msg/GPS.h"
#include "msg/ShapeConfig.h"
#include "msg/Posi.h"
#include "msg/Attitude.h"
#include "MyDataProcess.h"

using namespace zigbee;

enum QuadStatus{
    INIT_SHAKE=0,
    BODY_FRAME,
    TAKE_OFF,
    MEET,
    CRUISE,
    MISSION,
    RETURN,
    LANDIG,
    LANDED
};

class MyQuad
{
public:
    MyQuad();
    u16 zigbeeID;
    GPS global_position;
    Posi local_position;
    Posi local_vel;
    Attitude atti;
    int uavID;
    QuadStatus quadStatus;

private:



};

#endif // MYQUAD_H
