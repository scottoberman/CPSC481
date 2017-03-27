#include "stage2.h"




// Pretty confusing yeah!
// Need to debug like fu-




struct HeuristicNode
{
double f;
double g;
double h;

int x;
int y;

bool open;

HeuristicNode* parent;

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

Stage2::Stage2(turtlePositionNode turtlePositionNodes[], PathNode shortestPath[])
{
	shortestPathSize = 0;
	SeperateTargetAndVillainTurtles(turtlePositionNodes);
}

void Stage2::InitializeHeuristicField(HeuristicNode heuristicField[][120])
{
	for (int x = 0; x < 120; x++)
	{
		for (int y = 0; y < 120; y++)
		{
			heuristicField[y][x].f		= std::numeric_limits<double>::max();
			heuristicField[y][x].g		= std::numeric_limits<double>::max();
			heuristicField[y][x].h		= std::numeric_limits<double>::max();

			heuristicField[y][x].x = x;
			heuristicField[y][x].y = y;

			heuristicField[y][x].open	= true;

			heuristicField[y][x].parent = NULL;
		}
	}
}

// The beef
int Stage2::FindMinimalPath(PathNode shortestPath[])
{
	double pathLength;

	turtlePositionNode	currentTargetTurtle;
	std::vector<turtlePositionNode>::iterator currentTargetIter;

	HeuristicNode heuristicField[120][120];

	std::priority_queue<HeuristicNode, std::vector<HeuristicNode>, std::greater<HeuristicNode>> openQueue;
	std::vector<HeuristicNode> closedQueue;

	bool targetFound = false;

	// Prepare a table for heuristic values
	InitializeHeuristicField(heuristicField);

	// A vector of target turtles that have not been visited yet
	std::vector<turtlePositionNode> remainingTargets(targetTurtles.size());
	std::copy(targetTurtles.begin(), targetTurtles.end(), remainingTargets.begin());

	// Add the starting point to the open queue
	openQueue.push(heuristicField[0][0]);


	// Start is always at (0, 0)
	// Move in a clockwise fashion calculating heustic function
	while (!openQueue.empty() && !remainingTargets.empty()) // This condition may be subject to alteration
	{
		// Choose target turtle
		currentTargetTurtle = ChooseTargetTurtle(openQueue.top().x, openQueue.top().y, remainingTargets, currentTargetIter);
		
		// Attempt to navigate to target turtle
		// Manage open/closed queues
		// Add nodes to some potential path
		// TODO TODO
		do
		{
			ExecuteSingleSearch(openQueue.top().x, openQueue.top().y, openQueue.top(), *currentTargetIter, heuristicField);
			if (DistanceToTurtle(openQueue.top().x, openQueue.top().y, *currentTargetIter))
			{
				targetFound = true;
			}
		} while(!targetFound && !openQueue.empty());

		// After success or failure in navigation, reset the
		// heuristic field, open/closed queues,
		// and remove target turtle from remaining targets
		if (!remainingTargets.empty())
		{
			remainingTargets.erase(currentTargetIter);
			targetFound = false;
			UpdateShortestPath(openQueue.top());
		}
	}

	// If not all target turtles could be visited, then there is no path leading to all turtles.
	if (!remainingTargets.empty())
	{
		// A path traveling to all target turtles is impossible
		// Return a value indicating failure (maybe an empty queue of nodes to visit?)
		pathLength = 0;
	}


	return pathLength;
}

bool Stage2::MoveInBounds(const int X, const int Y)
{
	return X <= 119 && X >= 0 && Y <= 119 && Y >= 0;
}

bool Stage2::MoveSafe(const int X, const int Y)
{
	std::vector<turtlePositionNode>::iterator villainIter = villainTurtles.begin();
	bool moveSafe = true;
	while (moveSafe && villainIter != villainTurtles.end())
	{
		if (DistanceToTurtle(X, Y, *villainIter) < (villainIter->x*.1 + villainIter->y*.1))
		{
			moveSafe = false;
		}
		else
		{
			villainIter++;
		}
		
	}

	return moveSafe;
}

bool Stage2::ExecuteSingleSearch(	const int CURRENT_X,
									const int CURRENT_Y,
									HeuristicNode currentNode,
									turtlePositionNode currentTarget,
									HeuristicNode heuristicField[][120])
{
	// Look around the current node for a new node to visit
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			if (y != 0 && x != 0)
			{
				if (heuristicField[CURRENT_Y + y][CURRENT_X + x].open && MoveSafe(CURRENT_X + x, CURRENT_Y + y))
				{
					if (MoveInBounds(CURRENT_X + x, CURRENT_Y + y))
					{
						if (currentNode.g + .1 < heuristicField[y][x].g) // Only adding .1 since the heuristic field is 10 times the resolution of the turtle table (120 by 120 vs 12 by 12)
						{
							heuristicField[CURRENT_Y + y][CURRENT_X + x].g = currentNode.g + .1;
							heuristicField[CURRENT_Y + y][CURRENT_X + x].h = DistanceToTurtle(CURRENT_X, CURRENT_Y, currentTarget);
							heuristicField[CURRENT_Y + y][CURRENT_X + x].f = heuristicField[CURRENT_Y + y][CURRENT_X + x].g + heuristicField[CURRENT_Y + y][CURRENT_X + x].h;
							heuristicField[CURRENT_Y + y][CURRENT_X + x].parent = &heuristicField[CURRENT_Y][CURRENT_X];
						}
					}
				}
				else
				{
					heuristicField[CURRENT_Y + y][CURRENT_X + x].open = false;
				}
				
			}
		}
	}

}

double Stage2::DistanceToTurtle(const int X,
								const int Y,
								turtlePositionNode otherTurtle)
{
	return sqrt(pow((double)X / 10 - otherTurtle.x, 2) + pow((double)Y / 10 - otherTurtle.y, 2));
}

void Stage2::SeperateTargetAndVillainTurtles(turtlePositionNode turtlePositionNodes[])
{
	for (int x = 0; x < 7; x++)
	{
		switch (turtlePositionNodes[x].friendly)
		{
		case true:
			targetTurtles.push_back(turtlePositionNodes[x]);
			break;
		case false:
			villainTurtles.push_back(turtlePositionNodes[x]);
			break;
		default:
			// Error case
		}
	}
}

// Should never be called if the number of remaining target turtles is 0.
turtlePositionNode Stage2::ChooseTargetTurtle(	const int CURRENT_X,
												const int CURRENT_Y,
												std::vector<turtlePositionNode> remainingTargets,
												std::vector<turtlePositionNode>::iterator& targetPos)
{
	double shortestLength = std::numeric_limits<double>::max();
	double straightLineDist;
	turtlePositionNode bestTarget;
	

	for (std::vector<turtlePositionNode>::iterator targetsIter = remainingTargets.begin();
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
			targetPos = targetsIter;
		}
	}

	// Without error checking prior to the calling of this function,
	// this will return an unitialized value.
	return bestTarget;
}


void Stage2::UpdateShortestPath(HeuristicNode currentNode)
{
	HeuristicNode* heuristicNodePtr = &currentNode;
	HeuristicNode reversedPathSegment[14400];
	int reversedPathSegmentSize;
	int shortestPathSizeTemp = shortestPathSize;
	int x = 0;

	while (heuristicNodePtr != NULL)
	{
		reversedPathSegment[x] = *heuristicNodePtr;
		x++;
	}

	// Have to reverse the path segment we created
	for (int y = 0; y < x; y++)
	{
		shortestPath[shortestPathSizeTemp + y].x = reversedPathSegment[shortestPathSizeTemp + (x - y)].x;
		shortestPath[shortestPathSizeTemp + y].y = reversedPathSegment[shortestPathSizeTemp + (x - y)].y;
		shortestPath[shortestPathSizeTemp + y].parent = shortestPath[shortestPathSizeTemp + y - 1].parent; // May not need parent in pathnode
	}
}