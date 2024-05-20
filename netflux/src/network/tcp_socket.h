#pragma once

#include <network/network_id.h>

#include <cstdint>
#include <string>

namespace netflux {

	class tcp_socket {
	public:
		tcp_socket();
		tcp_socket(std::uintptr_t socket_id);
		~tcp_socket();

		bool test();

		bool bind_to(const network_id& id);
		bool start_listening();

		std::uintptr_t accept_connection();
		bool receive_packet(std::string& packet, int32_t& recv_len);

		bool close();

	private:
		std::uintptr_t m_socket_id;
		network_id m_network_id;
	};

}