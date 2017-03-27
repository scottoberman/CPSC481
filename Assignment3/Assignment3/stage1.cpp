#include "stage1.h"
#include <cstdlib>      //for rand()
#include <cstring>      //for turtle names
#include <ros/ros.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Kill.h>


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
void SpawnTurtles(turtlePositionNode* turtle, ros::NodeHandle & nh)
{
	// Add all turtles to the turtlePosition stucture
	// (Their x, y, and role) (see stage1.h)
    
    	//create a client object for the spawn service. This needs to know the
    	// data type of the service and its name.
    	ros::ServiceClient spawnClient = nh.serviceClient<turtlesim::Spawn("spawn");
    
    	//create the request and response objects
    	turtlesim::Spawn::Request requestSpawn;
    	turtlesim::Spawn::Response responseSpawn;
    
    	//need to get random coordinates for turtles to spawn
    	srand((unsigned) time(0));
    
   	 for (int i = 0; i < 8; ++i)
    	{
       		 //create random coordinates larger than 2.0 and less than 11
        		// this says rand() % (maxNum - minNum) so if
        		// rand() % 10 will give numbers from [0-9] so add min back
        		// to get 2.0 + [0-9] = [2.0 - 11.0]
        		requestSpawn.x = 2.0 + static_cast<float>((rand() % (12.0 - 2.0)));
        		requestSpawn.y = 2.0 + static_cast<float>((rand() % (12.0 - 2.0)));
        		requestSpawn.theta = -90;   //have turtles facing bottom of screen
        
        		//store the turtles spawning position in our own data structure
        		turtle[i].x = requestSpawn.x;
        		turtle[i].y = requestSpawn.y;
        
        		//give the turtles a name... target turtles (T1, T2, T3)
       		 //                           enemy turtles  (X1, X2, X3)
        		if (i < 3 )
        		{
          			std::string name = "T" + std::to_string(i);
            			requestSpawn.name = name;
            
           			 //store the turtles role FRIENDLY OR TARGET TURTLE (true)
            			turtle[i].friendly = true;
        		}
        		else
        		{
            			std::string name = "X" + std::to_string(i % 3); // want 1, 2, 3 not 4, 5, 6 as turtle name
           			 requestSpawn.name = name;
            
           			 //store the turtles role ENEMY OR VILLIAN TURTLE (false)
           			 turtle[i].friendly = false;
        		}
    
       		//attempt to spawn the turtle
        		bool successfulSpawn = spawnClient.call(requestSpawn, responseSpawn);
        
        		//check if it was able to spawn the turtle
        		if (successfulSpawn)
        		{
         			ROS_INFO_STREAM("Spawned a turtle named " << responseSpawn.name);
        		}
        		else
       		{
            			ROS_ERROR_STREAM("Failed to spawn " << responseSpawn.name);
        		}
   	}
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
turtlesim::Kill:Request requestKill;
   	 turtlesim::Kill::Response responseKill;
   	 ros::ServiceClient killClient = nh.serviceClient<turtlesim::Kill>("kill");
    
   	 for (int i = 0; i < 8; ++i)
   	 {
       		if (i < 3)
        		{
            			std::string name = "T" + std::to_string(i);
            			requestKill.name = name;
        		}
        		else
        		{
            			std::string name = "X" + std::to_string(i);
            			requestKill.name = name;
        		}
        
        		bool successfulKill = killClient.call(requestKill, ResponseKill);
        
       	 	if (successfulKill)
       		{
            			ROS_INFO_STREAM("Killed a turtle named " << responseKill.name);
        		}
        		else
        		{
            			ROS_ERROR_STREAM("Failed to kill " << responseKill.name);
        		}
    	}
}






