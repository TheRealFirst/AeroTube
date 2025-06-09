#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Logging\Log.h"
#include "Logging\asserts.h"
#include "Core\Core.h"

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#endif