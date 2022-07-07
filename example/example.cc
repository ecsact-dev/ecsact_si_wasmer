#include <vector>
#include <string>
#include <filesystem>
#include <iostream>

#include "ecsactsi_wasm.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
	bool use_meta = false;
	std::vector<std::string> wasm_file_paths;

	for(int i=1; argc > i; ++i) {
		std::string arg(argv[i]);

		if(arg == "--use_meta") {
			use_meta = true;
		} else if(arg.starts_with('-')) else {
			std::cerr << "[ERROR] Unknown arg: " << arg << "\n";
			return 1;
		} {
			wasm_file_paths.push_back(arg);
		}
	}

	if(wasm_file_paths.empty()) {
		std::cerr << "[ERROR] Nothing to do.\n";
		return 1;
	}

	for(auto& wasm_path : wasm_file_paths) {
		if(!fs::exists(wasm_path)) {
			std::cerr << "[ERROR] " << wasm_path << " does not exist\n";
			return 1;
		}
	}

	if(use_meta) {
		for(auto& wasm_path : wasm_file_paths) {
			auto err = ecsactsi_wasm_load_file_meta(wasm_path.c_str());
			if(err != ECSACTSI_WASM_OK) {
				std::cerr
					<< "[ERROR] loading wasm file "
					<< wasm_path << " with meta API failed: "
					<< magic_enum::  << "\n";
				return 2;
			}
		}
	}

	return 0;
}
