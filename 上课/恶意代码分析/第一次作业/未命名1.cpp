#include <tchar.h> 
#include <windows.h>  
#include <stdio.h>  
#include <TlHelp32.h>


wchar_t* AnsiToUnicode( const char* szStr )  
{  
    int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );  
    if (nLen == 0)  
    {  
        return NULL;  
    }  
    wchar_t* pResult = new wchar_t[nLen];  
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );  
    return pResult;  
}  
  
//将宽字节wchar_t*转化为单字节char*  
inline char* UnicodeToAnsi( const wchar_t* szStr )  
{  
    int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );  
    if (nLen == 0)  
    {  
        return NULL;  
    }  
    char* pResult = new char[nLen];  
    WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );  
    return pResult;  
}  

typedef struct tagWNDINFO
{
    DWORD dwProcessId;
    HWND hWnd;
} WNDINFO, *LPWNDINFO;



//2.枚举所有进程

BOOL CALLBACK MyEnumProc(HWND hWnd,LPARAM lParam)//枚举所有进程
{
    DWORD dwProcId;
    GetWindowThreadProcessId(hWnd, &dwProcId);
    LPWNDINFO pInfo=(LPWNDINFO)lParam;
    if(dwProcId==pInfo->dwProcessId)
    {
        pInfo->hWnd = hWnd;
        return FALSE;
    }

    return TRUE;
}



//3.获取给定进程ID的窗口handle

HWND GetProcessMainWnd(DWORD dwProcessId)//获取给定进程ID的窗口handle
{
    WNDINFO wi;
    wi.dwProcessId = dwProcessId;
    wi.hWnd = NULL;
    EnumWindows(MyEnumProc,(LPARAM)&wi);

    return wi.hWnd;
}

HWND GetWndHwnd(wchar_t *strExeName)
{
    PROCESSENTRY32 pe32;
    //在使用这个结构前，先设置它的大小  
    pe32.dwSize = sizeof(pe32);  
    //给系统内所有的进程拍个快照  
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  
    if (hProcessSnap == INVALID_HANDLE_VALUE)  
    {  
        //printf("CreateToolhelp32Snapshot 调用失败.\n");  
        return NULL;  
    }  
    //遍历进程快照，轮流显示每个进程的信息  ((((
    BOOL bMore = ::Process32First(hProcessSnap,&pe32);  
    while (bMore)  
    {  
        //if(pe32.szExeFile == exeName)//找到了进程
        if(_tcscmp(pe32.szExeFile,UnicodeToAnsi(strExeName)) == 0)
        // if(pe32.th32ProcessID==14668)
        {
            HWND game_window_hwnd=GetProcessMainWnd(pe32.th32ProcessID );
            if (NULL != game_window_hwnd)
            {
                ::CloseHandle(hProcessSnap); 
                return game_window_hwnd;
            }                        
        }
        bMore = ::Process32Next(hProcessSnap,&pe32);  
    }  
    //不要忘记清除掉snapshot对象  
    ::CloseHandle(hProcessSnap); 
    return NULL;
}
  
int main()  
{  
    //HWND hWinmine = FindWindow(NULL, "Minesweeper");
    HWND  hWinmine=GetWndHwnd(L"winmine.exe");
    DWORD dwPID = 0;  
    GetWindowThreadProcessId(hWinmine, &dwPID);  
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID);  
  
    //基础地址、雷数、宽度、高度  
    DWORD dwBaseAddr = 0x01005330,j;  
    DWORD dwNum = 0, dwWidth = 0, dwHight = 0, dwSize = 0;  
  
  
    //读取内存信息  
    //读进程的内存空间数据  
    //参数：第1个参数：    HANDLE hProcess进程句柄。  
    //第2个参数：    LPCVOID lpBaseAddress基址指针。  
    //第3个参数：    LPVOID lpBuffer接收数据缓冲区指针。  
    //第4个参数：    DWORD nSize接收数据缓冲区缓冲区大小。  
    //第5个参数：    LPDWORD lpNumberOfBytesRead读入数据量大小指针。  
  
  //      返回值：       成功：TRUE  
    //    失败：FALSE  
    ReadProcessMemory(hProcess, (LPVOID)dwBaseAddr, &dwNum, sizeof(DWORD), &dwSize);  
    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddr +0x4), &dwWidth, sizeof(DWORD), &dwSize);  
    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 0x8), &dwHight, sizeof(DWORD), &dwSize);  
  
    //棋盘总大小=棋盘+空白边+4角  
    DWORD dwReadsize = dwWidth*dwHight + dwHight * 2 + dwWidth * 2 + 4;  
    DWORD dwBoundAddr=dwReadsize+dwBaseAddr;
    PBYTE pByte = new BYTE[dwReadsize];  
      
    //ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 0x16), pByte, dwReadsize, &dwSize);  
  
    BYTE bClear = 0x8E;  
     BYTE bRead;  
    //for (size_t i = 0; i < dwReadsize; i++)  
    //{  
     /*   if (pByte[i] == 0x8F)  
        {  
            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddr + 0x16 +i), &bClear, sizeof(BYTE), &dwSize);  
        } */
        dwBaseAddr+=0x10;
        int i;
	 for(i = 0; i < 16; i++)  
    {  
        for(j = dwBaseAddr; j <dwBoundAddr; j++)  
        {  
            if(ReadProcessMemory(hProcess, (LPVOID)j, &bRead, 1, NULL))  
            {  
                printf("%x\n", bRead);  
                if(bRead == 0x8f)  
                {  
                    printf("flag\n");  
                    WriteProcessMemory(hProcess, (LPVOID)j, &bClear, 1, NULL);  
                }  
            }  
        }  
        dwBaseAddr = dwBaseAddr + 32;  
        dwBoundAddr = dwBoundAddr + 32;  
    }   
   // }  
    //函数功能：       得到窗体客户区的大小。  
    //第1个参数：    HWND hWnd窗体句柄。  
    //第2个参数：    LPRECT lpRect客户区RECT结构的指针。  
    RECT rt = { 0 };  
    GetClientRect(hWinmine, &rt);  
  
     bool f= KillTimer(hWinmine, 1); 
    InvalidateRect(hWinmine, &rt, true);//这个函数屏蔽一个窗口客户区的全部或部分区域。这会导致窗口在事件期间部分重画  
    delete pByte;  
    CloseHandle(hProcess);  
    getchar();  
    return 0;  
   /* DWORD processId;  
    HWND hWnd;  
    HANDLE hProcess;  
  
    DWORD dwAddrA;    
    DWORD dwAddrB;  
  
    int i;  
    DWORD j;  
  
    BYTE bRead;  
    BYTE bWrite = 0x8e;  
  
    dwAddrA = 0x1005340;  
    dwAddrB = 0x1005340 + 30;  
  
    //hWnd = FindWindow(NULL, "");  
    hWnd=GetWndHwnd(L"winmine.exe");
    if(hWnd == NULL)  
    {  
        printf("can not find window!\n");  
        return 0;  
    }  
    GetWindowThreadProcessId(hWnd, &processId);  
    printf("%d", processId);  
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);  
    if(hProcess == NULL)  
    {  
     int ret = GetLastError();
        // skip the system process and Idle process or one of CSRSS process
        if (ret != ERROR_INVALID_PARAMETER && ret != ERROR_ACCESS_DENIED) 
            return 0;  // function failed for other errors
        printf("error!\n");  
      //  while(1);
        return 0;  
    }  
  
    for(i = 0; i < 16; i++)  
    {  
        for(j = dwAddrA; j < dwAddrB; j++)  
        {  
            if(ReadProcessMemory(hProcess, (LPVOID)j, &bRead, 1, NULL))  
            {  
                printf("%x\n", bRead);  
                if(bRead == 0x8f)  
                {  
                    printf("flag\n");  
                    WriteProcessMemory(hProcess, (LPVOID)j, &bWrite, 1, NULL);  
                }  
            }  
        }  
        dwAddrA = dwAddrA + 32;  
        dwAddrB = dwAddrB + 32;  
    }  
  
    ShowWindow(hWnd, SW_MINIMIZE);  
    ShowWindow(hWnd, SW_SHOWNORMAL);  
  
    return 1;  */ 
}  