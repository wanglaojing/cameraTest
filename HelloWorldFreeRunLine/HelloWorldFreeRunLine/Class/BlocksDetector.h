#pragma once

#include "../globle.h"
#include "BufferStorage.h";
#include "MicroDisplay\MicroDisplayInit.h";
class BlocksDetector
{
public:
	BlocksDetector(BufferStorage *ss, MicroDisplayInit *mdii);
	~BlocksDetector();

	void Start();

	//�������е�ͼƬ
	cv::Mat drowDebugDetect;
	cv::Mat drowDebugResult;

	cv::Point A;
	cv::Point B;
	cv::Point C;
	cv::Point D;
private:
	BufferStorage *s;
	MicroDisplayInit *mdi;

	//�ƽ���������Ͻǣ���֪endline��x��Ϊ�߽�
	int GetEdgeLeftx3(cv::Point start, int range = 200);
	int GetEdgeLeftApproach(cv::Point start, cv::Point end, int range = 200);
	int GetEdgeLeft(cv::Point start, int range = 200);
	int GetEdgeRightx3(cv::Point start, int range = 200);
	int GetEdgeRightApproach(cv::Point start, cv::Point end, int range = 200);
	int GetEdgeRight(cv::Point start, int range = 200);

	vector < cv::Point > leftBorder;
	vector < cv::Point > rightBorder;
	//vector<int> leftEdgeLine;//y���꣬�ڼ���
	//map<int, int> leftEdgeX;//����Y����洢X���꣬�ڼ���
	//vector<int> rightEdgeLine;//y���꣬�ڼ���
	//map<int, int> rightEdgeX;
	const int linespan = 100;
	const int margin = 300;
	const int orange = 400;

	static bool comp(cv::Point i, cv::Point j) { return i.y < j.y; }
};

