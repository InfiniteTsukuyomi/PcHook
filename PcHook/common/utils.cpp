#include "stdafx.h"
#include "utils.h"

CStringA Unicode_To_Utf8(CString strUnicode)
{
	CStringA strUtf8;

	int nLen = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, strUtf8.GetBuffer(nLen + 1), nLen, NULL, NULL);
	strUtf8.ReleaseBuffer();

	return strUtf8;
}

CString Utf8_To_Unicode(CStringA strUtf8)
{
	CString strUnicode;

	int nLen = MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, NULL, 0);

	MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, strUnicode.GetBuffer(nLen + 1), nLen);
	strUnicode.ReleaseBuffer();

	return strUnicode;
}
