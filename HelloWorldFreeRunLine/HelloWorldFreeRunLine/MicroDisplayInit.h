#pragma once
#include "globle.h"


#define CONFIG_FILENAME "anOriginalConfigFileName.mcf"

// ���������ʼ���࣬�洢����趨����������ʼ���ɼ��������
class MicroDisplayInit
{
private:
	static int getNrOfBoards();
	static int getBoardInfo();
public:
	const char *dllNameGRAY = "DualLineGray16.dll";
	const char *dllNameRGB = "DualLineRGB30.dll";

	// Number of memory buffer
	int nr_of_buffer = 8;
	// Board Number
	int nBoard = 0;
	// Port (PORT_A / PORT_B / PORT_C / PORT_D)
	int nCamPort = PORT_A;
	// Number of images to grab
	int MaxPics = 10000;
	int colorType = 0;
	// ��ͼ����ID
	int nId = -1;

	enum ColorType
	{
		GRAY = 0,
		RGB = 1
	};
	// Setting the image size
	int width = 4096;
	int height = 1;



	static int Init(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);
	//���������ļ���ʼ��
	static int InitLoad(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);
	//������ʼ��
	static int InitParameter(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);
	//�����ڴ�  ��Ū����
	static void CreateBufferWithOutDiplay(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);
	//�����ڴ�����ӻ�����
	static void CreateBufferWithDiplay(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);
	//��ʼ�ɼ�
	static int StartGrabbing(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);
	//�����ɼ�
	static void EndGrabbing(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);


	static int getNoOfBitsFromImageFormat(const int format);
};

