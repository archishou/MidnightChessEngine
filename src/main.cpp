#include "uci_interpreter/uci_interpreter.h"

int main(i32 argc, char *argv[]) {
	initialize_engine();
	if (argc > 1 && string{argv[1]} == "bench") {
		bench();
		return 0;
	}
	read_uci();
}
