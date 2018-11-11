// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>
#include <atlstr.h>
#include <mutex>
#include <stdio.h>
#include <string>
#include <list>
#include <atomic>

using namespace std;


// TODO:  在此处引用程序需要的其他头文件
#include "common/singleton.h"
#include "common/log.h"
#include "common/hook.h"
#include "common/utils.h"
#include "common/EnumHandle.h"

