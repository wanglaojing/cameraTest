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

#ifdef OUTPUT_DEBUG_INFO
	//�������е�ͼƬ
	cv::Mat drowDebugDetectLR;
	cv::Mat drowDebugDetectUD;
	cv::Mat drowDebugResult;
#endif

	cv::Point A;
	cv::Point B;
	cv::Point C;
	cv::Point D;

	vector < cv::Point > LeftBorder;
	vector < cv::Point > RightBorder;
	vector < cv::Point > UpBorder;
	vector < cv::Point > DownBorder;


private:
	BufferStorage *s;
	MicroDisplayInit *mdi;

	//�ƽ���������Ͻǣ���֪endline��x��Ϊ�߽�
	int GetEdgeLeftx3(cv::Point start, int range = 200);
	int GetEdgeLeft(cv::Point start, int range = 200);

	int GetEdgeRightx3(cv::Point start, int range = 200);
	int GetEdgeRight(cv::Point start, int range = 200);

	int GetEdgeUpx3(cv::Point start, int range = 200);
	int GetEdgeUpApproach(cv::Point start, cv::Point end, int range);
	int GetEdgeUp(cv::Point start, int range = 200);

	int GetEdgeDownx3(cv::Point start, int range = 200);
	int GetEdgeDownApproach(cv::Point start, cv::Point end, int range);
	int GetEdgeDown(cv::Point start, int range = 200);
	//�ƽ������ı�Ե
	int GetEdgeVerticalApproach(cv::Point start, cv::Point end, int range, bool isLeft, cv::Point Target);
	//����Եʱ���ۼӶ��ٸ�
	const int SUM_COUNT = 20;
	//ƽ��ÿ����֮��Ҫ��SUM_AVG_THRESHOD�������Ǳ߽磬��������ֵΪSUM_COUNT*SUM_AVG_THRESHOD;
	const int SUM_AVG_THRESHOD = 5;
	const int SUM_THRESHOD = SUM_COUNT * SUM_AVG_THRESHOD;


	//�����в���һ��
	const int ROW_SPAN = 111;
	//Ĭ������ɨ������ĵ㣬���ΪORANGE_MARGIN_LINE���ұ�Ϊwidth-ORANGE_MARGIN_LINE
	const int ORANGE_MARGIN_ROW = 200;
	//Ĭ�ϵģ���һ�����Ҷ��ٷ�Χ�ڽ���ɨ��
	const int ORANGE_RANGE_ROW = 150;


	//�����в���һ��
	const int COL_SPAN = 88;
	//Ĭ�ϵģ���һ�����¶��ٷ�Χ�ڽ���ɨ��
	const int ORANGE_RANGE_COL = 200;

	//ÿ��������������Χ
	const int RANGE_MINI = 40;		//�ο�ֵ50
	//ÿ�η�Χ���ٶ���
	const int RANGE_REDUCE_BY = 50;
	//��LIST<POINT>����

	static bool ORDER_BY_Y_ASC(cv::Point i, cv::Point j) { return i.y < j.y; }
	static bool ORDER_BY_Y_DESC(cv::Point i, cv::Point j) { return i.y > j.y; }
	static bool ORDER_BY_X_ASC(cv::Point i, cv::Point j) { return i.x < j.x; }
	static bool ORDER_BY_X_DESC(cv::Point i, cv::Point j) { return i.x > j.x; }
};
