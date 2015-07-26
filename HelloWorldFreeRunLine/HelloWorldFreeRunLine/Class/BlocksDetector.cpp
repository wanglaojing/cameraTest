#include "BlocksDetector.h"


BlocksDetector::BlocksDetector()
{
}


BlocksDetector::~BlocksDetector()
{
}

// ��line��x���긽���������Ƿ��б�Ե���������꣬û���򷵻�-1
int BlocksDetector::GetEdgeLeft(int line, int x, BufferStorage& s)
{
	const int range = 200;
	int offset = x - range;
	const int sumcount = 20;
	//����㷨
	cv::Mat oneLineGray;
	cv::cvtColor(s.NowBufferImg(cv::Rect(x - range, line, range + range, 1)), oneLineGray, CV_BGR2GRAY);

	int elementCount = oneLineGray.cols;//ÿ��Ԫ����
	uchar* linehead = oneLineGray.ptr<uchar>(0);//ÿ�е���ʼ��ַ
	int ret = -1;
	int diff = 2;
	for (size_t i = sumcount; i < oneLineGray.cols - 1 - sumcount; i++)
	{
		int leftsum = 0;
		for (size_t ii = i; ii > i - 20; ii--)
		{
			leftsum += linehead[ii];
		}
		int rightsum = 0;
		for (size_t ii = i + 1; ii <= i + 20; ii++)
		{
			rightsum += linehead[ii];
		}
		int c = (rightsum - leftsum);
		if (c > 20 * 10 && c > diff)
		{
			diff = c;
			ret = i;
		}
	}
	linehead[oneLineGray.cols - 1] = 0;
	if (ret >= 0)
		ret += offset;
	return ret;
}
int BlocksDetector::GetEdgeRight(int line, int x, BufferStorage& s)
{
	const int range = 200;
	int offset = x - range;
	const int sumcount = 20;
	//����㷨
	cv::Mat oneLineGray;
	cv::cvtColor(s.NowBufferImg(cv::Rect(x - range, line, range + range, 1)), oneLineGray, CV_BGR2GRAY);

	int elementCount = oneLineGray.cols;//ÿ��Ԫ����
	uchar* linehead = oneLineGray.ptr<uchar>(0);//ÿ�е���ʼ��ַ
	int ret = -1;
	int diff = 2;

	for (size_t i = elementCount - sumcount; i > sumcount - 1; i--)
	{
		int leftsum = 0;
		for (size_t ii = i; ii > i - 20; ii--)
		{
			leftsum += linehead[ii];
		}
		int rightsum = 0;
		for (size_t ii = i + 1; ii <= i + 20; ii++)
		{
			rightsum += linehead[ii];
		}
		int c = (leftsum - rightsum);
		if (c > 20 * 10 && c > diff)
		{
			diff = c;
			ret = i;
		}
	}
	linehead[oneLineGray.cols - 1] = 0;
	if (ret >= 0)
		ret += offset;
	return ret;
}