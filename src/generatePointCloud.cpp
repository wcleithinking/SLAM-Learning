#include <iostream>
#include <string>

using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloud;

const double camera_factor = 1000;
const double camera_cx = 325.5;
const double camera_cy = 253.5;
const double camera_fx = 518.0;
const double camera_fy = 519.0;

int main( int argc, char** argv)
{
	cv::Mat rgb, depth;
	rgb = cv::imread( "../data/test_figure_rgb.png" );
	depth = cv::imread( "../data/test_figure_depth.png" );

	PointCloud::Ptr cloud( new PointCloud ); // empty point cloud
	
	for (int m=0; m < depth.rows; m++)
	{
		for (int n=0; n < depth.cols; n++)
		{
			ushort d = depth.ptr<ushort>(m)[n];
			if (d==0) 
			{
				continue;
			}
			else
			{
				PointT p;

				p.z = double(d) / camera_factor;
				p.x = (n - camera_cx) * p.z / camera_fx;
				p.y = (m - camera_cy) * p.z / camera_fy;

				p.b = rgb.ptr<uchar>(m)[n*3];
				p.g = rgb.ptr<uchar>(m)[n*3+1];
				p.r = rgb.ptr<uchar>(m)[n*3+2];

				cloud->points.push_back( p );
			}
		}
	}
	
	cloud->height = 1;
	cloud->width = cloud->points.size();
	cout<<"point cloud size = "<<cloud->points.size()<<endl;
	cloud->is_dense = false;
	pcl::io::savePCDFile( "../data/test_figure_pointcloud.pcd", *cloud );

	cloud->points.clear();
	cout<<"Point cloud saved."<<endl;
	
	return 0;

}
