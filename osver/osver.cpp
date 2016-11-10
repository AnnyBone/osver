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

DWORD Osver::getProductType()
{	/*
	#define VER_NT_WORKSTATION              0x0000001
	#define VER_NT_DOMAIN_CONTROLLER        0x0000002
	#define VER_NT_SERVER                   0x0000003
	*/

	if (_isEqualProductType(VER_NT_WORKSTATION)) return VER_NT_WORKSTATION;
	else if (_isEqualProductType(VER_NT_WORKSTATION))return VER_NT_DOMAIN_CONTROLLER;
	else if (_isEqualProductType(VER_NT_SERVER))return VER_NT_SERVER;
	return 0; //urkrow
}

int  main()
{
	OSVERSIONINFOEXW osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
	Osver::getMinorAndMajor(&osv);

	std::cout << Osver::getProductType();

	system("pause");

	return 0;
}
