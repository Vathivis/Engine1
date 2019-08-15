#pragma once

#ifdef E1_PLATFORM_WINDOWS
	#include <winsock2.h>
	#define _WINSOCKAPI_
	#include "windows.h"
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <algorithm>
#include <limits>
#include <chrono>
#include <numeric>

//data struktury
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "Engine1/Log.h"
#include "Engine1/KeyCodes.h"
#include "Engine1/MouseButtonCodes.h"





