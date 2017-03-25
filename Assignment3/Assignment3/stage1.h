#ifndef STAGE1_H_

#define STAGE1_H_

#include <vector>
#include <queue>

// Refrain from altering this struct as other stages depend on this.
struct Stage1Node
{
	int x;	  // X coordinate
	int y;	  // Y Coordinate
	int role; // 0 = moveable, 1 = target, 2 = villain, 3 = empty
};

void Stage1(std::vector<Stage1Node>& stage1Nodes);


#endif
