#include "assignment3.h"



// ATTENTION: Most code that is in your respective stage can be
//			  changed freely without breaking functionality
//			  (hopefully). The little code that there is in
//			  the Stage1 and Stage3 files is basically
//			  just a template. However, do not change
//			  the structs that are used for transferring
//			  data between stages (ie Stage1Node and PathNode
//			  and their respective vectors and queues).
//
//			  Since each portion is being worked on at the
//			  same time, you may have to come up with your
//			  own test data and test it with your
//			  function/class, and comment out everyone elses
//			  code.
//
//			  ROS code has not been included in this so far.
//			  Stage1 and Stage2 will have to make use of
//			  ROS.


int main()
{
	std::vector<Stage1Node> stage1Nodes;
	std::queue<PathNode> shortestPath;


	// Generate field of turtles and put all of the turtles/
	// x, y coordinates and role in each node (see stage1.h)
	Stage1(stage1Nodes);

	// Find shortest path.
	// Stage2 is a class. ATM Stage1 and Stage3 are simply functions
	// but change those as you see fit.
	Stage2 stage2(stage1Nodes);
	shortestPath = stage2.FindMinimalPath();

	// Traverse "shortest path"
	// (See stage2.h for the PathNode struct)
	Stage3(shortestPath);

	return 0;
}