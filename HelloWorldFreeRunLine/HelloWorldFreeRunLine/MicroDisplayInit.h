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

	int nr_of_buffer = 8;			// Number of memory buffer
	int nBoard = 0;			// Board Number
	int nCamPort = PORT_A;		// Port (PORT_A / PORT_B / PORT_C / PORT_D)
	int MaxPics = 10000;		// Number of images to grab
	int colorType = 0;
	enum ColorType
	{
		GRAY = 0,
		RGB = 1
	};
	// Setting the image size
	int width = 4096;
	int height = 1;



	static int Init(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);
	static int InitLoad(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);//���������ļ���ʼ��
	static int InitParameter(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);//������ʼ��
	static int CreateDiplay(MicroDisplayInit& mdi, Fg_Struct **fg, dma_mem **pMem0);//������ʼ��


	static int getNoOfBitsFromImageFormat(const int format);
};

