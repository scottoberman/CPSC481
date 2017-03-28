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

	std::string turtleCapturedHere; // Name of a target turtle which
									// is being captured at this node.
									// If the string is empty, then no
									// turtle is being captured at this
									// node.
	//PathNode* parent;

};

class Stage2
{
public:
	Stage2(	turtlePositionNode [],
			PathNode []);
	int  FindMinimalPath(PathNode []);

private:
	PathNode shortestPath[144];
	int shortestPathSize;
	double shortestPathDistance;

	void InitializeHeuristicField(HeuristicNode heuristicField[][12]);
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
								HeuristicNode [][12],
								std::priority_queue<HeuristicNode, std::vector<HeuristicNode>, std::greater<HeuristicNode>>);
	double DistanceToTurtle(const int,
							const int,
							turtlePositionNode);
	void UpdateShortestPath(HeuristicNode);
	bool PushToOpenQueue(	std::priority_queue<HeuristicNode, std::vector<HeuristicNode>, std::greater<HeuristicNode>>,
									HeuristicNode);
};


#endif