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
	bool operator>(const HeuristicNode& h2) const;
	bool operator<(const HeuristicNode& h2) const;
};

// Required for priority queue usage
bool HeuristicNode::operator>(const HeuristicNode& h2) const
{
	return this->f > h2.f;
}
bool HeuristicNode::operator<(const HeuristicNode& h2) const
{
	return this->f < h2.f;
}

// Seems to work correctly!
Stage2::Stage2(turtlePositionNode turtlePositionNodes[])
{
	cout << "1" << endl;
	shortestPathSize = 0;
	SeperateTargetAndVillainTurtles(turtlePositionNodes);
	cout << "2" << endl;
}

// Might work correctly (no errors on running)
void Stage2::InitializeHeuristicField(HeuristicNode heuristicField[][12])
{
	cout << "3" << endl;
	for (int x = 0; x < 12; x++)
	{
		for (int y = 0; y < 12; y++)
		{
			heuristicField[y][x].f = std::numeric_limits<double>::max();
			heuristicField[y][x].g = std::numeric_limits<double>::max();
			heuristicField[y][x].h = std::numeric_limits<double>::max();

			heuristicField[y][x].x = x;
			heuristicField[y][x].y = y;

			heuristicField[y][x].open = true;

			heuristicField[y][x].parent = NULL;
		}
	}
	cout << "4" << endl;
}

// The beef
int Stage2::FindMinimalPath(PathNode shortestPath[])
{
	cout << "5" << endl;
	double pathLength;

	turtlePositionNode	currentTargetTurtle;
	std::vector<turtlePositionNode>::iterator currentTargetIter;

	HeuristicNode heuristicField[12][12];
	HeuristicNode* rootOfCurrentPath;

	std::priority_queue<HeuristicNode, std::vector<HeuristicNode>, std::greater<HeuristicNode>> openQueue;
	std::vector<HeuristicNode> closedQueue;

	cout << "6" << endl;

	bool targetFound = false;

	// Prepare a table for heuristic values
	InitializeHeuristicField(heuristicField);
	rootOfCurrentPath = &heuristicField[0][0];
	rootOfCurrentPath->x = 0;
	rootOfCurrentPath->y = 0;
	rootOfCurrentPath->f = 0;
	rootOfCurrentPath->g = 0;

	// A vector of target turtles that have not been visited yet
	std::vector<turtlePositionNode> remainingTargets(targetTurtles.size());
	std::copy(targetTurtles.begin(), targetTurtles.end(), remainingTargets.begin());

	// Add the starting point to the open queue
	openQueue.push(heuristicField[0][0]);
	cout << "7" << endl;


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
			cout << "Current Target iter x and y are: " << currentTargetIter->x << " " << currentTargetIter->y << endl;
			ExecuteSingleSearch(openQueue.top().x, openQueue.top().y, openQueue.top(), *currentTargetIter, heuristicField, openQueue);
			if (TargetCloseEnoughForCapture(openQueue.top(), *currentTargetIter))
			{
				cout << "7.1" << endl;
				targetFound = true;
			}

			cout << "7.2" << endl;
			// Close the expanded node
			//closedQueue.push_back(openQueue.top());
			//openQueue.pop(); // This is done inside of ExecuteSingleSearchInstead

		} while (!targetFound && !openQueue.empty());

		cout << "7.3" << endl;
		// After success or failure in navigation, reset the
		// heuristic field, open/closed queues,
		// and remove target turtle from remaining targets
		if (!remainingTargets.empty())
		{
			cout << "7.4" << endl;
			cout << "Remaining targets size is: " << remainingTargets.size() << endl;
			// Remove just captured turtle from remaining targets
			remainingTargets.erase(currentTargetIter);
			cout << "7.45" << endl;
			targetFound = false;
			UpdateShortestPath(openQueue.top(), rootOfCurrentPath);
			cout << "7.5" << endl;

			// Reset our heuristical values
			InitializeHeuristicField(heuristicField);
		}
		cout << "7.6" << endl;
	}

	cout << "openQueue.size() is: " << openQueue.size() << endl;
	cout << "remainingTarget.size() is : " << remainingTargets.size() << endl;
	cout << "8" << endl;
	// If not all target turtles could be visited, then there is no path leading to all turtles.
	if (!remainingTargets.empty())
	{
		// A path traveling to all target turtles is impossible
		// Return a value indicating failure (maybe an empty queue of nodes to visit?)
		pathLength = 0;
	}

	cout << "9" << endl;
	cout << "Path length is: " << pathLength << endl;
	return pathLength;
}

// Probably works correctly
bool Stage2::MoveInBounds(const int X, const int Y)
{
	return X <= 119 && X >= 0 && Y <= 119 && Y >= 0;
}

bool Stage2::MoveSafe(HeuristicNode turtle1)
{
	cout << "10" << endl;
	std::vector<turtlePositionNode>::iterator villainIter = villainTurtles.begin();
	bool moveSafe = true;
	while (moveSafe && villainIter != villainTurtles.end())
	{
		if (VillainCloseEnoughForCapture(turtle1, *villainIter))
		{
			moveSafe = false;
		}
		else
		{
			villainIter++;
		}

	}
	cout << "11" << endl;
	return moveSafe;
}

bool Stage2::ExecuteSingleSearch(const int CURRENT_X,
	const int CURRENT_Y,
	HeuristicNode currentNode,
	turtlePositionNode currentTarget,
	HeuristicNode heuristicField[][12],
	std::priority_queue<HeuristicNode, std::vector<HeuristicNode>, std::greater<HeuristicNode>>& openQueue)
{
	openQueue.pop(); // Since the topmost node is being expanded, remove it from open queue
	cout << "12" << endl;
	// Look around the current node for a new node to visit
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			cout << "12.1" << endl;
			if ((y == x && y != 0) || y != x)
			{
				cout << "12.2" << endl;
				if (MoveInBounds(CURRENT_X + x, CURRENT_Y + y))
				{
					cout << "12.3" << endl;
					if (heuristicField[CURRENT_Y + y][CURRENT_X + x].open && MoveSafe(heuristicField[CURRENT_Y + y][CURRENT_X + x]))
					{
						cout << "12.4" << endl;
						// If diagonal move
						if (std::abs(x) == std::abs(y))
						{
							cout << "12.5" << endl;
							if (currentNode.g + 1.41421356237 < heuristicField[CURRENT_Y + y][CURRENT_X + x].g)
							{
								cout << "12.6" << endl;
								cout << "adding node to path" << endl;
								heuristicField[CURRENT_Y + y][CURRENT_X + x].g = currentNode.g + 1;
								heuristicField[CURRENT_Y + y][CURRENT_X + x].h = DistanceToTurtle(CURRENT_X, CURRENT_Y, currentTarget);
								heuristicField[CURRENT_Y + y][CURRENT_X + x].f = heuristicField[CURRENT_Y + y][CURRENT_X + x].g + heuristicField[CURRENT_Y + y][CURRENT_X + x].h;
								heuristicField[CURRENT_Y + y][CURRENT_X + x].parent = &heuristicField[CURRENT_Y][CURRENT_X];
								PushToOpenQueue(openQueue, heuristicField[CURRENT_Y + y][CURRENT_X + x]);
							}
							else
							{
								cout << (currentNode.g + 1.41421356237) << " is greater than " << heuristicField[CURRENT_Y + y][CURRENT_X + x].g << endl;
							}
						}
						// If non-diagonal move
						else
						{
							cout << "12.7" << endl;
							if (currentNode.g + 1 < heuristicField[CURRENT_Y + y][CURRENT_X + x].g)
							{
								cout << "12.8" << endl;
								cout << "adding node to path" << endl;
								heuristicField[CURRENT_Y + y][CURRENT_X + x].g = currentNode.g + 1;
								heuristicField[CURRENT_Y + y][CURRENT_X + x].h = DistanceToTurtle(CURRENT_X, CURRENT_Y, currentTarget);
								heuristicField[CURRENT_Y + y][CURRENT_X + x].f = heuristicField[CURRENT_Y + y][CURRENT_X + x].g + heuristicField[CURRENT_Y + y][CURRENT_X + x].h;
								heuristicField[CURRENT_Y + y][CURRENT_X + x].parent = &heuristicField[CURRENT_Y][CURRENT_X];
								PushToOpenQueue(openQueue, heuristicField[CURRENT_Y + y][CURRENT_X + x]);
							}
							else
							{
								cout << (currentNode.g + 1.41421356237) << " is greater than " << heuristicField[CURRENT_Y + y][CURRENT_X + x].g << endl;
							}
						}
					}
					else
					{
						cout << "Move to " << CURRENT_X + x << ", " << CURRENT_Y + y << "not safe" << endl;
						heuristicField[CURRENT_Y + y][CURRENT_X + x].open = false;
					}
				}
				else
				{
					cout << "Move to " << CURRENT_X + x << ", " << CURRENT_Y + y << "out of bounds" << endl;
				}
			}
		}
	}
	cout << "OpenQueueSize(): " << openQueue.size() << endl;
	cout << "13" << endl;
}

double Stage2::DistanceToTurtle(const int X,
	const int Y,
	turtlePositionNode otherTurtle)
{
	return sqrt(pow((double)X / 10 - otherTurtle.x, 2) + pow((double)Y / 10 - otherTurtle.y, 2));
}

// Seems to work correctly!
void Stage2::SeperateTargetAndVillainTurtles(turtlePositionNode turtlePositionNodes[])
{
	cout << "14" << endl;
	for (int x = 0; x < 7; x++)
	{
		switch (turtlePositionNodes[x].friendly)
		{
		case true:
			targetTurtles.push_back(turtlePositionNodes[x]);
			cout << "Pushed back target of name: " << targetTurtles.back().name << endl;
			break;
		case false:
			villainTurtles.push_back(turtlePositionNodes[x]);
			cout << "Pushed back target of name: " << villainTurtles.back().name << endl;
			break;
		default:
			// Error case
			break;
		}
	}
	cout << "There are " << targetTurtles.size() << " target turtles" << endl;
	cout << "There are " << villainTurtles.size() << " villain turtles" << endl;
	cout << "15" << endl;
}

// Should never be called if the number of remaining target turtles is 0.
turtlePositionNode Stage2::ChooseTargetTurtle(const int CURRENT_X,
	const int CURRENT_Y,
	std::vector<turtlePositionNode>& remainingTargets,
	std::vector<turtlePositionNode>::iterator& targetPos)
{
	double shortestLength = std::numeric_limits<double>::max();
	double straightLineDist;
	turtlePositionNode bestTarget;

	cout << "16" << endl;
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

	cout << "Target turtle chosen: " << bestTarget.name << endl;
	cout << "17" << endl;
	// Without error checking prior to the calling of this function,
	// this will return an unitialized value.
	return bestTarget;
}


void Stage2::UpdateShortestPath(HeuristicNode currentNode, HeuristicNode* rootOfCurrentPath)
{
	cout << 17.1 << endl;
	HeuristicNode* heuristicNodePtr = &currentNode;
	HeuristicNode reversedPathSegment[144];
	int reversedPathSegmentSize;
	int shortestPathSizeTemp = shortestPathSize;
	int x = 0;

	cout << "18" << endl;
	while (heuristicNodePtr->x != rootOfCurrentPath->x && heuristicNodePtr->y != rootOfCurrentPath->y)
	{
		reversedPathSegment[x].x = heuristicNodePtr->x;
		reversedPathSegment[x].y = heuristicNodePtr->y;
		heuristicNodePtr = heuristicNodePtr->parent;
		x++;
		cout << "18.01" << endl;
	}
	cout << "18.1" << endl;
	// Have to reverse the path segment we created
	for (int y = 0; y < x; y++)
	{
		shortestPath[shortestPathSizeTemp + y].x = reversedPathSegment[shortestPathSizeTemp + (x - y)].x;
		shortestPath[shortestPathSizeTemp + y].y = reversedPathSegment[shortestPathSizeTemp + (x - y)].y;
		//shortestPath[shortestPathSizeTemp + y].parent = shortestPath[shortestPathSizeTemp + y - 1].parent; // May not need parent in pathnode
	}

	cout << "19" << endl;
}

// This is not working properly (still probably the case)
bool Stage2::PushToOpenQueue(std::priority_queue<HeuristicNode, std::vector<HeuristicNode>, std::greater<HeuristicNode>>& openQueue,
	HeuristicNode node)
{
	cout << "20" << endl;
	cout << "OpenQueue.size(): " << openQueue.size() << endl;
	std::vector<HeuristicNode> temp;
	cout << "20.001" << endl;
	int x = 0;
	bool nodeFound = false;
	cout << "20.002" << endl;
	if (!openQueue.empty())
	{
		cout << "20.01" << endl;
		while (!openQueue.empty() && !nodeFound)
		{
			cout << "20.02" << endl;
			temp.push_back(openQueue.top());
			openQueue.pop();
			cout << "20.03" << endl;
			if (temp.back().x == node.x && temp.back().y == node.y)
			{
				cout << "20.1" << endl;
				cout << "NODE FOUND IN QUEUE: " << node.x << " " << node.y << endl;
				temp.back().f = node.f;
				temp.back().g = node.g;
				temp.back().h = node.h;
				temp.back().open = node.open;
				nodeFound = true;
			}
			//			else
			//			{
			//				cout << "20.2" << endl;
			//				cout << "NODE NOT FOUND IN QUEUE: " << node.x << " " << node.y << endl;
			//				temp.push_back(openQueue.top());
			//				openQueue.pop();
			//			}
		}
		cout << "21" << endl;
		while (!temp.empty())
		{
			cout << "21.1" << endl;
			openQueue.push(temp.back());
			temp.pop_back();
		}
	}
	else
	{
		openQueue.push(node);
	}
	cout << "22" << endl;
	return nodeFound;	// Returns if the node existed previously in the openqueue (not really used atm but whatever)
}

bool Stage2::TargetCloseEnoughForCapture(HeuristicNode turtle1, turtlePositionNode target)
{
	return (std::abs(turtle1.x - target.x) < std::abs(.05*target.x)) && (std::abs(turtle1.y - target.y) < std::abs(.05*target.y));
}

bool Stage2::VillainCloseEnoughForCapture(HeuristicNode turtle1, turtlePositionNode villain)
{
	return (std::abs(turtle1.x - villain.x) < std::abs(.1*villain.x)) && (std::abs(turtle1.y - villain.y) < std::abs(.1*villain.y));
}
