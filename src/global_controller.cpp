/*
 * Global controller to follow a trajectory
 *
 * By: Christopher Dunkers, cmdunkers@cmu.edu
 * March 1, 2015
 */


#include "trajectory_controller/global_controller.h"

double getValueFromPolynomial(std::vector<double> alphas, double time){

  double value = 0.0;
  for(int i = 0; i < size(alphas); i++){
    value += alphas[i] * pow(t,i);
  }

  return value;
}

global_controller::global_controller(){
  
  //param
  //distance step for resolution

  local_pub = nh.advertise<sensor_msgs::JointState>("/Plan", 1000, true);
  waypoints_sub = nh.subscribe("/waypoints", 1000, &global_controller::update_waypoints, this);

  cur_robot_pose = yaw = 0 and position = 0,0
 
}

global_controller::update_waypoints(nav_msgs::Path msg){
  bool goal_changed = false;

  if(goal_waypoints.size() == msg->poses.size()){
    for(int i = 0; i < msg->poses.size(); i++){
      //compare the waypointsst 
    }
  } else {
    goal_changed = true;
  }	

  //update path if goal position changed
  if(goal_changed){
    this->create_path()
  }
}

global_controller::create_path(){
  //find and store the robots current pose in the world frame (using tf)
  try{
    tf_listener.lookupTransform("/base_link", "/world", ros::Time(0), transform);
    cur_robot_pose = 
  } catch (tf::TransformException ex){
    ROS_ERROR("%s",ex.what());
    ros::Duration(1.0).sleep();
  }

  //calc the polynomial for the two points
  // assume straight line
  // assume heading is that from the first to second point
  x_alphas.clear();
  y_alphas.clear();
  h_alphas.clear():

  double x_start = cur_robot_pose.pose.position.x;
  double y_start = cur_robot_pose.pose.position.y;

  double x_end = goal_waypoints.front().pose.position.x;
  double y_end = goal_waypoints.front().pose.position.y;

  x_alphas[0].push_back(x_start);
  x_alphas[0].push_back(x_end - x_start);	  
  y_alphas[0].push_back(y_start);
  y_alphas[0].push_back(y_end - y_start);
  h_alphas[0].push_back(atan2(y_end - y_start,x_end - x_start));

  //calculate the new path
  plan.clear();
  //add the robots current location to the start of the path
  plan.push_back(cur_robot_pose);

  ros::Time plan_time = ros::Time::now();
  for(int i = 0; i < size(x_alphas); i++){
    distance = sqrt(pow(x_alphas[i].back(),2) + pow(y_alphas[i].back(),2));
    double time_increment = 1/floor(distance/this->step);
    
    for(int t = 0; t < 1; t + time_increment){
      geometry_msgs::PoseStamped next_pose;
      next_pose.header.stamp = plan_time;
      next_pose.header.frame_id = 
      
      next_pose.position.x = getValueFromPolynomial(x_alphas[i],t);
      next_pose.position.y = getValueFromPolynomial(y_alphas[i],t); 

      tf::Quaternion goal_quat = tf::createQuaternionFromYaw(getValueFromPolynomial(y_alphas[i],t));
      next_pose.pose.orientation.x = goal_quat.x();
      next_pose.pose.orientation.y = goal_quat.y();
      next_pose.pose.orientation.z = goal_quat.z();
      next_pose.pose.orientation.w = goal_quat.w();

      plan.push_back(next_pose);
    }
    //add the goal to the end of the path 
    plan.push_back(goal_waypoints[i]);
  }

  //publish the new path

}

int main(int argv, char * arc[]){

  ros::init(argc, argv, "global_controller");

  global_controller gc = global_controller();
  
  ROS_INFO("global path planner started!");	

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;

}



