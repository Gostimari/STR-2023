#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud_conversion.h>

ros::Publisher newPointCloud;
bool runflag=false;
sensor_msgs::PointCloud2::ConstPtr pointcloud;
std::vector<geometry_msgs::Point32> valid_points;
sensor_msgs::PointCloud converted_points;
//
void handlePointCloud(sensor_msgs::PointCloud2::ConstPtr scan_out)
{
    pointcloud=scan_out;
    sensor_msgs::convertPointCloud2ToPointCloud(*pointcloud,converted_points);
    //std::cout<<"Points: "<<scan_out->height*scan_out->width<<std::endl;
    //std::cout<<"Points: "<<converted_points.points.size()<<std::endl;
    runflag=true;
}
void f1(sensor_msgs::PointCloud2::ConstPtr pc){
 //Part I 1)
    handlePointCloud(pc);
}
void f2(){
 //Part I 2)
    valid_points.clear(); // Clear valid_points before using it

    // Loop through all points
    for(const auto& point : converted_points.points){
        // Check if the point is valid
        if (!(point.x<0  || fabs(point.y)>10 || fabs(point.x)>30 || ( fabs(point.x)<2 && fabs(point.y)<1 ))){
            // If valid, add to the valid points vector
            valid_points.push_back(point);
        }
    }
}
void f3(){
    sensor_msgs::PointCloud output;
    output.header=pointcloud->header; //set message header (using the same from /velodyne_points)
    //Part I 3)
    output.points = valid_points;
    newPointCloud.publish(output);
}
int main(int argc, char **argv){
    ros::init(argc, argv, "t2g5e6_package");
    ros::NodeHandle nh("~");
    newPointCloud = nh.advertise<sensor_msgs::PointCloud>("/output_results", 100);
    ros::Subscriber PointCloudHandlervelodyne = nh.subscribe<sensor_msgs::PointCloud2>("/velodyne_points", 100, handlePointCloud);
    ros::Rate rate(100.0);
    while (nh.ok()){
        if(runflag){
            f1(pointcloud);
            f2();
            f3();
            runflag=false;
        }
    ros::spinOnce();
    rate.sleep();
    }
 return 1;
}