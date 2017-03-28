
// main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/nonfree.hpp>  
#include <opencv2/nonfree/features2d.hpp>  

#include "localfeature.h"

#pragma comment(lib,"opencv_core2413d.lib")          
#pragma comment(lib,"opencv_highgui2413d.lib")          
#pragma comment(lib,"opencv_imgproc2413d.lib") 
#pragma comment(lib,"opencv_nonfree2413d.lib")    
#pragma comment(lib,"opencv_features2d2413d.lib")    


using namespace std;

int _tmaindf(int argc, _TCHAR* argv[])
{
	/*if (argc != 6)
	{
	cout << "wrong usage!" << endl;
	cout << "usage: .exe FAST SIFT BruteForce queryImage trainImage" << endl;
	return -1;
	}*/

	string detectorType = "SIFT";
	string extractorType = "SIFT";
	string matchType = "BruteForce";
	string queryImagePath = "swan.jpg";
	string trainImagePath = "swan.jpg";


	Mat queryImage = imread(queryImagePath, CV_LOAD_IMAGE_GRAYSCALE);
	if (queryImage.empty())
	{
		cout << "read failed" << endl;
		return -1;
	}

	Mat trainImage = imread(trainImagePath, CV_LOAD_IMAGE_GRAYSCALE);
	if (trainImage.empty())
	{
		cout << "read failed" << endl;
		return -1;
	}


	Feature feature(detectorType, extractorType, matchType);

	vector<KeyPoint> queryKeypoints, trainKeypoints;
	feature.detectKeypoints(queryImage, queryKeypoints);
	feature.detectKeypoints(trainImage, trainKeypoints);


	Mat queryDescriptor, trainDescriptor;


	feature.extractDescriptors(queryImage, queryKeypoints, queryDescriptor);
	feature.extractDescriptors(trainImage, trainKeypoints, trainDescriptor);


	vector<DMatch> matches;
	feature.bestMatch(queryDescriptor, trainDescriptor, matches);

	vector<vector<DMatch>> knnmatches;
	feature.knnMatch(queryDescriptor, trainDescriptor, knnmatches, 2);

	Mat outImage;
	feature.saveMatches(queryImage, queryKeypoints, trainImage, trainKeypoints, matches, "../");


	return 0;
}
