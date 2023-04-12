#include "uci_interpreter/uci_interpreter.h"

int main (int argc, char *argv[]) {
	initialize_engine();
	if (argc > 1 && std::string(argv[1]) == "bench") {
		bench();
		return 0;
	}
	read_uci();
}
