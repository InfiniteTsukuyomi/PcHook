#pragma once
#include <atlstr.h>

#define LOCK(x)		std::lock_guard<std::mutex> lock(x);

CStringA Unicode_To_Utf8(CString strUnicode);
CString  Utf8_To_Unicode(CStringA strUtf8);

#define CW2UTF8(strUnicode)		Unicode_To_Utf8(strUnicode)
#define CUTF82W(strUtf8)		Utf8_To_Unicode(strUtf8)

//可能丢失信息
#define CUTF82Gbk(strUtf8)		(CW2A(CUTF82W(strUtf8)))