#pragma once

//根据类型和名字枚举指定进程句柄, 参数dwOptions默认允许远程关闭该句柄.
HANDLE FindHandle(ULONG pid, LPCTSTR szObjectType, LPCTSTR szObjectName, DWORD dwOptions = DUPLICATE_CLOSE_SOURCE);