#include <windows.h>
#include <lm.h> // Esta cabezera se incluye por NetWkstaGetInfo
#include <iostream>
#include "osver.h"

void Osver::getMinorAndMajor(POSVERSIONINFOEXW pVersions)
{
	LPBYTE dataRaw;
	DWORD status = NetWkstaGetInfo(NULL, 100, &dataRaw);
	if (status == NERR_Success)
	{
		WKSTA_INFO_100* pworkstationInfo = (WKSTA_INFO_100 *)dataRaw;
		pVersions->dwMajorVersion = pworkstationInfo->wki100_ver_major;
		pVersions->dwMinorVersion = pworkstationInfo->wki100_ver_minor;
		::NetApiBufferFree(dataRaw);
	}
}

BYTE Osver::_isEqualProductType(BYTE productType)
{
	OSVERSIONINFOEX osVersionInfo;
	::ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osVersionInfo.wProductType = productType;
	ULONGLONG maskCondition = ::VerSetConditionMask(0, VER_PRODUCT_TYPE, VER_EQUAL);
	return ::VerifyVersionInfo(&osVersionInfo, VER_PRODUCT_TYPE, maskCondition);
}

bool Osver::isEqualSuiteMask(BYTE suiteMask)
{
	//Detecting a Product Suite
	OSVERSIONINFOEX osVersionInfo;
	::ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osVersionInfo.wSuiteMask = suiteMask;
	ULONGLONG maskCondition = ::VerSetConditionMask(0, VER_SUITENAME, VER_EQUAL);
	return ::VerifyVersionInfo(&osVersionInfo, VER_SUITENAME, maskCondition);
}

bool Osver::isEqualServicePack(WORD servicePackMajor)
{
	OSVERSIONINFOEX osVersionInfo;
	::ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osVersionInfo.wServicePackMajor = servicePackMajor;
	ULONGLONG maskCondition = ::VerSetConditionMask(0, VER_SERVICEPACKMAJOR, VER_EQUAL);
	return ::VerifyVersionInfo(&osVersionInfo, VER_SERVICEPACKMAJOR, maskCondition);
}

DWORD Osver::getProductType()
{

	/*
		#define VER_NT_WORKSTATION              0x0000001
		#define VER_NT_DOMAIN_CONTROLLER        0x0000002
		#define VER_NT_SERVER                   0x0000003
	*/

	if (_isEqualProductType(VER_NT_WORKSTATION)) return VER_NT_WORKSTATION;
	else if (_isEqualProductType(VER_NT_WORKSTATION))return VER_NT_DOMAIN_CONTROLLER;
	else if (_isEqualProductType(VER_NT_SERVER))return VER_NT_SERVER;
	return 0; //unknown
}

WORD Osver::getServicePack()
{
	WORD sp = NULL;
	for (size_t i = 1; i <= 4; i++)
	{
		if (isEqualServicePack(i))
		{
			sp = isEqualServicePack(i);
			break;
		}
	}

	return sp;
}

DWORD Osver::getVersion()
{
	DWORD version = false;
	OSVERSIONINFOEXW osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
	//ZeroMemory(&osv, sizeof(OSVERSIONINFOEXW)); //Mem::_zero(&osv, sizeof(POSVERSIONINFOEXW));

	getMinorAndMajor(&osv);

	if (getProductType() == VER_NT_WORKSTATION)
	{
		if (osv.dwMajorVersion == 5 && osv.dwMinorVersion == 0) version = Windows2000;
		else if (osv.dwMajorVersion == 5 && osv.dwMinorVersion == 1) version = WindowsXP;
		else if (osv.dwMajorVersion == 5 && osv.dwMinorVersion == 2) version = WindowsXPProfessionalx64;
		else if (osv.dwMajorVersion == 6 && osv.dwMinorVersion == 0) version = WindowsVista;
		else if (osv.dwMajorVersion == 6 && osv.dwMinorVersion == 1) version = Windows7;
		else if (osv.dwMajorVersion == 6 && osv.dwMinorVersion == 2) version = Windows8;
		else if (osv.dwMajorVersion == 6 && osv.dwMinorVersion == 3) version = Windows8_1;
		else if (osv.dwMajorVersion == 10 && osv.dwMinorVersion == 0) version = Windows10;
	}

	else if (getProductType() == VER_NT_SERVER)
	{
		if (osv.dwMajorVersion == 5 && osv.dwMinorVersion == 2 && GetSystemMetrics(SM_SERVERR2) == 0) version = WindowsServer2003;
		else if (osv.dwMajorVersion == 5 && osv.dwMinorVersion == 2 && isEqualSuiteMask(VER_SUITE_WH_SERVER)) version = WindowsHomeServer;
		else if (osv.dwMajorVersion == 5 && osv.dwMinorVersion == 2 && GetSystemMetrics(SM_SERVERR2) != 0) version = WindowsServer2003R2;
		else if (osv.dwMajorVersion == 6 && osv.dwMinorVersion == 0) version = WindowsServer2008;
		else if (osv.dwMajorVersion == 6 && osv.dwMinorVersion == 1) version = WindowsServer2008R2;
		else if (osv.dwMajorVersion == 6 && osv.dwMinorVersion == 2) version = WindowsServer2012;
		else if (osv.dwMajorVersion == 6 && osv.dwMinorVersion == 3) version = WindowsServer2012R2;
		else if (osv.dwMajorVersion == 10 && osv.dwMinorVersion == 0) version = WindowsServer2016TechnicalPreview;
	}

	return  version;
}

void Osver::getVersionEx(COSINFO* prt)
{
	ZeroMemory(prt, sizeof(COSINFO));
	
	prt->version = getVersion();

	if (prt->version != NULL) prt->sp = getServicePack();
	

	/* 
							fixme later need
			We need to find another way to get the * dwBuildNumber *
				Maybe we will read it from the registry keys
	
	*/

	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_UNKNOWN)
	{
		prt->architecture = si.wProcessorArchitecture;
	}
	

		
}


int  main()
{
	switch (Osver::getVersion())
	{

	case unknown: printf("Tu versicion de Windows es: %s\n", "Desconocido");
	case Windows2000: printf("Tu versicion de Windows es: %s\n", "Windows 2000");
	case WindowsXP:  printf("Tu versicion de Windows es: %s\n", "Windows XP");
	case WindowsXPProfessionalx64: printf("Tu versicion de Windows es: %s\n", "Windows XP Professional x64 Edition");
	case WindowsServer2003:printf("Tu versicion de Windows es: %s\n", "Windows Server 2003");
	case WindowsHomeServer: printf("Tu versicion de Windows es: %s\n", "Windows Home Server");
	case WindowsServer2003R2: printf("Tu versicion de Windows es: %s\n", "Windows Server 2003 R2");
	case WindowsVista: printf("Tu versicion de Windows es: %s\n", "Windows Vista");
	case WindowsServer2008: printf("Tu versicion de Windows es: %s\n", "Windows Server 2008");
	case WindowsServer2008R2: printf("Tu versicion de Windows es: %s\n", "Windows Server 2008 R2");
	case Windows7: printf("Tu versicion de Windows es: %s\n", "Windows 7");
	case WindowsServer2012: printf("Tu versicion de Windows es: %s\n", "Windows Server 2012");
	case Windows8: printf("Tu versicion de Windows es: %s\n", "Windows 8");
	case WindowsServer2012R2: printf("Tu versicion de Windows es: %s\n", "Windows Server 2012 R2");
	case Windows8_1: printf("Tu versicion de Windows es: %s\n", "Windows 8.1");
	case WindowsServer2016TechnicalPreview: printf("Tu versicion de Windows es: %s\n", "Windows Server 2016");
	case Windows10: printf("Tu versicion de Windows es: %s\n", "Windows 10");

	}
	system("pause");

	return 0;
}
