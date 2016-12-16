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

using namespace zigbee;

class MyQuad
{
public:
    MyQuad();
    GPS global_position;
    Posi local_position;
    int status;
private:



};

#endif // MYQUAD_H
