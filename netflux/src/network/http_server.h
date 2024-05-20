#pragma once

#include <network/tcp_socket.h>
#include <network/http.h>

namespace netflux {

	class http_server {
	public:
		http_server();
		~http_server();

		void launch(const network_id& id);

	private:
		void accept_connections();
		void handle_request(tcp_socket* socket);

		http_request parse_http_request(const std::string& request);
		void process_http_request(const http_request& req, http_response& res);
		std::string generate_http_response(const http_response& res);

	private:
		tcp_socket m_server_socket;
	};

}