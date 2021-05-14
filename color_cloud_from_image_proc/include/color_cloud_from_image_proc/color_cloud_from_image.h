#ifndef COLOR_CLOUD_FROM_IMAGE_H
#define COLOR_CLOUD_FROM_IMAGE_H

#include <ros/ros.h>

#include <Eigen/Eigen>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>

#include <functional>

#include <aslam/cameras.hpp>

#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

// pcl
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <pcl_conversions/pcl_conversions.h>

#include <pcl_ros/transforms.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>

#include "robot_self_filter/self_see_filter.h"

#include <tf2_ros/message_filter.h>
#include <message_filters/subscriber.h>

#include <kalibr_camera_loader/camera_loader.h>


namespace color_cloud_from_image {

  class ColorCloudFromImage {
  public:
    ColorCloudFromImage(ros::NodeHandle &nh, ros::NodeHandle &pnh);
    ~ColorCloudFromImage();
  private:
    /* on new pc:
     * 1. iterate over each point
     * 2. transform point to cam frame
     * 3. project point to each cam until first success (aslam cam)
     * 4. save pixel color for point
     * 5. republish cloud
     */
    void cloudCallback(const sensor_msgs::PointCloud2ConstPtr& cloud_ptr);
    void connectCb();

    void startSubscribers();
    void stopSubscribers();

    ros::NodeHandle nh_, pnh_;

    bool lazy_;
    bool enabled_;

    sensor_msgs::PointCloud2 last_cloud_;

    kalibr_image_geometry::CameraLoader camera_loader_;
    boost::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    boost::shared_ptr<tf2_ros::TransformListener> tf_listener_;

    boost::shared_ptr<filters::SelfFilter<pcl::PointCloud<pcl::PointXYZ> > > self_filter_;
    bool use_self_filter_;

    //ros::Subscriber cloud_sub_;
    ros::Publisher cloud_pub_;
    ros::Publisher cloud_debug_pub_;

    std::vector<std::string> filter_frames_;

    tf2_ros::MessageFilter<sensor_msgs::PointCloud2> *mn_;
    message_filters::Subscriber<sensor_msgs::PointCloud2> *sub_;

    ros::Subscriber no_filter_sub_;

  };
}

#endif
