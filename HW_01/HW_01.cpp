// Windows Programming.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdarg.h>
#include <Strsafe.h>
#include <WinBase.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"
/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
void print(_In_ const char* fmt, _In_ ...)
{
	char log_buffer[2048];
	va_list args;

	va_start(args, fmt);
	HRESULT hRes = StringCbVPrintfA(log_buffer, sizeof(log_buffer), fmt, args);
	if (S_OK != hRes)
	{
		fprintf(
			stderr,
			"%s, StringCbVPrintfA() failed. res = 0x%08x",
			__FUNCTION__,
			hRes
			);
		return;
	}

	OutputDebugStringA(log_buffer);
	fprintf(stdout, "%s \n", log_buffer);
}



/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
bool create_bob_txt()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	if (true == is_file_existsW(file_name))
	{
		::DeleteFileW(file_name);
	}

	// 파일 생성
	HANDLE file_handle = CreateFileW(
		file_name,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		print("err, CreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
		return false;
	}

	// 파일에 데이터 쓰기
	DWORD bytes_written = 0;
	wchar_t string_buf[1024];
	if (!SUCCEEDED(StringCbPrintfW(
		string_buf,
		sizeof(string_buf),
		L"동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라만세 All work and no play makes jack a dull boy.")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}

	int bufalen = 0;
	char *bufa = NULL;
	bufalen = WideCharToMultiByte(CP_UTF8, 0, string_buf, -1, bufa, 0, NULL, NULL);
	bufa = (char*)malloc(bufalen + 1);
	WideCharToMultiByte(CP_UTF8, 0, string_buf, -1, bufa, 1024, NULL, NULL);


	if (!WriteFile(file_handle, bufa, strlen(bufa), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	// 파일 닫기
	CloseHandle(file_handle);
	return true;

}

bool delete_bob_txt(){
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	::DeleteFileW(file_name);

	// current dir \\ bob.txt 파일명 생성
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;
	::DeleteFileW(file_name);

}

bool copy_bob_txt(){
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t file_name_src[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name_src,
		sizeof(file_name_src),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	// current dir \\ bob2.txt 파일명 생성
	wchar_t file_name_dst[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name_dst,
		sizeof(file_name_dst),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	free(buf); buf = NULL;

	// 파일을 복사한다.
	if (!SUCCEEDED(CopyFile(file_name_src, file_name_dst, TRUE))){
		print("err, can not copy file name");
		return false;
	}

	return true;




}
bool read_bob_txt_rf(){

	HANDLE fp = NULL;
	DWORD readn;
	BOOL brtv;
	char read[1024];
	//LARGE_INTEGER curPtr;

	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob2.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	free(buf); buf = NULL;

	// 파일 생성
	HANDLE file_handle = CreateFileW(
		file_name,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		NULL,
		NULL);


	if (file_handle == INVALID_HANDLE_VALUE)
	{
		print("err, CreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
		return false;
	}

	// 파일 읽기
	ReadFile(file_handle, read, 1024, &readn, NULL);
	CloseHandle(file_handle);
	int len = MultiByteToWideChar(CP_UTF8, 0, read, readn, NULL, NULL);
	wchar_t* strbuf = NULL;
	strbuf = (wchar_t*)malloc(sizeof(wchar_t)*(len + 1));
	strbuf[len] = 0;
	MultiByteToWideChar(CP_UTF8, 0, read, readn, strbuf, len);


	int bufalen = 0;
	char *bufa = NULL;
	bufalen = WideCharToMultiByte(CP_ACP, 0, strbuf, -1, bufa, 0, NULL, NULL);
	bufa = (char*)malloc(sizeof(char)*(bufalen + 1));
	bufa[bufalen] = 0;
	WideCharToMultiByte(CP_ACP, 0, strbuf, -1, bufa, bufalen, NULL, NULL);


	printf("%s\n", bufa);

	free(strbuf); strbuf = NULL;
	free(bufa); bufa = NULL;

	return true;
}
bool read_bob_txt_mm(){
	HANDLE fp = NULL;
	DWORD readn;
	BOOL brtv;
	char read[1024];
	//LARGE_INTEGER curPtr;

	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob2.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}

	free(buf); buf = NULL;

	HANDLE file_handle = CreateFileW(
		(LPCWSTR)file_name,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (INVALID_HANDLE_VALUE == file_handle)
	{
		print("err, CreateFile(%ws) failed, gle = %u", file_name, GetLastError());
		return false;
	}

	// check file size
	//
	LARGE_INTEGER fileSize;
	if (TRUE != GetFileSizeEx(file_handle, &fileSize))
	{
		print("err, GetFileSizeEx(%ws) failed, gle = %u", file_name, GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	// [ WARN ]
	//
	// 4Gb 이상의 파일의 경우 MapViewOfFile()에서 오류가 나거나
	// 파일 포인터 이동이 문제가 됨
	// FilIoHelperClass 모듈을 이용해야 함
	//
	_ASSERTE(fileSize.HighPart == 0);
	if (fileSize.HighPart > 0)
	{
		print("file size = %I64d (over 4GB) can not handle. use FileIoHelperClass",
			fileSize.QuadPart);
		CloseHandle(file_handle);
		return false;
	}

	DWORD file_size = (DWORD)fileSize.QuadPart;
	HANDLE file_map = CreateFileMapping(
		file_handle,
		NULL,
		PAGE_READONLY,
		0,
		0,
		NULL
		);
	if (NULL == file_map)
	{
		print("err, CreateFileMapping(%ws) failed, gle = %u", file_name, GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	PCHAR file_view = (PCHAR)MapViewOfFile(
		file_map,
		FILE_MAP_READ,
		0,
		0,
		0
		);
	if (file_view == NULL)
	{
		print("err, MapViewOfFile(%ws) failed, gle = %u", file_name, GetLastError());

		CloseHandle(file_map);
		CloseHandle(file_handle);
		return false;
	}

	// do some io
	//char a = file_view[0];  // 0x d9
	//char b = file_view[1];  // 0xb3




	int len = MultiByteToWideChar(CP_UTF8, 0, file_view, file_size, NULL, NULL);
	wchar_t* strbuf = NULL;
	strbuf = (wchar_t*)malloc(sizeof(wchar_t)*(len + 1));
	strbuf[len] = 0;
	MultiByteToWideChar(CP_UTF8, 0, file_view, file_size, strbuf, len);


	int bufalen = 0;
	char *bufa = NULL;
	bufalen = WideCharToMultiByte(CP_ACP, 0, strbuf, -1, bufa, 0, NULL, NULL);
	bufa = (char*)malloc(sizeof(char)*(bufalen + 1));
	bufa[bufalen] = 0;
	WideCharToMultiByte(CP_ACP, 0, strbuf, -1, bufa, bufalen, NULL, NULL);


	printf("%s\n", bufa);

	free(strbuf); strbuf = NULL;
	free(bufa); bufa = NULL;

	// close all
	UnmapViewOfFile(file_view);
	CloseHandle(file_map);
	CloseHandle(file_handle);
	return true;

	return true;
}
int _tmain(int argc, _TCHAR* argv[])
{
	create_bob_txt();
	copy_bob_txt();
	read_bob_txt_rf();
	read_bob_txt_mm();
	//delete_bob_txt();
	return 0;
}


