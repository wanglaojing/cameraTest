// HelloWorldFreeRunLine.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "globle.h"
#include "Class\MicroDisplay\MicroDisplayInit.h"
#include "Class\MicroDisplay\MicroDisplayControler.h"
#include "Class\BufferStorage.h"
#include "Class\VirtualCamera.h"
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
		cv::Mat oneLineGray;
		cv::cvtColor(oneLine, oneLineGray, CV_BGR2GRAY);

		int elementCount = mdi.width;//ÿ��Ԫ����
		uchar* linehead = oneLineGray.ptr<uchar>(0);//ÿ�е���ʼ��ַ
		uchar* lineheadRGB = oneLine.ptr<uchar>(0);//ÿ�е���ʼ��ַ
		int left = -1;//��ߴ�ש
		if (linehead[0] > 180)left = 0;
		int leftdiffer = 0;
		for (int j = 20; j < 700; j++)
		{
			int leftsum = 0;
			for (size_t ii = j; ii > j - 20; ii--)
			{
				leftsum += linehead[ii];
			}
			int rightsum = 0;
			for (size_t ii = j + 1; ii <= j + 20; ii++)
			{
				rightsum += linehead[ii];
			}

			if ((rightsum - leftsum) > 20 * 10 && (rightsum - leftsum) > leftdiffer)
			{
				leftdiffer = (rightsum - leftsum);
				left = j;
			}
		}
		if (left >= 0)
		{
			lineheadRGB[left * 3 + 0] = 0;
			lineheadRGB[left * 3 + 1] = 0;
			lineheadRGB[left * 3 + 2] = 255;
		}

		int right = -1;//�ұߴ�ש
		if (linehead[elementCount - 1] > 180)right = 0;
		int rightdiffer = 0;
		for (int j = elementCount - 20; j > elementCount - 720; j--)
		{
			int leftsum = 0;
			for (size_t ii = j; ii > j - 20; ii--)
			{
				leftsum += linehead[ii];
			}
			int rightsum = 0;
			for (size_t ii = j + 1; ii <= j + 20; ii++)
			{
				rightsum += linehead[ii];
			}

			if ((leftsum - rightsum) > 20 * 5 && (leftsum - rightsum) > rightdiffer)
			{
				rightdiffer = (leftsum - rightsum);
				right = j;
			}
		}
		if (right >= 0)
		{
			lineheadRGB[right * 3 + 0] = 0;
			lineheadRGB[right * 3 + 1] = 255;
			lineheadRGB[right * 3 + 2] = 0;
		}

		i++;
	} while (flag != 0);



	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "������ʱ��" << t << endl;


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