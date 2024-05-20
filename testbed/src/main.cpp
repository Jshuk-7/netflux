#include <netflux.h>

int main() {
	netflux::init();
	
	netflux::http_server server;
	server.start({ "127.0.0.1", 27015 });

	netflux::shutdown();
}