#pragma once

#include <cstdint>

namespace netflux {

	struct network_id
	{
		const char* ip_addr;
		uint16_t port;
	};

}