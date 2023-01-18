#include "wasi_test.ecsact.hh"
#include "wasi_test.ecsact.systems.h"

#include <iostream>
#include <fstream>

static const auto example_text = []() -> std::string {
	std::cout << "Initialzing static string\n";

	// auto f = std::ifstream{"example.txt"};
	// auto content = std::string{};
	// f >> content;
	// f.close();
	// return content;
	return "Example CONTENT!";
}();

void wasi_test__WasiTestSystem(ecsact_system_execution_context*) {
	// std::cout << "Content from file: " << example_text << "\n";
	std::printf("Content From File: %s\n", example_text.c_str());
}
