#include <netflux.h>

int main() {
	netflux::init();
	getchar();
	netflux::shutdown();
	return 0;
}