//#include "assignment3.h"
#include "stage1.h"
#include "stage2.h"
#include "ros/ros.h"                //used for basic ROS functionality
#include "geometry_msgs/Twist.h"    //allows us to move/turn turtle
#include "turtlesim/Pose.h"         //can only inform us of the turtle's
// position and heading


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

//GLOBAL VARIABLES
ros::Publisher geoMsgPublish;
turtlesim::Pose turtlesim_pose;
const double PI = 3.14159265359;


void newPoseMsg(const turtlesim::Pose::ConstPtr& poseMsg);


int main(int argc, char** argv)
{

	//initialize the ROS client library with the name assignment3
	ros::init(argc, argv, "assignment3");

	//create a node hangle to interact with the ROS system
	ros::NodeHandle nh;

	//set up the publisher messenger that allows the default turtle to move
	geoMsgPublish = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 100);

	//create a subscriber object to receive the default turtles position
	ros::Subscriber poseSub = nh.subscribe("turtle1/pose", 100, newPoseMsg);

	//stores all the turtles positions except default turtle
	turtlePositionNode turtlePosition[7];

	//stores the shortestPath to all target turtles
	PathNode shortestPath[144];

	int shortestPathLength;

	//initialize turtle1's starting position to (0,0)
	InitPosition(nh);

	// Generate field of turtles and put all of the turtles/
	// x, y coordinates and role in each node (see stage1.h)
	SpawnTurtles(turtlePosition, nh);

	//TESTING STOPS HERE FOR STAGE1
	// Find shortest path.
	// Stage2 is a class. ATM Stage1 and Stage3 are simply functions
	// but change those as you see fit.
	Stage2 stage2(turtlePosition);
	shortestPathLength = stage2.FindMinimalPath(shortestPath);

	// Traverse "shortest path"
	// (See stage2.h for the PathNode struct)
	//	Stage3(shortestPath);

	return 0;
}


void newPoseMsg(const turtlesim::Pose::ConstPtr& poseMsg)
{
	turtlesim_pose.x = poseMsg->x;
	turtlesim_pose.y = poseMsg->y;
	turtlesim_pose.theta = poseMsg->theta;

	//UNCOMMENT THIS FOR DEBUGGING PURPOSES TO TRACK TURTLES MOVEMENTS
	//    ROS_INFO_STREAM(std::setprecision(2) << std::fixed
	//                    << "position = ("    << poseMsg->x  << ", " << poseMsg->y
	//                    << ") direction = "  << poseMsg->theta);
}


