#include "netflux.h"

#include <winsock2.h>

namespace netflux {

	bool init()
	{
		WSADATA startupData;
		int32_t startup_result = WSAStartup(MAKEWORD(2, 2), &startupData);
		if (startup_result != 0) {
			printf("Error: Failed to statup network API\n");
			return false;
		}

		return true;
	}

	bool shutdown()
	{
		WSACleanup();
		return true;
	}

}
