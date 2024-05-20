#pragma once

#include <cstdint>
#include <string>

namespace netflux {

	enum class http_method
	{
		None = 0, GET, POST, UPDATE, DELETE,
	};

	struct http_request
	{
		http_method method;
		std::string url;
		std::string header;
		std::string body;
	};

	struct http_status
	{
		uint32_t code;
		std::string message;
	};

	struct http_response
	{
		http_status status;
		std::string header;
		std::string body;
	};

	static const char* string_from_http_method(http_method method) {
		switch (method) {
			case http_method::GET:    return "GET";
			case http_method::POST:   return "POST";
			case http_method::UPDATE: return "UPDATE";
			case http_method::DELETE: return "DELETE";
		}

		return "Unknown http method!";
	}

	static http_method string_to_http_method(const char* str) {
		if (strcmp(str,"GET") == 0)    return http_method::GET;
		if (strcmp(str,"POST") == 0)   return http_method::POST;
		if (strcmp(str,"UPDATE") == 0) return http_method::UPDATE;
		if (strcmp(str,"DELETE") == 0) return http_method::DELETE;

		return http_method::None;
	}

}