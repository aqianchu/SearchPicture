// similarity.cpp : 定义控制台应用程序的入口点。
//http://blog.csdn.net/fengbingchun/article/details/42153261

#include "stdafx.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#pragma comment(lib,"opencv_core2413d.lib")          
#pragma comment(lib,"opencv_highgui2413d.lib")          
#pragma comment(lib,"opencv_imgproc2413d.lib")    


using namespace std;

int imgdfdsdiff(char *img1, char *img2)
{
	cv::Mat mat1,mat2, matSrc1, matSrc2;

	mat1 = cv::imread(img1, CV_LOAD_IMAGE_COLOR);
	CV_Assert(matSrc1.channels() == 3);

	mat2 = cv::imread(img2, CV_LOAD_IMAGE_COLOR);
	CV_Assert(matSrc2.channels() == 3);

	cv::resize(mat1, matSrc1, cv::Size(357, 419), 0, 0, cv::INTER_NEAREST);
	//cv::flip(matSrc1, matSrc1, 1);
	cv::resize(mat2, matSrc2, cv::Size(2177, 3233), 0, 0, cv::INTER_LANCZOS4);

	cv::Mat matDst1, matDst2;

	cv::resize(matSrc1, matDst1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::resize(matSrc2, matDst2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);

	cv::cvtColor(matDst1, matDst1, CV_BGR2GRAY);
	cv::cvtColor(matDst2, matDst2, CV_BGR2GRAY);

	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];

	for (int i = 0; i < 8; i++)
	{
		uchar* data1 = matDst1.ptr<uchar>(i);
		uchar* data2 = matDst2.ptr<uchar>(i);

		int tmp = i * 8;

		for (int j = 0; j < 8; j++)
		{
			int tmp1 = tmp + j;

			arr1[tmp1] = data1[j] / 4 * 4;
			arr2[tmp1] = data2[j] / 4 * 4;

			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	for (int i = 0; i < 64; i++)
	{
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
	if (arr1[i] != arr2[i])
		++iDiffNum;

	return iDiffNum;
}


int _tmaifdfn(int argc, _TCHAR* argv[])
{

	string strSrcImageName = "lurao.png";

	cv::Mat matSrc, matSrc1, matSrc2;

	matSrc = cv::imread(strSrcImageName, CV_LOAD_IMAGE_COLOR);
	CV_Assert(matSrc.channels() == 3);

	cv::resize(matSrc, matSrc1, cv::Size(357, 419), 0, 0, cv::INTER_NEAREST);
	//cv::flip(matSrc1, matSrc1, 1);
	cv::resize(matSrc, matSrc2, cv::Size(2177, 3233), 0, 0, cv::INTER_LANCZOS4);

	cv::Mat matDst1, matDst2;

	cv::resize(matSrc1, matDst1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::resize(matSrc2, matDst2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);

	cv::cvtColor(matDst1, matDst1, CV_BGR2GRAY);
	cv::cvtColor(matDst2, matDst2, CV_BGR2GRAY);

	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];

	for (int i = 0; i < 8; i++)
	{
		uchar* data1 = matDst1.ptr<uchar>(i);
		uchar* data2 = matDst2.ptr<uchar>(i);

		int tmp = i * 8;

		for (int j = 0; j < 8; j++)
		{
			int tmp1 = tmp + j;

			arr1[tmp1] = data1[j] / 4 * 4;
			arr2[tmp1] = data2[j] / 4 * 4;

			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	for (int i = 0; i < 64; i++)
	{
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
	if (arr1[i] != arr2[i])
		++iDiffNum;

	cout << "iDiffNum = " << iDiffNum << endl;

	if (iDiffNum <= 5)
		cout << "two images are very similar!" << endl;
	else if (iDiffNum > 10)
		cout << "they are two different images!" << endl;
	else
		cout << "two image are somewhat similar!" << endl;

	getchar();
	return 0;
}
/*
感知哈希算法(perceptual hash algorithm)，它的作用是对每张图像生成一个“指纹”(fingerprint)字符串，然后比较不同图像的指纹。结果越接近，就说明图像越相似。

实现步骤：
缩小尺寸：将图像缩小到8 * 8的尺寸，总共64个像素。这一步的作用是去除图像的细节，只保留结构 / 明暗等基本信息，摒弃不同尺寸 / 比例带来的图像差异；
简化色彩：将缩小后的图像，转为64级灰度，即所有像素点总共只有64种颜色；
计算平均值：计算所有64个像素的灰度平均值；
比较像素的灰度：将每个像素的灰度，与平均值进行比较，大于或等于平均值记为1，小于平均值记为0；
计算哈希值：将上一步的比较结果，组合在一起，就构成了一个64位的整数，这就是这张图像的指纹。组合的次序并不重要，只要保证所有图像都采用同样次序就行了；
得到指纹以后，就可以对比不同的图像，看看64位中有多少位是不一样的。在理论上，这等同于”汉明距离”(Hamming distance, 在信息论中，两个等长字符串之间的汉
明距离是两个字符串对应位置的不同字符的个数)。如果不相同的数据位数不超过5，就说明两张图像很相似；如果大于10，就说明这是两张不同的图像。
*/