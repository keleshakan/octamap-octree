
#include <pcl/point_cloud.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/pcd_io.h>

#include <pcl/compression/octree_pointcloud_compression.h>

#include <stdio.h>
#include <sstream>
#include <stdlib.h>

#ifdef WIN32
# define sleep(x) Sleep((x)*1000)
#endif


int main (int argc, char **argv)
{
    bool showStatistics = true;

    // for a full list of profiles see: /io/include/pcl/compression/compression_profiles.h
    pcl::io::compression_Profiles_e compressionProfile = pcl::io::MED_RES_OFFLINE_COMPRESSION_WITH_COLOR;

    // instantiate point cloud compression for encoding and decoding
    auto PointCloudEncoder = new pcl::io::OctreePointCloudCompression<pcl::PointXYZRGBA> (compressionProfile, showStatistics);
    auto PointCloudDecoder = new pcl::io::OctreePointCloudCompression<pcl::PointXYZRGBA> ();

    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGBA>);
    pcl::io::loadPCDFile ("learn23.pcd", *cloud);



    // stringstream to store compressed point cloud
    std::stringstream compressedData;
    // output pointcloud
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloudOut (new pcl::PointCloud<pcl::PointXYZRGBA> ());

    // compress point cloud
    PointCloudEncoder->encodePointCloud (cloud, compressedData);

    // decompress point cloud
    //PointCloudDecoder->decodePointCloud (compressedData, cloudOut);

    pcl::visualization::CloudViewer viewer("viewer");
    viewer.showCloud(cloud);

    while(!viewer.wasStopped()){

    }

    return (0);
}