#pragma once
#include "globle.h"
#include "MicroDisplayInit.h"
class MicroDisplayStorage
{
public:
	MicroDisplayStorage(){};
	MicroDisplayStorage(int width, int length);
	~MicroDisplayStorage();


	int BufferWriteIndex = 0;
	int BufferReadIndex = 0;

	cv::Mat Buffer0;
	cv::Mat Buffer0Gray;
	cv::Mat Buffer0Img;
	//cv::Mat Buffer0Flag;//������������ʾĳһ֡״̬����ʼΪ0��д���Ϊ1����ȡ��Ϊ2

	cv::Mat Buffer1;
	cv::Mat Buffer1Gray;
	cv::Mat Buffer1Img;
	//cv::Mat Buffer1Flag;//������������ʾĳһ֡״̬����ʼΪ0��д���Ϊ1����ȡ��Ϊ2

	cv::Mat NowBuffer;//ָ��ǰ������Ļ�����Buffer0 �� Buffer1
	cv::Mat NowBufferGray;
	cv::Mat NowBufferImg;



	bool EndFlag = false;


	void Start();//��ʼ��һ�ֲɼ�������Read/Write Index = 0;
	bool AddFrame(cv::Mat& frame);//������ͷ����һ֡,�����Ƿ�ֹͣ�ɼ�
	bool AddFrame(cv::Mat& frame,int);//������ͷ����һ֡,�����Ƿ�ֹͣ�ɼ�
	int GetFrame(cv::Mat& frame);//�ӻ������һ֡����ֵ��frame,���� 0 ����,1 ����,-1��һ֡��δд�����
private:
	unsigned int bufferIndex = 0;//�жϵ�ǰ����ʹ����һ��buffer��ÿִ��һ��Start()����һ��Buffer
	int WIDTH = 4096;//��������ֱ���
	int LENGTH = 1000;//����ɼ�����
};

