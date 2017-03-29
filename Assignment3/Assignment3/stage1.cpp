#include "stage1.h"
#include <cstdlib>      //for rand()
#include <string>      //for turtle names
#include <ros/ros.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Kill.h>



//=============================================================================
// Description: This function will place 'turtle1' (main turtle that moves)
//		at the coordinates (0,0). It will do this by first killing
//		'turtle1' and then respawning it at the correct spot (0,0).
//
// Input: ros::NodeHandle& nh - a nodeHandle to interact with the ROS system
//
// Output: NONE
//
//=============================================================================
void InitPosition(ros::NodeHandle& nh)
{
	//kill 'turtle1'
	turtlesim::Kill::Request  requestKill;
	turtlesim::Kill::Response responseKill;

	ros::ServiceClient killClient = nh.serviceClient<turtlesim::Kill>("kill");
	requestKill.name = "turtle1";

	bool successfulKill = killClient.call(requestKill, responseKill);

	//for debugging
	if (successfulKill)
	{
		ROS_INFO_STREAM("Killed a turtle named " << requestKill.name);
	}
	else
	{
		ROS_ERROR_STREAM("Failed to kill " << requestKill.name);
	}

	//now to spawn 'turtle1'
	turtlesim::Spawn::Request  requestSpawn;
	turtlesim::Spawn::Response responseSpawn;

	ros::ServiceClient spawnClient = nh.serviceClient<turtlesim::Spawn>("spawn");

	//spawn at (0,0)
	requestSpawn.x = 0.0;
	requestSpawn.y = 0.0;
	requestSpawn.theta = 45;	//have him spawn at 45 degrees counter clock wise
	requestSpawn.name = "turtle1";

	bool successfulSpawn = spawnClient.call(requestSpawn, responseSpawn);

	if (successfulSpawn)
	{
		ROS_INFO_STREAM("Spawned a turtle named " << responseSpawn.name << " at (" << requestSpawn.x << ", " << requestSpawn.y << ")");
	}
	else
	{
		ROS_ERROR_STREAM("Failed to spawn " << responseSpawn.name);
	}
}


//==============================================================================
// Description: This function will randomly spawn the target turtles and villian
//              turtles from points (2.0, 2.0) to (11.0, 11.0). It will store
//              these random coordinates in the turtlePositionNode structure
//              passed as an argument. It is assumed that a nodeHandle data
//              type has also been declared and passed as the second argument.
//
// Input:   turtlePositionNode * turtle - a pointer to the structure that will
//                                        store the spawned target and villian
//                                        turtles along with their loyalty
//                                        (friendly or enemy)
//          ros::NodeHandle & nh - a nodeHandle to interact with the ROS system
//
// Output: NONE
//
//==============================================================================
void SpawnTurtles(turtlePositionNode* turtle, ros::NodeHandle& nh)
{
	// Add all turtles to the turtlePosition stucture
	// (Their x, y, and role) (see stage1.h)

	//create a client object for the spawn service. This needs to know the
	// data type of the service and its name.
	ros::ServiceClient spawnClient = nh.serviceClient<turtlesim::Spawn>("spawn");

	//create the request and response objects
	turtlesim::Spawn::Request requestSpawn;
	turtlesim::Spawn::Response responseSpawn;

	//need to get random coordinates for turtles to spawn
	srand((unsigned)time(0));

	for (int i = 0; i < 7; ++i)
	{
		requestSpawn.x = GetRandomCoordinate();
		requestSpawn.y = GetRandomCoordinate();
		requestSpawn.theta = -90;   //have turtles facing bottom of screen

									//store the turtles spawning position in our own data structure
		turtle[i].x = requestSpawn.x;
		turtle[i].y = requestSpawn.y;


		//give the turtles a name... target turtles (T1, T2, T3)
		//                           enemy turtles  (X1, X2, X3, X4)
		if ((i + 1) < 4)
		{
			std::string name = "T" + std::to_string(i + 1);
			requestSpawn.name = name;
			turtle[i].name = name;

			//store the turtles role FRIENDLY OR TARGET TURTLE (true)
			turtle[i].friendly = true;
		}
		else
		{
			std::string name = "X" + std::to_string(((i + 1) % 4) + 1);
			// want 1, 2, 3 not 4, 5, 6 as turtle name
			requestSpawn.name = name;
			turtle[i].name = name;

			//store the turtles role ENEMY OR VILLIAN TURTLE (false)
			turtle[i].friendly = false;
		}

		//attempt to spawn the turtle
		bool successfulSpawn = spawnClient.call(requestSpawn, responseSpawn);

		//check if it was able to spawn the turtle
		if (successfulSpawn)
		{
			ROS_INFO_STREAM("Spawned a turtle named " << responseSpawn.name);
			ROS_INFO_STREAM(" at (" << requestSpawn.x << ", " << requestSpawn.y << " )");
		}
		else
		{
			ROS_ERROR_STREAM("Failed to spawn " << responseSpawn.name);
		}
	}

}



//=============================================================================
// Description: This function will produce a random number between the MIN and
//		MAX range. The MIN number is 2.0 and the MAX is 11.0. This 
//		function will return a number between (2.0 and 11.0]. 
//
// Input:	NONE
//
// Output: 	NONE
//
//=============================================================================
double GetRandomCoordinate()
{
	//create random coordinates larger than 2.0 and less than 11
	// this says rand() % (maxNum - minNum) so if
	// rand() % 9 will give numbers from [0-8] so divide by MAX to get
	// decimal place and then add min and randomNum back to get 
	// randomNum between [0 - 8] + 2.0 + [decimal between [2.0-10/MAX] = [2.0 - 11.0]
	//		(rnad() % (MAX - MIN) + MIN
	int randomNum = (rand() % static_cast<int>(11.0 - 2.0)) + 2.0;	//range 2.0 - 10.0
	int decimalNum = (rand() % 11);	//range 0-10
									//		 randomNum + (decimalNum / MAX);
	double coordinate = randomNum + (decimalNum / 11.0);

	return coordinate;
}



//==============================================================================
// Description: This function will kill every target and villian turtle that
//              was spawned at the beginning of the program. It will NOT kill
//              turtle1 that can move.
//
// Input:   ros::NodeHandle & nh - a nodeHandle to interact with the ROS system
//
// Output: NONE
//
//==============================================================================
void KillTurtles(ros::NodeHandle & nh)
{
	turtlesim::Kill::Request requestKill;
	turtlesim::Kill::Response responseKill;
	ros::ServiceClient killClient = nh.serviceClient<turtlesim::Kill>("kill");

	for (int i = 0; i < 7; ++i)
	{
		if ((i + 1) < 4)
		{
			std::string name = "T" + std::to_string(i + 1);
			requestKill.name = name;
		}
		else
		{
			std::string name = "X" + std::to_string(((i + 1) % 4) + 1);
			requestKill.name = name;
		}

		bool successfulKill = killClient.call(requestKill, responseKill);

		if (successfulKill)
		{
			ROS_INFO_STREAM("Killed a turtle named " << requestKill.name);
		}
		else
		{
			ROS_ERROR_STREAM("Failed to kill " << requestKill.name);
		}
	}
}



