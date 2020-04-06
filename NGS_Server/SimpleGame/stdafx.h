#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <map>
using namespace std;
