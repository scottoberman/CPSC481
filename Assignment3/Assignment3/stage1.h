#ifndef STAGE1_H_

#define STAGE1_H_

//#include <vector>
//#include <queue>

// Refrain from altering this struct as other stages depend on this.
struct turtlePositionNode
{
	double x;	 	// X coordinate
	double y;	 	// Y Coordinate
	bool friendly; 	//if target turtle = true else if villian turtle = false
};


void SpawnTurtles(turtlePositionNode* turtle, ros::NodeHandle& nh);
void KillTurtles(ros::NodeHandle& nh);

#endif