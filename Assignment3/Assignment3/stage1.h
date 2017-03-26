#ifndef STAGE1_H_

#define STAGE1_H_

#include <vector>
#include <queue>

// Refrain from altering this struct as other stages depend on this.
struct TurtlePositionNode
{
	int x;	  // X coordinate
	int y;	  // Y Coordinate
	int role; // 1 = target, 2 = villain
};

void Stage1(TurtlePositionNode turtlePositionNodes[]);


#endif
