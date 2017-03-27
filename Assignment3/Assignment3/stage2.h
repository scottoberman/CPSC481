#ifndef STAGE2_H_

#define STAGE2_H_

#include "stage1.h"

#include <functional>
#include <math.h>
#include <queue>
#include <vector>

// Used solely by Stage2
struct HeuristicNode;

// Refrain from altering this struct as other stages depend on this.
// Used to send path nodes to Stage3
struct PathNode
{
	float x;
	float y;
	PathNode* parent;
};

class Stage2
{
public:
	Stage2(	turtlePositionNode [],
			PathNode []);
	int  FindMinimalPath(PathNode []);

private:
	PathNode shortestPath[14400];
	int shortestPathSize;

	void InitializeHeuristicField(HeuristicNode heuristicField[][120]);
	std::vector<turtlePositionNode> villainTurtles;
	std::vector<turtlePositionNode> targetTurtles;
	turtlePositionNode ChooseTargetTurtle(	const int,
											const int,
											std::vector<turtlePositionNode>,
											std::vector<turtlePositionNode>::iterator&);
	void SeperateTargetAndVillainTurtles(turtlePositionNode []);
	bool MoveInBounds(	const int,
						const int);
	bool MoveSafe(	const int,
					const int);
	bool ExecuteSingleSearch(	const int,
								const int,
								HeuristicNode,
								turtlePositionNode,
								HeuristicNode [][120]);
	double DistanceToTurtle(const int,
							const int,
							turtlePositionNode);
	void UpdateShortestPath(HeuristicNode);
};


#endif