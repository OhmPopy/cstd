//////////////////////////////////////////////////////////////////
//		���� : crazybit@263.net									//
//		��ҳ : http://crazybit.topcities.com/					//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef __FOO_HEADER_H_
#define __FOO_HEADER_H_
#pragma once

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "vfw32.lib")
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "msimg32.lib") // AlphaBlend func.
#pragma comment (lib, "zlib.lib")
#pragma comment (lib, "png.lib")
#pragma comment (lib, "Jpeg.lib")
#pragma comment (lib, "Tiff.lib")
//#pragma comment (lib, "flib.lib")		// To Use FLibrary

//=============================================================================

#include ".\Include\StdDefine.h"
#include ".\Include\fooBit.h"

//	����Win32����
#include ".\Include\fooSys.h"
#include ".\Include\C_x86_CPU.h"
#include ".\Include\C_Process.h"
#include ".\Include\C_Memory.h"
#include ".\Include\C_Disk.h"
#include ".\Include\C_File.h"
#include ".\Include\C_Network.h"

//	һЩ��
#include ".\Include\TrayIcon.h"
#include ".\Include\FileFind.h"
#include ".\Include\Folder.h"
#include ".\Include\MemMapFile.h"
#include ".\Include\ToolHelp.h"
#include ".\Include\Reg.h"
#include ".\Include\Time.h"

//	ѹ���㷨
#include ".\Include\Compress\Huffman.h"
#include ".\Include\Compress\Rle.h"
#include ".\Include\Compress\Lzw.h"

//	ͼ����
#include ".\Include\Pic\Bitmap.h"
#include ".\Include\Pic\Pic.h"
#include ".\Include\Pic\Effect.h"
#include ".\Include\Pic\Image.h"

//#include ".\WinApi\fooWave.h"

#endif