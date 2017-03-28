#ifndef STAGE1_H_

#define STAGE1_H_

//#include <vector>
//#include <queue>
#include <ros/ros.h>

// Refrain from altering this struct as other stages depend on this.
struct turtlePositionNode
{
	float x;	 	// X coordinate
	float y;	 	// Y Coordinate
	bool friendly; 	//if target turtle = true else if villian turtle = false
};

void 	InitPosition(ros::NodeHandle& nh);
void 	SpawnTurtles(turtlePositionNode* turtle, ros::NodeHandle& nh);
double 	GetRandomCoordinate();
void	KillTurtle(std::string name, ros::NodeHandle& nh);
void 	KillAllTurtles(ros::NodeHandle& nh);

#endif



