#include "stage2.h"
struct HeuristicNode
{
double f;
double g;
double h;
bool open;

// Required for priority queue usage
bool operator>(const HeuristicNode& h2);
bool operator<(const HeuristicNode& h2);
};

// Required for priority queue usage
bool HeuristicNode::operator>(const HeuristicNode& h2)
{
	return this->f > h2.f;
}
bool HeuristicNode::operator<(const HeuristicNode& h2)
{
	return this->f < h2.f;
}

Stage2::Stage2(std::vector<Stage1Node> stage1Nodes)
{
	SeperateTargetAndVillainTurtles(stage1Nodes);

	FindMinimalPath();
}

void Stage2::InitializeHeuristicField(HeuristicNode heuristicField[][110])
{
	for (int x = 0; x < 110; x++)
	{
		for (int y = 0; y < 110; y++)
		{
			heuristicField[y][x].f		= std::numeric_limits<double>::max();
			heuristicField[y][x].g		= std::numeric_limits<double>::max();
			heuristicField[y][x].h		= std::numeric_limits<double>::max();
			heuristicField[y][x].open	= true;
		}
	}
}

// The meat
std::queue<PathNode> Stage2::FindMinimalPath()
{
	int			currentX;
	int			currentY;
	Stage1Node	currentTargetTurtle;
	HeuristicNode heuristicField[110][110];

	std::queue<PathNode> path;

	std::priority_queue<HeuristicNode, std::vector<HeuristicNode>, std::greater<HeuristicNode>> openQueue;
	std::vector<HeuristicNode> closedQueue;

	// Prepare a table for heuristic values
	InitializeHeuristicField(heuristicField);

	// A vector of target turtles that have not been visited yet
	std::vector<Stage1Node> remainingTargets(targetTurtles.size());
	std::copy(targetTurtles.begin(), targetTurtles.end(), remainingTargets.begin());

	// Start is always at (0, 0)
	// Move in a clockwise fashion calculating heustic function
	while (heuristicField[0][0].open && !remainingTargets.empty()) // This condition may be subject to alteration
	{
		// Choose target turtle
		currentTargetTurtle = ChooseTargetTurtle(currentX, currentY, remainingTargets);
		
		// Attempt to navigate to target turtle
		// Manage open/closed queues
		// Add nodes to some potential path
		// TODO TODO

		// After successful navigation or failure, reset the
		// heuristic field, open/closed queues,
		// and remove target turtle from remaining targets
		// TODO TODO
	}

	// If not all target turtles could be visited, then there is no path leading to all turtles.
	if (!remainingTargets.empty())
	{
		// A path traveling to all target turtles is impossible
		// Return a value indicating failure (maybe an empty queue of nodes to visit?)
	}


	return path;
}

void Stage2::SeperateTargetAndVillainTurtles(std::vector<Stage1Node> stage1Nodes)
{
	for (std::vector<Stage1Node>::const_iterator stage1Iter = stage1Nodes.begin();
		 stage1Iter != stage1Nodes.end();
		 stage1Iter++)
	{
		switch (stage1Iter->role)
		{
		case 0:
			// Not technically needed to know start node since its always (0,0)
			// So do nothing
			break;
		case 1:
			targetTurtles.push_back(*stage1Iter);
			break;
		case 2:
			villainTurtles.push_back(*stage1Iter);
			break;
		default:
			// Error case
		}
	}
}

// Should never be called if the number of remaining target turtles is 0.
Stage1Node Stage2::ChooseTargetTurtle(const int CURRENT_X, const int CURRENT_Y, std::vector<Stage1Node> remainingTargets)
{
	double shortestLength = std::numeric_limits<double>::max();
	double straightLineDist;
	Stage1Node bestTarget;
	

	for (std::vector<Stage1Node>::const_iterator targetsIter = remainingTargets.begin();
		 targetsIter != remainingTargets.end();
		 targetsIter++)
	{
		// If the straight line distance between the current position
		// and another target is smallest so far, select it as potential
		// next target (prone to being wrong much of the time).
		straightLineDist = sqrt(pow(CURRENT_X - targetsIter->x, 2) + pow(CURRENT_Y - targetsIter->y, 2));
		if (straightLineDist < shortestLength)
		{
			shortestLength = straightLineDist;
			bestTarget = *targetsIter;
		}
	}

	// Without error checking prior to the calling of this function,
	// this will return an unitialized value.
	return bestTarget;
}