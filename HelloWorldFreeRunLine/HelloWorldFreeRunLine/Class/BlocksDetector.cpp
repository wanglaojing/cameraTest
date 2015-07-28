#include "BlocksDetector.h"



BlocksDetector::BlocksDetector(BufferStorage *ss, MicroDisplayInit *mdii)
{
	s = ss;
	mdi = mdii;
#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		drowDebugDetect = (*s).NowBufferImg.clone();
		drowDebugResult = (*s).NowBufferImg.clone();
	}
#endif
}
BlocksDetector::~BlocksDetector()
{
}

void BlocksDetector::Start()
{
	//vector<int> leftEdgeXTmp;
	if (1 == 1)//ʹ��IF�����ֲ�����
	{
		int range = ORANGE_RANGE_ROW;
		int leftX = ORANGE_MARGIN_ROW;
		int lastFoundLine = 0;//��һ���ҵ���Ե���ڵڼ���
		int noneCount = 0;//����û���ҵ���Ե��������
		bool needReFind = false;//�Ը����Ƿ���Ҫ����range��������
		for (size_t i = 0; (i + 3) < (*mdi).MaxPics; i += ROW_SPAN)
		{
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				cv::circle(drowDebugDetect, cv::Point(leftX, i), 5, cv::Scalar(0, 255, 255), 3);
			}
#endif
			int x1 = GetEdgeLeftx3(cv::Point(leftX, i), range);
			if (x1 >= 0)
			{
				cv::Mat oneLine = (*s).NowBufferImg(cv::Rect(0, i, (*mdi).width, 1));
				uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ

				lineheadRGB[x1 * 3 + 0] = 0;
				lineheadRGB[x1 * 3 + 1] = 0;
				lineheadRGB[x1 * 3 + 2] = 255;
#ifdef OUTPUT_DEBUG_INFO
				if (OUTPUT_DEBUG_INFO)
				{
					cv::circle(drowDebugResult, cv::Point(x1, i), 5, cv::Scalar(0, 0, 255), 2);
				}
#endif

				LeftBorder.push_back(cv::Point(x1, i));

				noneCount = 0;
				lastFoundLine = i;
				needReFind = true;

				//����ģ��Ԥ����һ�����x����
				if (LeftBorder.size() > 2)
				{
					int last_1 = LeftBorder[LeftBorder.size() - 1].x;
					int last_2 = LeftBorder[LeftBorder.size() - 2].x;
					leftX = last_1 + last_1 - last_2;
				}
				else
					leftX = x1;
				if (range > 50) range -= 50;
			}
			else
			{
				if (lastFoundLine > 0)
					noneCount++;
				if (noneCount > 3)//����N����û���ҵ��߽������
					break;
				if (needReFind)//�Ƿ�Ҫ����ɨ�����
				{
					range = ORANGE_RANGE_ROW;
					i -= ROW_SPAN;
					needReFind = false;
				}
			}
		}
	}
	//vector<int> rightEdgeXTmp;
	if (1 == 1)//ʹ��IF�����ֲ�����
	{
		int range = ORANGE_RANGE_ROW;
		int rightX = (*mdi).width - ORANGE_MARGIN_ROW;
		int lastFoundLine = 0;//��һ���ҵ���Ե���ڵڼ���
		int noneCount = 0;//����û���ҵ���Ե��������
		bool needReFind = false;//�Ը����Ƿ���Ҫ����range��������
		for (size_t i = 0; (i + 3) < (*mdi).MaxPics; i += ROW_SPAN)
		{
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				cv::circle(drowDebugDetect, cv::Point(rightX, i), 5, cv::Scalar(0, 255, 255), 3);
			}
#endif
			int x1 = GetEdgeRightx3(cv::Point(rightX, i), range);
			if (x1 >= 0)
			{
				cv::Mat oneLine = (*s).NowBufferImg(cv::Rect(0, i, (*mdi).width, 1));
				uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ

				lineheadRGB[x1 * 3 + 0] = 0;
				lineheadRGB[x1 * 3 + 1] = 0;
				lineheadRGB[x1 * 3 + 2] = 255;

#ifdef OUTPUT_DEBUG_INFO
				if (OUTPUT_DEBUG_INFO)
				{
					cv::circle(drowDebugResult, cv::Point(x1, i), 5, cv::Scalar(0, 0, 255), 2);
				}
#endif

				RightBorder.push_back(cv::Point(x1, i));

				noneCount = 0;
				lastFoundLine = i;
				needReFind = true;

				//����ģ��Ԥ����һ�����x����
				if (RightBorder.size() > 2)
				{
					int last_1 = RightBorder[RightBorder.size() - 1].x;
					int last_2 = RightBorder[RightBorder.size() - 2].x;
					rightX = last_1 + last_1 - last_2;
				}
				else
					rightX = x1;
				if (range > 50) range -= 50;
			}
			else
			{
				if (lastFoundLine > 0)
					noneCount++;
				if (noneCount > 3)//����N����û���ҵ��߽������
					break;
				if (needReFind)//�Ƿ�Ҫ����ɨ�����
				{
					range = ORANGE_RANGE_ROW;
					i -= ROW_SPAN;
					needReFind = false;
				}
			}
		}
	}
	//��û���ҵ���Ե��ֹͣ����
	if (LeftBorder.size() == 0 || RightBorder.size() == 0)
		return;

	int leftFirstLine = LeftBorder[0].y, leftFirstX = LeftBorder[0].x;
	int leftLastLine = LeftBorder[LeftBorder.size() - 1].y, leftLastX = LeftBorder[LeftBorder.size() - 1].x;
	GetEdgeLeftApproach(cv::Point(-1, (leftFirstLine > ROW_SPAN) ? (leftFirstLine - ROW_SPAN) : 0), cv::Point(leftFirstX, leftFirstLine));
	GetEdgeLeftApproach(cv::Point(leftLastX, leftLastLine), cv::Point(-1, (leftLastLine + ROW_SPAN) > (*mdi).MaxPics ? ((*mdi).MaxPics - 1) : (leftLastLine + ROW_SPAN)));

	int rightFirstLine = RightBorder[0].y, rightFirstX = RightBorder[0].x;
	int rightLastLine = RightBorder[RightBorder.size() - 1].y, rightLastX = RightBorder[RightBorder.size() - 1].x;
	GetEdgeRightApproach(cv::Point(-1, (rightFirstLine > ROW_SPAN) ? (rightFirstLine - ROW_SPAN) : 0), cv::Point(rightFirstX, rightFirstLine));
	GetEdgeRightApproach(cv::Point(rightLastX, rightLastLine), cv::Point(-1, (rightLastLine + ROW_SPAN) > (*mdi).MaxPics ? ((*mdi).MaxPics - 1) : (rightLastLine + ROW_SPAN)));

	std::sort(LeftBorder.begin(), LeftBorder.end(), ORDER_BY_Y_ASC);
	std::sort(RightBorder.begin(), RightBorder.end(), ORDER_BY_Y_ASC);



	//�����ϱ�Ե
	if (LeftBorder[0].y != 0 && RightBorder[0].y != 0)
	{
		int range = ORANGE_RANGE_COL;
		//��ȡROI
		//ROI�����yֵ��ͬʱҲ�Ǽ�������y�����OFFSET
		int offsetY = LeftBorder[0].y - ORANGE_RANGE_COL;
		if (offsetY < 0) offsetY = 0;
		int height = LeftBorder[0].y + ORANGE_RANGE_COL - offsetY;
		//ROI�б߽���ҵ�����и�
		int centerY = LeftBorder[0].y - offsetY;

		int lastFoundRow = 0;//��һ���ҵ���Ե���ڵڼ���
		int noneCount = 0;//����û���ҵ���Ե��������
		bool needReFind = false;//�Ը����Ƿ���Ҫ����range��������


		cv::Mat upROI;
		cv::cvtColor((*s).NowBufferImg(cv::Rect(0, offsetY, (*mdi).width, height)), upROI, CV_RGB2GRAY);
		cv::Mat img = (*s).NowBufferImg;
		for (size_t x = 0; x < (*mdi).width; x += COL_SPAN)
		{
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				cv::circle(drowDebugDetect, cv::Point(x, offsetY + centerY), 5, cv::Scalar(0, 255, 255), 3);
			}
#endif
			//���������ͼ���Yֵ
			int absoy1 = GetEdgeUP(upROI, offsetY, cv::Point(x, centerY), range);
			if (absoy1 >= 0)
			{
				cv::Mat oneLine = (*s).NowBufferImg(cv::Rect(0, absoy1, (*mdi).width, 1));
				uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ

				lineheadRGB[x * 3 + 0] = 0;
				lineheadRGB[x * 3 + 1] = 0;
				lineheadRGB[x * 3 + 2] = 255;
			}
		}
	}

	//�����±�Ե
	if (LeftBorder[LeftBorder.size() - 1].y != 0 && RightBorder[RightBorder.size() - 1].y != 0)
	{

	}










	A = LeftBorder[0];
	B = RightBorder[0];
	C = RightBorder[RightBorder.size() - 1];
	D = LeftBorder[LeftBorder.size() - 1];

#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		cv::putText(drowDebugResult, "A", A, CV_FONT_HERSHEY_COMPLEX, 2, cv::Scalar(255, 0, 0));
		cv::putText(drowDebugResult, "B", B, CV_FONT_HERSHEY_COMPLEX, 2, cv::Scalar(255, 0, 0));
		cv::putText(drowDebugResult, "C", C, CV_FONT_HERSHEY_COMPLEX, 2, cv::Scalar(255, 0, 0));
		cv::putText(drowDebugResult, "D", D, CV_FONT_HERSHEY_COMPLEX, 2, cv::Scalar(255, 0, 0));
	}
#endif
}

int BlocksDetector::GetEdgeLeftx3(cv::Point start, int range)
{
	int a = BlocksDetector::GetEdgeLeft(start, range);
	int b = BlocksDetector::GetEdgeLeft(cv::Point(start.x, start.y + 1), range);
	int c = BlocksDetector::GetEdgeLeft(cv::Point(start.x, start.y + 1), range);
	if ((a + b + c) > 0 && abs((a + c - b - b)) < 5)
		return ((a + b + c) / 3);
	return -1;
}

int BlocksDetector::GetEdgeLeftApproach(cv::Point start, cv::Point end, int range)
{
	int x1 = -1;
	int startline = start.y;
	int endline = end.y;
	int middleLine = (endline - startline) / 2 + startline;
	//���ַ�����startline��endline���м��Ƿ��б߽�
	//�ݹ�ֱ��startline = endline
#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		cv::circle(drowDebugDetect, cv::Point(end.x, middleLine), 3, cv::Scalar(255, 0, 0), 2);
	}
#endif
	if (start.x == -1)
	{
		if (middleLine == startline || middleLine == endline)
			return end.x;
		x1 = GetEdgeLeftx3(cv::Point(end.x, middleLine), range);
		if (x1 > 0)
		{
			LeftBorder.push_back(cv::Point(x1, middleLine));
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				if (x1 > 0)
				{
					cv::Mat oneLine = (*s).NowBufferImg(cv::Rect(0, middleLine, (*mdi).width, 1));
					int elementCount = (*mdi).width;//ÿ��Ԫ����
					uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ
					lineheadRGB[x1 * 3 + 0] = 0;
					lineheadRGB[x1 * 3 + 1] = 0;
					lineheadRGB[x1 * 3 + 2] = 255;

					cv::circle(drowDebugResult, cv::Point(x1, middleLine), 3, cv::Scalar(0, 255, 0), 1);
				}
			}
#endif
			x1 = GetEdgeLeftApproach(start, cv::Point(x1, middleLine));
		}
		else
			x1 = GetEdgeLeftApproach(cv::Point(x1, middleLine), end);
	}
	else
	{
		if (middleLine == startline || middleLine == endline)
			return start.x;
		x1 = GetEdgeLeftx3(cv::Point(start.x, middleLine), range);
		if (x1 > 0)
		{
			LeftBorder.push_back(cv::Point(x1, middleLine));
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				if (x1 > 0)
				{
					cv::Mat oneLine = (*s).NowBufferImg(cv::Rect(0, middleLine, (*mdi).width, 1));
					int elementCount = (*mdi).width;//ÿ��Ԫ����
					uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ
					lineheadRGB[x1 * 3 + 0] = 0;
					lineheadRGB[x1 * 3 + 1] = 0;
					lineheadRGB[x1 * 3 + 2] = 255;

					cv::circle(drowDebugResult, cv::Point(x1, middleLine), 3, cv::Scalar(0, 255, 0), 1);
				}
			}
#endif
			x1 = GetEdgeLeftApproach(cv::Point(x1, middleLine), end);
		}
		else
			x1 = GetEdgeLeftApproach(start, cv::Point(-1, middleLine));
	}
	return x1;
}

// ��:Point start���긽���������Ƿ��б�Ե���������꣬û���򷵻�-1
int BlocksDetector::GetEdgeLeft(cv::Point start, int range)
{
	const int sumcount = SUM_COUNT;
	//����㷨
	cv::Mat oneLineGray;
	int xstart = start.x - range;
	if (xstart < 0)
		xstart = 0;
	int width = range + range;
	if ((width + xstart) >(*mdi).width - 1)
		width = (*mdi).width - 1 - xstart;
	cv::cvtColor((*s).NowBufferImg(cv::Rect(xstart, start.y, width, 1)), oneLineGray, CV_BGR2GRAY);

	int elementCount = oneLineGray.cols;//ÿ��Ԫ����
	uchar* linehead = oneLineGray.ptr<uchar>(0);//ÿ�е���ʼ��ַ
	int ret = -1;
	int diff = SUM_THRESHOD;
	uchar* lineheadO;


#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		lineheadO = drowDebugDetect(cv::Rect(xstart, start.y, width, 1)).ptr<uchar>(0);//ÿ�е���ʼ��ַ
	}
#endif

	for (size_t i = sumcount; i < oneLineGray.cols - 1 - sumcount; i++)
	{
		int leftsum = 0;
		for (size_t ii = i; ii > i - sumcount; ii--)
		{
			leftsum += linehead[ii];
		}
		int rightsum = 0;
		for (size_t ii = i + 1; ii <= i + sumcount; ii++)
		{
			rightsum += linehead[ii];
		}
		int c = (rightsum - leftsum);
		if (c > diff)
		{
			diff = c;
			ret = i;
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				lineheadO[i * 3] = 255;//�����ɫ
			}
#endif
		}
#ifdef OUTPUT_DEBUG_INFO
		if (OUTPUT_DEBUG_INFO)
		{
			lineheadO[i * 3 + 1] = 255;//�����ɫ
		}
#endif
	}
	if (ret >= 0)
	{
#ifdef OUTPUT_DEBUG_INFO
		if (OUTPUT_DEBUG_INFO)
		{
			lineheadO[ret * 3 + 2] = 255;//����ɫ
		}
#endif
		ret += xstart;
	}
	return ret;
}
int BlocksDetector::GetEdgeRightx3(cv::Point start, int range)
{
	int a = BlocksDetector::GetEdgeRight(start, range);
	int b = BlocksDetector::GetEdgeRight(cv::Point(start.x, start.y + 1), range);
	int c = BlocksDetector::GetEdgeRight(cv::Point(start.x, start.y + 1), range);
	if ((a + b + c) > 0 && abs((a + c - b - b)) < 5)
		return ((a + b + c) / 3);
	return -1;
}
int BlocksDetector::GetEdgeRightApproach(cv::Point start, cv::Point end, int range)
{
	int x1 = -1;
	int startline = start.y;
	int endline = end.y;
	int middleLine = (endline - startline) / 2 + startline;
	//���ַ�����startline��endline���м��Ƿ��б߽�
	//�ݹ�ֱ��startline = endline
#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		cv::circle(drowDebugDetect, cv::Point(end.x, middleLine), 3, cv::Scalar(255, 0, 0), 2);
	}
#endif

	if (start.x == -1)
	{
		if (middleLine == startline || middleLine == endline)
			return end.x;
		x1 = GetEdgeRightx3(cv::Point(end.x, middleLine), range);
		if (x1 > 0)
		{
			RightBorder.push_back(cv::Point(x1, middleLine));
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				if (x1 > 0)
				{
					cv::Mat oneLine = (*s).NowBufferImg(cv::Rect(0, middleLine, (*mdi).width, 1));
					int elementCount = (*mdi).width;//ÿ��Ԫ����
					uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ
					lineheadRGB[x1 * 3 + 0] = 0;
					lineheadRGB[x1 * 3 + 1] = 0;
					lineheadRGB[x1 * 3 + 2] = 255;
					cv::circle(drowDebugResult, cv::Point(x1, middleLine), 3, cv::Scalar(0, 255, 0), 1);
				}
			}
#endif
			x1 = GetEdgeRightApproach(start, cv::Point(x1, middleLine));
		}
		else
			x1 = GetEdgeRightApproach(cv::Point(x1, middleLine), end);
	}
	else
	{
		if (middleLine == startline || middleLine == endline)
			return start.x;
		x1 = GetEdgeRightx3(cv::Point(start.x, middleLine), range);
		if (x1 > 0)
		{
			RightBorder.push_back(cv::Point(x1, middleLine));
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				if (x1 > 0)
				{
					cv::Mat oneLine = (*s).NowBufferImg(cv::Rect(0, middleLine, (*mdi).width, 1));
					int elementCount = (*mdi).width;//ÿ��Ԫ����
					uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ
					lineheadRGB[x1 * 3 + 0] = 0;
					lineheadRGB[x1 * 3 + 1] = 0;
					lineheadRGB[x1 * 3 + 2] = 255;
					cv::circle(drowDebugResult, cv::Point(x1, middleLine), 3, cv::Scalar(0, 255, 0), 1);
				}
			}
#endif
			x1 = GetEdgeRightApproach(cv::Point(x1, middleLine), end);
		}
		else
			x1 = GetEdgeRightApproach(start, cv::Point(-1, middleLine));
	}
	return x1;
}
int BlocksDetector::GetEdgeRight(cv::Point start, int range)
{
	const int sumcount = SUM_COUNT;
	//����㷨
	cv::Mat oneLineGray;
	int xstart = start.x - range;
	if (xstart < 0)
		xstart = 0;
	int width = range + range;
	if ((width + xstart) >(*mdi).width - 1)
		width = (*mdi).width - 1 - xstart;
	cv::cvtColor((*s).NowBufferImg(cv::Rect(xstart, start.y, width, 1)), oneLineGray, CV_BGR2GRAY);

	int elementCount = oneLineGray.cols;//ÿ��Ԫ����
	uchar* linehead = oneLineGray.ptr<uchar>(0);//ÿ�е���ʼ��ַ
	int ret = -1;
	int diff = SUM_THRESHOD;

	uchar* lineheadO;
#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		lineheadO = drowDebugDetect(cv::Rect(xstart, start.y, width, 1)).ptr<uchar>(0);//ÿ�е���ʼ��ַ
	}
#endif
	for (size_t i = sumcount; i < oneLineGray.cols - 1 - sumcount; i++)
	{
		int leftsum = 0;
		for (size_t ii = i; ii > i - sumcount; ii--)
		{
			leftsum += linehead[ii];
		}
		int rightsum = 0;
		for (size_t ii = i + 1; ii <= i + sumcount; ii++)
		{
			rightsum += linehead[ii];
		}
		int c = (leftsum - rightsum);
		if (c > diff)
		{
			diff = c;
			ret = i;
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				lineheadO[i * 3] = 255;//�����ɫ
			}
#endif
		}
#ifdef OUTPUT_DEBUG_INFO
		if (OUTPUT_DEBUG_INFO)
		{
			lineheadO[i * 3 + 1] = 255;//�����ɫ
		}
#endif
	}
	linehead[oneLineGray.cols - 1] = 0;
	if (ret >= 0)
	{
#ifdef OUTPUT_DEBUG_INFO
		if (OUTPUT_DEBUG_INFO)
		{
			lineheadO[ret * 3 + 2] = 255;//����ɫ
		}
#endif
		ret += xstart;
	}
	return ret;
}



/*******************�ϱ�Ե���ҿ�ʼ********************/

int BlocksDetector::GetEdgeUP(cv::Mat& ROI, int offsetY, cv::Point start, int range)
{
	const int sumcount = SUM_COUNT;
	int ystart = start.y - range;
	if (ystart < 0)
		ystart = 0;
	int height = range + range;
	if ((height + ystart) > ROI.rows)
		height = ROI.rows - 1 - ystart;
	int ret = -1;
	int diff = SUM_THRESHOD;
	for (size_t y = ystart + sumcount; y < ystart + height - sumcount; y++)
	{
		int upsum = 0;
		for (size_t yy = y; yy > y - sumcount; yy--)
		{
			upsum += ROI.ptr<uchar>(yy)[start.x];
		}
		int downsum = 0;
		for (size_t yy = y + 1; yy <= y + sumcount; yy++)
		{
			downsum += ROI.ptr<uchar>(yy)[start.x];
		}
		int c = (upsum - downsum);
		if (c > diff)
		{
			diff = c;
			ret = y;
		}
	}
	if (ret >= 0)
	{
		ret += offsetY;
	}
	return ret;
}