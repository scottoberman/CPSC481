#ifndef STAGE2_H_

#define STAGE2_H_

#include "stage1.h"

#include <functional>
#include <math.h>
#include <queue>
#include <vector>

// Used soley by Stage2
struct HeuristicNode;

// Refrain from altering this struct as other stages depend on this.
// Used to send path nodes to Stage3
struct PathNode
{
	int x;
	int y;
};

class Stage2
{
public:
	Stage2(TurtlePositionNode [], PathNode []);
	int  FindMinimalPath(PathNode []);

private:
	void InitializeHeuristicField(HeuristicNode heuristicField[][12]);
	std::vector<TurtlePositionNode> villainTurtles;
	std::vector<TurtlePositionNode> targetTurtles;
	TurtlePositionNode ChooseTargetTurtle(const int, const int, std::vector<TurtlePositionNode>);
	void SeperateTargetAndVillainTurtles(TurtlePositionNode []);
};


#endif




