#include "BlocksDetector.h"



BlocksDetector::BlocksDetector(BufferStorage *ss, MicroDisplayInit *mdii)
{
	s = ss;
	mdi = mdii;

	//��ʼ�����Ե������
	leftY = 0;//�ڵڼ��м��
	leftX = ORANGE_MARGIN_ROW;//�����е�
	leftRnage = ORANGE_RANGE_ROW;//���е���Χ���������ڼ��
	leftNoneCount = 0;//����û���ҵ���Ե��������
	leftNeedReFind = false;//�Ը����Ƿ���Ҫ����range��������

	rightY = 0;//�ڵڼ��м��
	rightX = (*mdi).width - ORANGE_MARGIN_ROW;;//�����е�
	rightRnage = ORANGE_RANGE_ROW;//���е���Χ���������ڼ��
	rightNoneCount = 0;//����û���ҵ���Ե��������
	rightNeedReFind = false;//�Ը����Ƿ���Ҫ����range��������


#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		drowDebugDetectLR = (*s).NowBufferImg.clone();
		drowDebugDetectUD = (*s).NowBufferImg.clone();
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
		for (; (leftY + 3) < (*mdi).MaxPics; leftY += ROW_SPAN)
		{
			if (leftNoneCount > 3)//����N����û���ҵ��߽������
				break;
			if(DetectOneLineLeft(leftY) < 0)
				if (leftNeedReFind)//�Ƿ�Ҫ����ɨ�����
				{
					leftRnage = ORANGE_RANGE_ROW;
					leftY -= ROW_SPAN;
					leftNeedReFind = false;
				}
		}
		//��û���ҵ���Ե��ֹͣ����
		if (LeftBorder.size() == 0)
			return;
		//�ƽ�����
		int leftFirstLine = LeftBorder[0].y, leftFirstX = LeftBorder[0].x;
		int leftLastLine = LeftBorder[LeftBorder.size() - 1].y, leftLastX = LeftBorder[LeftBorder.size() - 1].x;
		cv::Point upstart = cv::Point(-1, (leftFirstLine > ROW_SPAN) ? (leftFirstLine - ROW_SPAN) : 0);
		cv::Point upend = cv::Point(leftFirstX, leftFirstLine);
		GetEdgeVerticalApproach(upstart, upend, RANGE_MINI, true, upstart);
		cv::Point downstart = cv::Point(leftLastX, leftLastLine);
		cv::Point downend = cv::Point(-1, (leftLastLine + ROW_SPAN) > (*mdi).MaxPics ? ((*mdi).MaxPics - 1) : (leftLastLine + ROW_SPAN));
		GetEdgeVerticalApproach(downstart, downend, RANGE_MINI, true, downend);


#ifdef OUTPUT_DEBUG_INFO
		if (OUTPUT_DEBUG_INFO)
		{
			//debug��ͼ
			for (size_t i = 0; i < LeftBorder.size(); i++)
				cv::circle(drowDebugResult, LeftBorder[i], 5, cv::Scalar(0, 0, 255), 2);
			for (size_t i = 0; i < tmpLeftList.size(); i++)
				cv::circle(drowDebugResult, tmpLeftList[i], 6, cv::Scalar(128, 128, 0), 1);
			for (size_t i = 0; i < LeftUp.size(); i++)
				cv::circle(drowDebugResult, LeftUp[i], 1, cv::Scalar(0, 255, 0), 1);
			for (size_t i = 0; i < LeftDown.size(); i++)
				cv::circle(drowDebugResult, LeftDown[i], 1, cv::Scalar(0, 255, 0), 1);
		}
#endif
		LeftBorder.insert(LeftBorder.end(), LeftUp.begin(), LeftUp.end());
		LeftBorder.insert(LeftBorder.end(), LeftDown.begin(), LeftDown.end());
	}
	if (1 == 1)//ʹ��IF�����ֲ�����
	{
		for (; (rightY + 3) < (*mdi).MaxPics; rightY += ROW_SPAN)
		{
			if (rightNoneCount > 3)//����N����û���ҵ��߽������
				break;
			if (DetectOneLineRight(rightY) < 0)
				if (rightNeedReFind)//�Ƿ�Ҫ����ɨ�����
				{
					rightRnage = ORANGE_RANGE_ROW;
					rightY -= ROW_SPAN;
					rightNeedReFind = false;
				}
		}
		//��û���ҵ���Ե��ֹͣ����
		if (RightBorder.size() == 0)
			return;
		//�ƽ�����
		int rightFirstLine = RightBorder[0].y, rightFirstX = RightBorder[0].x;
		int rightLastLine = RightBorder[RightBorder.size() - 1].y, rightLastX = RightBorder[RightBorder.size() - 1].x;
		cv::Point upstart = cv::Point(-1, (rightFirstLine > ROW_SPAN) ? (rightFirstLine - ROW_SPAN) : 0);
		cv::Point upend = cv::Point(rightFirstX, rightFirstLine);
		GetEdgeVerticalApproach(upstart, upend, RANGE_MINI, false, upstart);
		cv::Point downstart = cv::Point(rightLastX, rightLastLine);
		cv::Point downend = cv::Point(-1, (rightLastLine + ROW_SPAN) > (*mdi).MaxPics ? ((*mdi).MaxPics - 1) : (rightLastLine + ROW_SPAN));
		GetEdgeVerticalApproach(downstart, downend, RANGE_MINI, false, downstart);

#ifdef OUTPUT_DEBUG_INFO
		if (OUTPUT_DEBUG_INFO)
		{
			for (size_t i = 0; i < RightBorder.size(); i++)
				cv::circle(drowDebugResult, RightBorder[i], 5, cv::Scalar(0, 0, 255), 2);
			for (size_t i = 0; i < tmpRightList.size(); i++)
				cv::circle(drowDebugResult, tmpRightList[i], 6, cv::Scalar(128, 128, 0), 1);
			for (size_t i = 0; i < RightUp.size(); i++)
				cv::circle(drowDebugResult, RightUp[i], 1, cv::Scalar(0, 255, 0), 1);
			for (size_t i = 0; i < RightDown.size(); i++)
				cv::circle(drowDebugResult, RightDown[i], 1, cv::Scalar(0, 255, 0), 1);
		}
#endif
		RightBorder.insert(RightBorder.end(), RightUp.begin(), RightUp.end());
		RightBorder.insert(RightBorder.end(), RightDown.begin(), RightDown.end());
	}
	std::sort(LeftBorder.begin(), LeftBorder.end(), ORDER_BY_Y_ASC);
	std::sort(RightBorder.begin(), RightBorder.end(), ORDER_BY_Y_ASC);
	//return;


	//�����ϱ�Ե
	if (LeftBorder[0].y != 0 && RightBorder[0].y != 0)
	{
		int range = ORANGE_RANGE_COL;
		//��ȡROI
		//ROI�����yֵ��ͬʱҲ�Ǽ�������y�����OFFSET
		//int offsetY = LeftBorder[0].y - ORANGE_RANGE_COL;
		//if (offsetY < 0) offsetY = 0;
		//ROI�б߽���ҵ�����и�
		int centerY = LeftBorder[0].y;
		bool needReFind = false;//�Ը����Ƿ���Ҫ����range��������

		for (size_t x = 0; (x + 3) < (*mdi).width; x += COL_SPAN)
		{
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				cv::circle(drowDebugDetectUD, cv::Point(x, centerY), 5, cv::Scalar(0, 255, 255), 3);
			}
#endif
			//���������ͼ���Yֵ
			int y1 = GetEdgeUpx3(cv::Point(x, centerY), range);
			if (y1 >= 0)
			{
				uchar* lineheadRGB = (*s).NowBufferImg.ptr<uchar>(y1);//ÿ�е���ʼ��ַ

				lineheadRGB[x * 3 + 0] = 0;
				lineheadRGB[x * 3 + 1] = 0;
				lineheadRGB[x * 3 + 2] = 255;

#ifdef OUTPUT_DEBUG_INFO
				if (OUTPUT_DEBUG_INFO)
				{
					cv::circle(drowDebugResult, cv::Point(x, y1), 5, cv::Scalar(0, 255, 255), 1);
					cv::circle(drowDebugResult, cv::Point(x, y1), 1, cv::Scalar(255, 255, 0), -1);
				}
#endif

				UpBorder.push_back(cv::Point(x, y1));
				needReFind = true;
				//����ģ��Ԥ����һ�����y����
				if (UpBorder.size() > 2)
				{
					int last_1 = UpBorder[UpBorder.size() - 1].y;
					int last_2 = UpBorder[UpBorder.size() - 2].y;
					centerY = last_1 + last_1 - last_2;
				}
				else
					centerY = y1;
				if (range > RANGE_MINI) range -= RANGE_REDUCE_BY;
				if (range < RANGE_MINI) range = RANGE_MINI;
			}
			else
			{
				if (needReFind)//�Ƿ�Ҫ����ɨ�����
				{
					range = ORANGE_RANGE_ROW;
					x -= COL_SPAN;
					needReFind = false;
				}
			}
		}

		if (UpBorder.size() > 0)
		{
			int upFirstCol = UpBorder[0].x, upFirstRow = UpBorder[0].y;
			int upLastCol = UpBorder[UpBorder.size() - 1].x, upLastRow = UpBorder[UpBorder.size() - 1].y;
			cv::Point leftstart = cv::Point((upFirstCol > COL_SPAN) ? (upFirstCol - COL_SPAN) : 0, -1);
			cv::Point leftend = cv::Point(upFirstCol, upFirstRow);
			GetEdgeHorizontalApproach(leftstart, leftend, RANGE_MINI, true, leftstart);
			cv::Point rightstart = cv::Point(upLastCol, upLastRow);
			cv::Point rightend = cv::Point((upLastCol + COL_SPAN) > (*mdi).width ? ((*mdi).width - 1) : (upLastCol + COL_SPAN), -1);
			GetEdgeHorizontalApproach(rightstart, rightend, RANGE_MINI, false, rightend);
		}
	}

	//�����±�Ե
	if (LeftBorder[LeftBorder.size() - 1].y > 0 && LeftBorder[LeftBorder.size() - 1].y < ((*mdi).MaxPics - SUM_COUNT) && RightBorder[RightBorder.size() - 1].y>0 && RightBorder[RightBorder.size() - 1].y < ((*mdi).MaxPics - SUM_COUNT))
	{
		int range = ORANGE_RANGE_COL;
		//��ȡROI
		//ROI�����yֵ��ͬʱҲ�Ǽ�������y�����OFFSET
		//int offsetY = LeftBorder[0].y - ORANGE_RANGE_COL;
		//if (offsetY < 0) offsetY = 0;
		//ROI�б߽���ҵ�����и�
		int centerY = LeftBorder[LeftBorder.size() - 1].y;
		bool needReFind = false;//�Ը����Ƿ���Ҫ����range��������

		for (size_t x = 0; (x + 3) < (*mdi).width; x += COL_SPAN)
		{
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				cv::circle(drowDebugDetectUD, cv::Point(x, centerY), 5, cv::Scalar(0, 255, 255), 3);
			}
#endif
			//���������ͼ���Yֵ
			int y1 = GetEdgeDownx3(cv::Point(x, centerY), range);
			if (y1 >= 0)
			{
				uchar* lineheadRGB = (*s).NowBufferImg.ptr<uchar>(y1);//ÿ�е���ʼ��ַ

				lineheadRGB[x * 3 + 0] = 0;
				lineheadRGB[x * 3 + 1] = 0;
				lineheadRGB[x * 3 + 2] = 255;

#ifdef OUTPUT_DEBUG_INFO
				if (OUTPUT_DEBUG_INFO)
				{
					cv::circle(drowDebugResult, cv::Point(x, y1), 5, cv::Scalar(0, 255, 255), 1);
					cv::circle(drowDebugResult, cv::Point(x, y1), 1, cv::Scalar(255, 255, 0), -1);
				}
#endif

				DownBorder.push_back(cv::Point(x, y1));
				needReFind = true;
				//����ģ��Ԥ����һ�����y����
				if (DownBorder.size() > 2)
				{
					int last_1 = DownBorder[DownBorder.size() - 1].y;
					int last_2 = DownBorder[DownBorder.size() - 2].y;
					centerY = last_1 + last_1 - last_2;
				}
				else
					centerY = y1;
				if (range > RANGE_MINI) range -= RANGE_REDUCE_BY;
				if (range < RANGE_MINI) range = RANGE_MINI;
			}
			else
			{
				if (needReFind)//�Ƿ�Ҫ����ɨ�����
				{
					range = ORANGE_RANGE_ROW;
					x -= COL_SPAN;
					needReFind = false;
				}
			}
		}

		if (DownBorder.size() > 0)
		{
			int upFirstCol = DownBorder[0].x, upFirstRow = DownBorder[0].y;
			int upLastCol = DownBorder[DownBorder.size() - 1].x, upLastRow = DownBorder[DownBorder.size() - 1].y;
			cv::Point leftstart = cv::Point((upFirstCol > COL_SPAN) ? (upFirstCol - COL_SPAN) : 0, -1);
			cv::Point leftend = cv::Point(upFirstCol, upFirstRow);
			GetEdgeHorizontalApproach(leftstart, leftend, RANGE_MINI, true, leftstart);
			cv::Point rightstart = cv::Point(upLastCol, upLastRow);
			cv::Point rightend = cv::Point((upLastCol + COL_SPAN) > (*mdi).width ? ((*mdi).width - 1) : (upLastCol + COL_SPAN), -1);
			GetEdgeHorizontalApproach(rightstart, rightend, RANGE_MINI, false, rightend);
		}
	}


	std::sort(UpBorder.begin(), UpBorder.end(), ORDER_BY_X_ASC);
	std::sort(DownBorder.begin(), DownBorder.end(), ORDER_BY_X_ASC);







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

int BlocksDetector::DetectOneLineLeft(int y)
{
	if (y == -1)
		y = leftY;
	//�ж��Ƿ�Խ��
	if (y < 0 || (y + 3 + 1) >(*mdi).MaxPics)
		return -1;
#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		cv::circle(drowDebugDetectLR, cv::Point(leftX, y), 5, cv::Scalar(0, 255, 255), 3);
	}
#endif
	int x1 = GetEdgeLeftx3(cv::Point(leftX, y), leftRnage);
	if (x1 >= 0)
	{
		//�����Ԥ�ⷶΧ֮���ҵ��˱߽磬����Ϊ�ҵ��������ĵ㣬������Ϊ���ɡ�
		if (leftRnage == RANGE_MINI)
		{
			if (LeftBorder.size() == 0)
			{
				LeftBorder.insert(LeftBorder.end(), tmpLeftList.begin(), tmpLeftList.end());
				tmpLeftList.clear();
			}
			LeftBorder.push_back(cv::Point(x1, y));
			allLeftList.push_back(cv::Point(x1, y));
		}
		else
		{
			tmpLeftList.push_back(cv::Point(x1, y));
			allLeftList.push_back(cv::Point(x1, y));
		}
		leftNoneCount = 0;
		leftNeedReFind = true;

		//����ģ��Ԥ����һ�����x����
		if (allLeftList.size() > 2)
		{
			int last_1 = allLeftList[allLeftList.size() - 1].x;
			int last_2 = allLeftList[allLeftList.size() - 2].x;
			leftX = last_1 + last_1 - last_2;
		}
		else
			leftX = x1;
		if (leftRnage > RANGE_MINI) leftRnage -= RANGE_REDUCE_BY;
		if (leftRnage < RANGE_MINI) leftRnage = RANGE_MINI;
	}
	else if (LeftBorder.size() > 0)
		leftNoneCount++;
	return x1;
}
int BlocksDetector::DetectOneLineRight(int y)
{
	//�ж��Ƿ�Խ��
	if (y < 0 || (y + 3 + 1) >(*mdi).MaxPics)
		return -1;
#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		cv::circle(drowDebugDetectLR, cv::Point(rightX, y), 5, cv::Scalar(0, 255, 255), 3);
	}
#endif
	int x1 = GetEdgeRightx3(cv::Point(rightX, y), rightRnage);
	if (x1 >= 0)
	{
		//�����Ԥ�ⷶΧ֮���ҵ��˱߽磬����Ϊ�ҵ��������ĵ㣬������Ϊ���ɡ�
		if (rightRnage == RANGE_MINI)
		{
			if (RightBorder.size() == 0)
			{
				RightBorder.insert(RightBorder.end(), tmpRightList.begin(), tmpRightList.end());
				tmpRightList.clear();
			}
			RightBorder.push_back(cv::Point(x1, y));
			allRightList.push_back(cv::Point(x1, y));
		}
		else
		{
			tmpRightList.push_back(cv::Point(x1, y));
			allRightList.push_back(cv::Point(x1, y));
		}

		rightNoneCount = 0;
		rightNeedReFind = true;

		//����ģ��Ԥ����һ�����x����
		if (allRightList.size() > 2)
		{
			int last_1 = allRightList[allRightList.size() - 1].x;
			int last_2 = allRightList[allRightList.size() - 2].x;
			rightX = last_1 + last_1 - last_2;
		}
		else
			rightX = x1;
		if (rightRnage > RANGE_MINI) rightRnage -= RANGE_REDUCE_BY;
		if (rightRnage < RANGE_MINI) rightRnage = RANGE_MINI;
	}
	else if (RightBorder.size() > 0)
		rightNoneCount++;
	return x1;
}

int BlocksDetector::GetEdgeLeftx3(cv::Point start, int range)
{
	int a = BlocksDetector::GetEdgeVertical(start, range, true);
	int b = BlocksDetector::GetEdgeVertical(cv::Point(start.x, start.y + 1), range, true);
	int c = BlocksDetector::GetEdgeVertical(cv::Point(start.x, start.y + 1), range, true);
	if ((a + b + c) > 0 && abs((a + c - b - b)) < 5)
		return ((a + b + c) / 3);
	return -1;
}

int BlocksDetector::GetEdgeRightx3(cv::Point start, int range)
{
	int a = GetEdgeVertical(start, range,false);
	int b = GetEdgeVertical(cv::Point(start.x, start.y + 1), range, false);
	int c = GetEdgeVertical(cv::Point(start.x, start.y + 1), range, false);
	if ((a + b + c) > 0 && abs((a + c - b - b)) < 5)
		return ((a + b + c) / 3);
	return -1;
} 

int BlocksDetector::GetEdgeUpx3(cv::Point start, int range)
{
	int a = GetEdgeHorizontal(start, range,true);
	int b = GetEdgeHorizontal(cv::Point(start.x + 1, start.y), range,true);
	int c = GetEdgeHorizontal(cv::Point(start.x + 1, start.y), range,true);
	if ((a + b + c) > 0 && abs((a + c - b - b)) < 5)
		return ((a + b + c) / 3);
	return -1;
}

int BlocksDetector::GetEdgeDownx3(cv::Point start, int range)
{
	int a = GetEdgeHorizontal(start, range, false);
	int b = GetEdgeHorizontal(cv::Point(start.x + 1, start.y), range, false);
	int c = GetEdgeHorizontal(cv::Point(start.x + 1, start.y), range, false);
	if ((a + b + c) > 0 && abs((a + c - b - b)) < 5)
		return ((a + b + c) / 3);
	return -1;
}


int BlocksDetector::GetEdgeVerticalApproach(cv::Point start, cv::Point end, int range, bool isUp, cv::Point Target)
{
	int x1 = -1;
	int startline = start.y;
	int endline = end.y;
	int middleLine = (endline - startline) / 2 + startline;
	//���ַ�����startline��endline���м��Ƿ��б߽�
	//�ݹ�ֱ��startline = endline
	//���ϱ�Ե����
	if (start.x == -1)
	{
		//���ҵ��ĵ������Ŀ����غϣ����������������
		if (middleLine == Target.y && middleLine != 0 && middleLine < ((*mdi).MaxPics- 2))
		{
			Target = cv::Point(-1, (Target.y > ROW_SPAN) ? (Target.y - ROW_SPAN) : 0);
			return GetEdgeVerticalApproach(Target, end, range, isUp, Target);
		}
		if (middleLine == startline || middleLine == endline)
			return end.x;
		if (isUp)
			x1 = GetEdgeLeftx3(cv::Point(end.x, middleLine), range);
		else
			x1 = GetEdgeRightx3(cv::Point(end.x, middleLine), range);
		if (x1 > 0)
		{
			if (isUp)
				LeftUp.push_back(cv::Point(x1, middleLine));
			else
				RightUp.push_back(cv::Point(x1, middleLine));
			x1 = GetEdgeVerticalApproach(start, cv::Point(x1, middleLine), range, isUp, Target);
		}
		else
			x1 = GetEdgeVerticalApproach(cv::Point(x1, middleLine), end, range, isUp, Target);
	}
	//���±�Ե����
	else
	{
		//���ҵ��ĵ������Ŀ����غϣ����������������
		if (middleLine == Target.y)
		{
			Target = cv::Point(-1, (Target.y + ROW_SPAN) > (*mdi).MaxPics ? ((*mdi).MaxPics - 1) : (Target.y + ROW_SPAN));
			return GetEdgeVerticalApproach(start, Target, range, isUp, Target);
		}
		if (middleLine == startline || middleLine == endline)
			return start.x;
		if (isUp)
			x1 = GetEdgeLeftx3(cv::Point(start.x, middleLine), range);
		else
			x1 = GetEdgeRightx3(cv::Point(start.x, middleLine), range);
		if (x1 > 0)
		{
			if (isUp)
				LeftDown.push_back(cv::Point(x1, middleLine));
			else
				RightDown.push_back(cv::Point(x1, middleLine));
			x1 = GetEdgeVerticalApproach(cv::Point(x1, middleLine), end, range, isUp, Target);
		}
		else
			x1 = GetEdgeVerticalApproach(start, cv::Point(-1, middleLine), range, isUp, Target);
	}

#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		cv::circle(drowDebugDetectLR, cv::Point(end.x, middleLine), 3, cv::Scalar(255, 0, 0), 2);
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

	return x1;
}

int BlocksDetector::GetEdgeVertical(cv::Point start, int range, bool isLeft)
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
		lineheadO = drowDebugDetectLR(cv::Rect(xstart, start.y, width, 1)).ptr<uchar>(0);//ÿ�е���ʼ��ַ
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
		int c = 0;
		if (isLeft)
			c = rightsum - leftsum;
		else
			c = leftsum - rightsum;
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


int BlocksDetector::GetEdgeHorizontalApproach(cv::Point start, cv::Point end, int range, bool isLeft, cv::Point Target)
{
	int y1 = -1;
	int startCol = start.x;
	int endCol = end.x;
	int middleCol = (endCol - startCol) / 2 + startCol;
	//�ƽ���ߣ���ʱendΪ���ҵ��ĵ�
	if (start.y == -1)
	{
		if (middleCol == startCol || middleCol == endCol)
			return end.y;
		if (isLeft)
			y1 = GetEdgeUpx3(cv::Point(middleCol, end.y), range);
		else
			y1 = GetEdgeDownx3(cv::Point(middleCol, end.y), range);
		if (y1 > 0)
		{
			UpBorder.push_back(cv::Point(middleCol, y1));
			y1 = GetEdgeHorizontalApproach(start, cv::Point(middleCol, y1), range, isLeft, Target);
		}
		else
			y1 = GetEdgeHorizontalApproach(cv::Point(middleCol, y1), end, range, isLeft, Target);
	}
	//�ƽ��ұߣ���ʱstartΪ�Ѿ��ҵ��ĵ�
	else
	{
		//���ҵ��ĵ������Ŀ����غϣ����������������
		if (middleCol == Target.x)
		{
			Target = cv::Point((Target.x + COL_SPAN + 1) > (*mdi).width ? ((*mdi).width - 1) : Target.x + COL_SPAN, -1);
			return GetEdgeHorizontalApproach(start, Target, range, isLeft, Target);
		}
		if (middleCol == startCol || middleCol == endCol)
			return start.y;
		if (isLeft)
			y1 = GetEdgeUpx3(cv::Point(middleCol, start.y), range);
		else
			y1 = GetEdgeDownx3(cv::Point(middleCol, start.y), range);
		if (y1 > 0)
		{
			UpBorder.push_back(cv::Point(middleCol, y1));
			y1 = GetEdgeHorizontalApproach(cv::Point(middleCol, y1), end, range, isLeft, Target);
		}
		else
			y1 = GetEdgeHorizontalApproach(start, cv::Point(middleCol, y1), range, isLeft, Target);
	}
#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		cv::circle(drowDebugDetectUD, cv::Point(middleCol, end.y), 3, cv::Scalar(255, 0, 0), 2);
		if (y1 > 0)
		{
			cv::circle(drowDebugResult, cv::Point(middleCol, y1), 3, cv::Scalar(255, 255, 0), 1);
			cv::circle(drowDebugResult, cv::Point(middleCol, y1), 1, cv::Scalar(255, 0, 255), -1);
		}
	}
#endif
	return y1;
}
int BlocksDetector::GetEdgeHorizontal(cv::Point start, int range, bool isUp)
{
	cv::Mat oneLineGray;
	const int sumcount = SUM_COUNT;
	if (range <= sumcount)
		range = sumcount + 1;
	//ȷ��ROI��Χ
	int ystart = start.y - range;
	if (ystart < 0)
		ystart = 0;
	int height = range + range;
	if ((height + ystart) >((*s).NowBufferImg.rows - 1))
		height = ((*s).NowBufferImg.rows - 1) - ystart;

	cv::cvtColor((*s).NowBufferImg(cv::Rect(start.x, ystart, 1, height)), oneLineGray, CV_BGR2GRAY);

	int ret = -1;
	int diff = SUM_THRESHOD;
	cv::Mat drowROI;
#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		drowROI = drowDebugDetectUD(cv::Rect(start.x, ystart, 1, height));
	}
#endif
	for (size_t y = sumcount; y < height - sumcount; y++)
	{
		int upsum = 0;
		for (size_t yy = y; yy > y - sumcount; yy--)
		{
			upsum += oneLineGray.ptr<uchar>(yy)[0];
		}
		int downsum = 0;
		for (size_t yy = y + 1; yy <= y + sumcount; yy++)
		{
			downsum += oneLineGray.ptr<uchar>(yy)[0];
		}
		int c = 0;
		if (isUp)
			c = (downsum - upsum);
		else
			c = (upsum - downsum);
		if (c > diff)
		{
			diff = c;
			ret = y;
#ifdef OUTPUT_DEBUG_INFO
			if (OUTPUT_DEBUG_INFO)
			{
				drowROI.ptr<uchar>(y)[0] = 255;//�����ɫ
			}
#endif
		}
#ifdef OUTPUT_DEBUG_INFO
		if (OUTPUT_DEBUG_INFO)
		{
			drowROI.ptr<uchar>(y)[1] = 255;//�����ɫ
		}
#endif
	}
	if (ret >= 0)
	{
#ifdef OUTPUT_DEBUG_INFO
		if (OUTPUT_DEBUG_INFO)
		{
			drowROI.ptr<uchar>(ret)[2] = 255;//����ɫ
		}
#endif
		ret += ystart;
	}
	return ret;
}