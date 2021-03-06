///全局头文件
#ifndef INCLUDE_ONCE
#define INCLUDE_ONCE
#pragma once

//头文件
#include <fgrab_struct.h>
#include <fgrab_prototyp.h>
#include <fgrab_define.h>
#include <SisoDisplay.h>


#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
//头文件END



//调试相关
//是否输出调试信息
#define OUTPUT_DEBUG_INFO 1
#include <Windows.h>
// 若宏定义配置为输出
// 则在控制台输出调试信息
// 函数位于 global.h
static void OutPutDebugInfo(string info){
#ifdef OUTPUT_DEBUG_INFO
	if (OUTPUT_DEBUG_INFO)
	{
		string l = "--------------------------------\n" + info + "\n--------------------------------\n";
		int dwLen = strlen(l.c_str()) + 1;
		int nwLen = MultiByteToWideChar(CP_ACP, 0, l.c_str(), dwLen, NULL, 0);//算出合适的长度
		LPWSTR lpsz = new WCHAR[dwLen];
		MultiByteToWideChar(CP_ACP, 0, l.c_str(), dwLen, lpsz, nwLen);

		OutputDebugString(lpsz);
	}
#endif
}




//全局函数
static int ErrorMessage(Fg_Struct *fg)
{
	int error = Fg_getLastErrorNumber(fg);
	const char*	err_str = Fg_getLastErrorDescription(fg);
	fprintf(stderr, "Error: %d : %s\n", error, err_str);
	return error;
}
static int ErrorMessageWait(Fg_Struct *fg)
{
	int error = ErrorMessage(fg);
	printf(" ... press ENTER to continue\n");
	getchar();
	return error;
}









#endif