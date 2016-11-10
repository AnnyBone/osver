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
	OSVERSIONINFOEXW osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

	prt->version = getVersion();
	if (prt->version != NULL)
	{
		// OK Let's get sp pack

	}


	/*
		SYSTEM_INFO si;
		GetNativeSystemInfo(&si);
		*/
}


int  main()
{
	COSINFO osinfo;
	Osver::getVersionEx(&osinfo);
	std::cout << osinfo.version;
	;
	system("pause");

	return 0;
}
