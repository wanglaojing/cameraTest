// HelloWorldFreeRunLine.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "globle.h"
#include "Class\MicroDisplay\MicroDisplayInit.h"
#include "Class\MicroDisplay\MicroDisplayControler.h"
#include "Class\BufferStorage.h"
#include "Class\VirtualCamera.h"
#include "Class\BlocksDetector.h"
#include <thread>


BufferStorage s;
int status = 0;
MicroDisplayInit mdi;
VirtualCamera vc;

const bool USING_VIRTUAL_CAMERA = true;//�Ƿ�ʹ����������ͷ 1ʹ�� 0��E2V

bool producerEndFlag = false, customerEndFlag = false;


//������
void producer()
{
	double t = (double)cv::getTickCount();

	if (!USING_VIRTUAL_CAMERA)
	{
		if (MicroDisplayControler::FreeRunning(mdi, s) < 0)
		{
			ErrorMessageWait(mdi.fg);
			return;
		}
	}
	else
	{
		vc.FreeRunning(mdi, s);
	}
	//������ʱ
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << mdi.width << "x" << mdi.MaxPics << "��" << t << endl;

	//��������߹�������
	producerEndFlag = true;
}
void customerWork1()
{

	double t = (double)cv::getTickCount();

	vector<int> leftEdgeX;
	map<int, int> leftEdgeY;
	vector<int> rightEdgeX;
	map<int, int> rightEdgeY;
	const int linespan = 200;

	for (size_t i = 0; i < mdi.MaxPics; i += linespan)
	{
		int x1 = BlocksDetector::GetEdgeLeft(i, 400, s);
		int x2 = BlocksDetector::GetEdgeRight(i, mdi.width - 400, s);
		if (x1 >= 0 || x2 >= 0)
		{
			cv::Mat oneLine = s.NowBufferImg(cv::Rect(0, i, mdi.width, 1));
			uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ
			if (x1 >= 0)
			{
				lineheadRGB[x1 * 3 + 0] = 0;
				lineheadRGB[x1 * 3 + 1] = 0;
				lineheadRGB[x1 * 3 + 2] = 255;
				leftEdgeX.push_back(i);
				leftEdgeY.insert({ i, x1 });
			}
			if (x2 >= 0)
			{
				lineheadRGB[x2 * 3 + 0] = 255;
				lineheadRGB[x2 * 3 + 1] = 0;
				lineheadRGB[x2 * 3 + 2] = 0;
				rightEdgeX.push_back(i);
				rightEdgeY.insert({ i, x2 });
			}
		}
	}
	int leftfirst = leftEdgeX[0], leftlast = leftEdgeX[leftEdgeX.size() - 1];
	//�����ͷ
	for (size_t i = (leftfirst > linespan) ? (leftfirst - linespan) : 0; i < leftfirst; i++)
	{
		int x1 = BlocksDetector::GetEdgeLeft(i, 400, s);
		if (x1 >= 0)
		{
			cv::Mat oneLine = s.NowBufferImg(cv::Rect(0, i, mdi.width, 1));
			uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ
			lineheadRGB[x1 * 3 + 0] = 0;
			lineheadRGB[x1 * 3 + 1] = 0;
			lineheadRGB[x1 * 3 + 2] = 255;
			leftEdgeX.push_back(i);
			leftEdgeY.insert({ i, x1 });
		}
	}
	//�����β
	int tmp = (leftlast + linespan) > mdi.MaxPics ? (mdi.MaxPics - 1) : (leftlast + linespan);
	for (size_t i = leftlast + 1; i < tmp; i++)
	{
		int x1 = BlocksDetector::GetEdgeLeft(i, 400, s);
		if (x1 >= 0)
		{
			cv::Mat oneLine = s.NowBufferImg(cv::Rect(0, i, mdi.width, 1));
			uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ

			lineheadRGB[x1 * 3 + 0] = 0;
			lineheadRGB[x1 * 3 + 1] = 0;
			lineheadRGB[x1 * 3 + 2] = 255;
			leftEdgeX.push_back(i);
			leftEdgeY.insert({ i, x1 });
		}
	}


	int rightfirst = rightEdgeX[0], rightlast = rightEdgeX[rightEdgeX.size() - 1];
	//�ұ���ͷ
	for (size_t i = (rightfirst > linespan) ? (rightfirst - linespan) : 0; i < rightfirst; i++)
	{
		int x2 = BlocksDetector::GetEdgeRight(i, mdi.width - 400, s);
		if (x2 >= 0)
		{
			cv::Mat oneLine = s.NowBufferImg(cv::Rect(0, i, mdi.width, 1));
			uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ
			lineheadRGB[x2 * 3 + 0] = 255;
			lineheadRGB[x2 * 3 + 1] = 0;
			lineheadRGB[x2 * 3 + 2] = 0;
			rightEdgeX.push_back(i);
			rightEdgeY.insert({ i, x2 });
		}
	}
	//�ұ���β
	int tmp2 = (rightlast + linespan) > mdi.MaxPics ? (mdi.MaxPics - 1) : (rightlast + linespan);
	for (size_t i = rightlast + 1; i < tmp2; i++)
	{
		int x2 = BlocksDetector::GetEdgeRight(i, mdi.width - 400, s);
		if (x2 >= 0)
		{
			cv::Mat oneLine = s.NowBufferImg(cv::Rect(0, i, mdi.width, 1));
			uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ

			lineheadRGB[x2 * 3 + 0] = 255;
			lineheadRGB[x2 * 3 + 1] = 0;
			lineheadRGB[x2 * 3 + 2] = 0;
			rightEdgeX.push_back(i);
			rightEdgeY.insert({ i, x2 });
		}
	}

	sort(leftEdgeX.begin(), leftEdgeX.end());
	sort(rightEdgeX.begin(), rightEdgeX.end());

	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "�ǲ��д�����ʱ��" << t << endl;

}
//������
void customer()
{
	//��ʼ��ʱ
	double t = (double)cv::getTickCount();

	//�����㷨

	//���˵ڼ���
	int i = 0;
	//״̬���0��ʾ������-1��ʾ��Ҫ�ȴ���һ֡д��
	int flag = 0;
	do{
		if (s.EndWriteFlag)
			break;
		cv::Mat f;
		flag = s.GetFrame(f);
		if (flag == -1)
		{
			Sleep(1);
			continue;
		}
		if (flag == 0)
			break;

		//����㷨
		cv::Mat oneLine = s.NowBufferImg(cv::Rect(0, i, mdi.width, 1));
		int elementCount = mdi.width;//ÿ��Ԫ����
		uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ

		int x1 = BlocksDetector::GetEdgeLeft(i, 400, s);
		if (x1 >= 0)
		{
			lineheadRGB[x1 * 3 + 0] = 0;
			lineheadRGB[x1 * 3 + 1] = 0;
			lineheadRGB[x1 * 3 + 2] = 255;
		}
		int x2 = BlocksDetector::GetEdgeRight(i, elementCount - 400, s);
		if (x2 >= 0)
		{
			lineheadRGB[x2 * 3 + 0] = 255;
			lineheadRGB[x2 * 3 + 1] = 0;
			lineheadRGB[x2 * 3 + 2] = 0;
		}
		i++;
	} while (flag != 0);
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "���д�����ʱ��" << t << endl;




	customerWork1();

	//��������߹�������
	customerEndFlag = true;
}

int main()
{
	mdi.colorType = mdi.GRAY;
	mdi.width = 4096;
	mdi.height = 1;
	mdi.MaxPics = 10000;//�ɼ�����֡ͼ��
	s = BufferStorage(mdi);

	if (!USING_VIRTUAL_CAMERA)
	{
		//��ʼ���ɼ���
		status = MicroDisplayInit::InitParameter(mdi);
		if (status < 0)
		{
			ErrorMessageWait(mdi.fg);
			return -1;
		}
		//MicroDisplayInit::CreateBufferWithOutDiplay(mdi);
		MicroDisplayInit::CreateBufferWithDiplay(mdi);
	}
	else
	{
		//��ʼ���������
		vc = VirtualCamera(mdi, "��Ʒ2_o.jpg");
	}




	int grabbingIndex = 0;
	//��ѭ��
	string input;
	do{
		std::cout << "����1��ʼ��ͼ��2������ս������ţ�q�˳���";
		std::cin >> input;
		
		if (input == "1")
		{
			grabbingIndex += 1;
			if (grabbingIndex > 1000) grabbingIndex = 1;

			//����

			//��ʼ������
			s.Start();

			std::thread t1(producer);
			t1.join();

			std::thread t2(customer);
			t2.join();

			//�ȴ������̴߳������
			while (!customerEndFlag || !producerEndFlag)
			{
				Sleep(10);
			}

			string p1;
			stringstream ss1;
			ss1 << "samples/result" << grabbingIndex << "_o.jpg";
			ss1 >> p1;
			string p2;
			stringstream ss2;
			ss2 << "samples/result" << grabbingIndex << "_x3.jpg";
			ss2 >> p2;
			cv::imwrite(p1, s.NowBuffer);
			//cv::imwrite("result1.jpg", s.NowBufferGray);
			cv::imwrite(p2, s.NowBufferImg);
		}
		if (input == "2")
		{
			//��������ͼ��
			MicroDisplayInit mditmp;
			mditmp.MaxPics = 10;
			mditmp.colorType = mdi.colorType;
			mditmp.width = mdi.width;
			mditmp.height = mdi.height;
			VirtualCamera vctmp = VirtualCamera(mditmp, "����У��_o.jpg");
			BufferStorage stmp = BufferStorage(mditmp);
			stmp.Start();
			if (!USING_VIRTUAL_CAMERA)
			{
				if (MicroDisplayControler::FreeRunning(mditmp, stmp) < 0)
				{
					ErrorMessageWait(mditmp.fg);
					return 0;
				}
			}
			else
			{
				vctmp.FreeRunning(mditmp, stmp);
			}
			//cv::Mat img = stmp.NowBufferImg;

			//X - ����������ʱֱ����Ӽ���
			cv::Mat SamplesRGBTMP = stmp.NowBufferImg(cv::Rect(0, 2, mditmp.width, 1)).clone() / 3;
			cv::imwrite("samples/����У������.jpg", SamplesRGBTMP);
			//RGB
			cv::Mat SamplesRGB = cv::Mat(mditmp.height, mditmp.width, CV_8UC3, cv::Scalar(50, 50, 50));
			SamplesRGB = SamplesRGB - SamplesRGBTMP;
			SamplesRGB.copyTo(mdi.SamplesRGB);
			//Gray
			cv::cvtColor(SamplesRGB, mdi.SamplesGray, CV_BGR2GRAY);
			cv::Mat SamplesGray = mdi.SamplesGray;
			std::cout << "OK\r\n";

			//���³�ʼ������
			s = BufferStorage(mdi);
		}
		else
			Sleep(10);
	} while (input != "q");

	MicroDisplayInit::Release(mdi);
	return 0;
}