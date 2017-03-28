// similarity.cpp : �������̨Ӧ�ó������ڵ㡣
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
��֪��ϣ�㷨(perceptual hash algorithm)�����������Ƕ�ÿ��ͼ������һ����ָ�ơ�(fingerprint)�ַ�����Ȼ��Ƚϲ�ͬͼ���ָ�ơ����Խ�ӽ�����˵��ͼ��Խ���ơ�

ʵ�ֲ��裺
��С�ߴ磺��ͼ����С��8 * 8�ĳߴ磬�ܹ�64�����ء���һ����������ȥ��ͼ���ϸ�ڣ�ֻ�����ṹ / �����Ȼ�����Ϣ��������ͬ�ߴ� / ����������ͼ����죻
��ɫ�ʣ�����С���ͼ��תΪ64���Ҷȣ����������ص��ܹ�ֻ��64����ɫ��
����ƽ��ֵ����������64�����صĻҶ�ƽ��ֵ��
�Ƚ����صĻҶȣ���ÿ�����صĻҶȣ���ƽ��ֵ���бȽϣ����ڻ����ƽ��ֵ��Ϊ1��С��ƽ��ֵ��Ϊ0��
�����ϣֵ������һ���ıȽϽ���������һ�𣬾͹�����һ��64λ�����������������ͼ���ָ�ơ���ϵĴ��򲢲���Ҫ��ֻҪ��֤����ͼ�񶼲���ͬ����������ˣ�
�õ�ָ���Ժ󣬾Ϳ��ԶԱȲ�ͬ��ͼ�񣬿���64λ���ж���λ�ǲ�һ���ġ��������ϣ����ͬ�ڡ��������롱(Hamming distance, ����Ϣ���У������ȳ��ַ���֮��ĺ�
�������������ַ�����Ӧλ�õĲ�ͬ�ַ��ĸ���)���������ͬ������λ��������5����˵������ͼ������ƣ��������10����˵���������Ų�ͬ��ͼ��
*/