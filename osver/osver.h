#pragma once

#ifndef osver_H
#define osver_H

typedef struct COSINFO
{
	DWORD version;		//Windows Version
	DWORD sp;			//service Pack
	WORD build;			// build number						
	WORD architecture;

}COSINFO;

enum knownSo
{
	unknown,
	Windows2000,						//5.0
	WindowsXP,							//5.1
	WindowsXPProfessionalx64,			//5.2
	WindowsServer2003,					//5.2
	WindowsHomeServer,					//5.2
	WindowsServer2003R2,				//5.2
	WindowsVista,						//6.0
	WindowsServer2008,					//6.0
	WindowsServer2008R2,				//6.1
	Windows7,							//6.1
	WindowsServer2012,					//6.2
	Windows8,							//6.2
	WindowsServer2012R2,				//6.3
	Windows8_1,							//6.3
	WindowsServer2016TechnicalPreview,  //10.0
	Windows10						    //10.0

};



namespace Osver
{

	void getMinorAndMajor(POSVERSIONINFOEXW pVersions);
	BYTE _isEqualProductType(BYTE productType);
	bool isEqualSuiteMask(BYTE suiteMask);
	DWORD getProductType();
	DWORD getVersion();
	void getVersionEx(COSINFO *prt);

}


#endif // osver_H
