#include "http_server.h"

#include <iostream>

namespace netflux {

	http_server::http_server()
	{
	}

	http_server::~http_server()
	{
	}

	void http_server::start(const network_id& id)
	{
		if (!m_server_socket.test()) {
			printf("Error: Server socket is invalid\n");
			return;
		}

		if (!m_server_socket.bind_to(id)) {
			printf("Error: Failed to bind server socket to address\n");
			return;
		}

		if (!m_server_socket.start_listening()) {
			printf("Error: Failed to start listening on server socket\n");
			return;
		}

		printf("Server started listening @ '%s:%u'", id.ip_addr, id.port);

		accept_connections();
	}

	void http_server::accept_connections()
	{
		for (;;) {
			tcp_socket client_socket(m_server_socket.accept_connection());
			if (!client_socket.test()) {
				continue;
			}

			handle_request(&client_socket);
			client_socket.close();
		}
	}

	void http_server::handle_request(tcp_socket* socket)
	{
		std::string packet;
		int32_t recv_len;
		if (!socket->receive_packet(packet, recv_len)) {
			return;
		}

		printf("%s", packet.c_str());
	}

}
