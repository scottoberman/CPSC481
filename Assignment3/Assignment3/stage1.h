#ifndef STAGE1_H_

#define STAGE1_H_

//#include <vector>
//#include <queue>
#include <ros/ros.h>
#include <string>

using std::string;

// Refrain from altering this struct as other stages depend on this.
struct turtlePositionNode
{
	int x;	  // X coordinate
	int y;	  // Y Coordinate
	int friendly; //if target turtle = true else if villian turtle = false
	string name;
};

void InitPosition(ros::NodeHandle& nh);
void SpawnTurtles(turtlePositionNode* turtle, ros::NodeHandle& nh);
double GetRandomCoordinate();
void KillTurtles(ros::NodeHandle& nh);

#endif
