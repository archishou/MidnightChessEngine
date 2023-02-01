#include "ChessEngine_lib/uci_interpreter/uci_interpreter.h"

int main () {
	bool uci_debug_from_file = false;
	if (uci_debug_from_file) {
		const string& input_file_path = "/Users/archishmaan/Documents/CodeProjects/chess-engine/ChessEngine/ChessEngine_lib/uci_input.txt";
		const string& output_file_path = "/Users/archishmaan/Documents/CodeProjects/chess-engine/ChessEngine/ChessEngine_lib/uci_output.txt";
		read_uci_from_file(input_file_path, output_file_path);
	} else {
		read_uci("/Users/archishmaan/Documents/CodeProjects/chess-engine/ChessEngine/ChessEngine_lib/engine_uci_inputs_diagnostics.txt");
	}
}
