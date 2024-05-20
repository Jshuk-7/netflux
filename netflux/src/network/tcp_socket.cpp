#include "tcp_socket.h"

#include <iostream>

#include <winsock2.h>

namespace netflux {

#define MAX_PACKET_SIZE 4096

	tcp_socket::tcp_socket()
	{
		m_socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (m_socket_id == INVALID_SOCKET) {
			printf("Error(%i): Failed to create socket\n", WSAGetLastError());
			return;
		}
	}

	tcp_socket::tcp_socket(std::uintptr_t socket_id)
	{
		m_socket_id = socket_id;
	}

	tcp_socket::~tcp_socket()
	{
		close();
	}

	bool tcp_socket::test()
	{
		return m_socket_id > 0;
	}

	bool tcp_socket::bind_to(const network_id& id)
	{
		SOCKADDR_IN socket_data;
		socket_data.sin_family = AF_INET;
		socket_data.sin_addr.S_un.S_addr = inet_addr(id.ip_addr);
		socket_data.sin_port = htons(id.port);

		int32_t bind_result = bind((SOCKET)m_socket_id, (const sockaddr*)&socket_data, sizeof(socket_data));
		if (bind_result == SOCKET_ERROR) {
			printf("Error(%i): Failed to bind socket @ '%s:%u'\n", WSAGetLastError(), id.ip_addr, id.port);
			close();
			return false;
		}

		m_network_id = id;

		return true;
	}

	bool tcp_socket::start_listening()
	{
		int32_t listen_result = listen((SOCKET)m_socket_id, 0);
		if (listen_result == SOCKET_ERROR) {
			printf("Error(%i): Socket failed to start listening\n", WSAGetLastError());
			close();
			return false;
		}

		return true;
	}

	std::uintptr_t tcp_socket::accept_connection()
	{
		SOCKET client_socket = accept((SOCKET)m_socket_id, nullptr, nullptr);
		if (client_socket == INVALID_SOCKET) {
			printf("Error(%i): Socket failed to accept connection\n", WSAGetLastError());
			return ~0;
		}

		return client_socket;
	}

	bool tcp_socket::receive_packet(std::string& packet)
	{
		char* recv_buf = (char*)malloc(sizeof(char) * MAX_PACKET_SIZE);
		if (recv_buf == nullptr) {
			return false;
		}

		int32_t recv_len = recv((SOCKET)m_socket_id, recv_buf, MAX_PACKET_SIZE, 0);

		bool result = false;
		if (recv_len > 0) {
			packet.resize(recv_len);
			memcpy(packet.data(), recv_buf, recv_len);
			result = true;
		}

		free(recv_buf);
		return result;
	}

	bool tcp_socket::send_packet(const std::string& packet)
	{
		int32_t send_result = send((SOCKET)m_socket_id, packet.data(), packet.size(), 0);
		if (send_result == SOCKET_ERROR) {
			return false;
		}

		return true;
	}

	bool tcp_socket::close()
	{
		if (test()) {
			closesocket((SOCKET)m_socket_id);
		}

		return true;
	}

}
