#include "http_server.h"

#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>

namespace netflux {

	http_server::http_server()
	{
	}

	http_server::~http_server()
	{
	}

	void http_server::launch(const network_id& id)
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
		std::string request;
		if (!socket->receive_packet(request)) {
			return;
		}

		printf("%s", request.c_str());
		http_request parsed_request = parse_http_request(request);

		http_response response;
		process_http_request(parsed_request, response);

		std::string response_str = generate_http_response(response);
		if (!socket->send_packet(response_str)) {
			printf("Error: Server failed to send HTTP response\n");
			return;
		}
	}

	http_request http_server::parse_http_request(const std::string& request)
	{
		http_request result;
		size_t start = 0;
		size_t cursor = 0;

		while (cursor < request.size()) {
			if (std::isspace(request[cursor])) {
				std::string http_method = request.substr(start, cursor - start);
				result.method = string_to_http_method(http_method.c_str());
				cursor++;
				start = cursor;
				break;
			}

			cursor++;
		}

		while (cursor < request.size()) {
			if (std::isspace(request[cursor])) {
				result.url = request.substr(start, cursor - start);
				cursor++;
				start = cursor;
				break;
			}

			cursor++;
		}

		while (cursor < request.size()) {
			std::string slice = request.substr(cursor, cursor + 4);
			if (slice == "\r\n\r\n") {
				result.header = request.substr(start, (cursor - 1) - start);
				for (uint32_t i = 0; i < 4; i++)
					cursor++;
				start = cursor;
				break;
			}

			cursor++;
		}

		result.body = request.substr(start, request.size() - start);

		return result;
	}

	void http_server::process_http_request(const http_request& req, http_response& res)
	{
		switch (req.method) {
			case http_method::GET: {
				if (req.url == "/" || req.url.starts_with("/index.html")) {
					// index.html
					std::string path = "site/index.html";
					std::string index_html_contents;

					if (!std::filesystem::exists(path)) {
						res.status.code = 404;
						res.status.message = "Not Found";
						return;
					}

					std::ifstream file(path);
					if (!file.is_open()) {
						res.status.code = 404;
						res.status.message = "Not Found";
						return;
					}

					file.seekg(0, std::ios::end);
					size_t file_size = file.tellg();
					index_html_contents.resize(file_size);
					file.seekg(0, std::ios::beg);

					file.read(index_html_contents.data(), file_size);
					file.close();

					res.body = index_html_contents;
				}
				else {
					res.body = "Welcome to my web server!";
				}

				res.status.code = 200;
				res.status.message = "OK";

				res.header = std::format("Content-Length: {}\r\nContent-Type: {}", res.body.size(), "text/html");
			} break;
			case http_method::POST: {

			} break;
			case http_method::UPDATE: {

			} break;
			case http_method::DELETE: {

			} break;
			default:
				printf("Error: Failed to process http request\n");
				res.status.code = 500;
				res.status.message = "Internal Server Error";
				break;
		}
	}

	std::string http_server::generate_http_response(const http_response& res)
	{
		std::string response_str = std::format("HTTP/1.1 {} {}\r\n{}\r\n\r\n{}",
			res.status.code,
			res.status.message,
			res.header,
			res.body
		);

		return response_str;
	}

}
