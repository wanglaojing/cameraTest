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

<<<<<<< HEAD
const bool USING_VIRTUAL_CAMERA = false;//�Ƿ�ʹ����������ͷ 1ʹ�� 0��E2V
=======
const int USING_VIRTUAL_CAMERA = 1;//�Ƿ�ʹ����������ͷ
>>>>>>> bf7d178fc64e70f6838b90186d71fbcb666d6cc4

int main()
{
	mdi.colorType = mdi.GRAY;
	mdi.width = 4096;
	mdi.height = 1;
	mdi.MaxPics = 10000;//�ɼ�����֡ͼ��
	s = BufferStorage(mdi.width, mdi.MaxPics);

	if (!USING_VIRTUAL_CAMERA)
<<<<<<< HEAD
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
		vc = VirtualCamera(mdi);
	}
=======
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
		vc = VirtualCamera(mdi);
	}
>>>>>>> bf7d178fc64e70f6838b90186d71fbcb666d6cc4




	int grabbingIndex = 0;
	//��ѭ��
	while (true)
	{
		grabbingIndex += 1;
		if (grabbingIndex > 1000) grabbingIndex = 1;

		char input;
		std::cout << "����1��ʼ��ͼ��q�˳���";
		do{
			input = getchar();
			if (input == 'q')
				return 0;
			Sleep(10);
		} while (input != '1');

		//����

		//��ʼ������
		s.Start();


		double t = (double)cv::getTickCount();

		if (!USING_VIRTUAL_CAMERA)
		{
			if (MicroDisplayControler::FreeRunning(mdi, s) < 0)
			{
				ErrorMessageWait(mdi.fg);
				return -1;
			}
		}
		else
		{
			vc.FreeRunning(mdi, s);
		}
<<<<<<< HEAD
		//������ʱ
		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		cout << mdi.width << "x" << mdi.MaxPics << "��" << t << endl;
		//���¿�ʼ��ʱ
		t = (double)cv::getTickCount();
=======
>>>>>>> bf7d178fc64e70f6838b90186d71fbcb666d6cc4

		//�����㷨

		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		cout << "������ʱ��" << t << endl;

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
	MicroDisplayInit::Release(mdi);
	return 0;
}

