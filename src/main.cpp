#include "uci_interpreter/datagen.h"
#include "uci_interpreter/uci_interpreter.h"

int main(i32 argc, char *argv[]) {
	initialize_engine();
	if (argc > 1) {
		if (string{argv[1]} == "bench") {
			bench();
		} else if (string{argv[1]} == "datagen") {
			datagen(std::stoi(string{argv[2]}), string{argv[3]});
		}
		return 0;
	}
	read_uci();
}
