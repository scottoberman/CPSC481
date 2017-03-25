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
	Stage2(std::vector<Stage1Node>);
	std::queue<PathNode>  FindMinimalPath();
private:
	void InitializeHeuristicField(HeuristicNode heuristicField[][110]);
	std::vector<Stage1Node> villainTurtles;
	std::vector<Stage1Node> targetTurtles;
	Stage1Node ChooseTargetTurtle(const int, const int, std::vector<Stage1Node>);
	void SeperateTargetAndVillainTurtles(std::vector<Stage1Node>);
};


#endif




