#pragma once

#include <network/tcp_socket.h>

namespace netflux {

	class http_server {
	public:
		http_server();
		~http_server();

		void start(const network_id& id);

	private:
		void accept_connections();
		void handle_request(tcp_socket* socket);

	private:
		tcp_socket m_server_socket;
	};

}