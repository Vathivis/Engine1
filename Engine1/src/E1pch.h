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
#include <ctime>
#include <chrono>
#include <numeric>
#include <thread>
#include <future>

//data struktury
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "Engine1/Core/Log.h"
#include "Engine1/Core/KeyCodes.h"
#include "Engine1/Core/MouseButtonCodes.h"





