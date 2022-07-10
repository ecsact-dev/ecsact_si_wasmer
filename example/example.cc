#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <csignal>
#include "magic_enum.hpp"
#include <ecsact/runtime/core.h>
#include <ecsact/runtime.hh>
#include "ecsactsi_wasm.h"

#include "example/example.ecsact.hh"
#include "example/example.ecsact.systems.hh"

namespace fs = std::filesystem;

void print_load_error
	( ecsactsi_wasm_error  err
	, const std::string&   wasm_path
	)
{
	std::cerr
		<< "[ERROR] loading wasm file " << wasm_path << " failed: "
		<< magic_enum::enum_name(err) << "\n\n";
}

int main(int argc, char* argv[]) {
	std::vector<std::string> wasm_file_paths;

	for(int i=1; argc > i; ++i) {
		std::string arg(argv[i]);

		if(arg.starts_with('-')) {
			std::cerr << "[ERROR] Unknown arg: " << arg << "\n";
			return 1;
		} else {
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

	ecsact::registry test_registry;
	auto test_entity = test_registry.create_entity();
	test_registry.add_component(test_entity, example::ExampleComponent{});

	for(auto& wasm_path : wasm_file_paths) {
		std::vector<ecsact_system_id> system_ids;
		std::vector<const char*> wasm_exports;

		system_ids.push_back(
			static_cast<ecsact_system_id>(example::ExampleSystem::id)
		);
		wasm_exports.push_back("example__ExampleSystem");

		assert(system_ids.size() == wasm_exports.size());

		auto err = ecsactsi_wasm_load_file(
			wasm_path.c_str(),
			static_cast<int>(system_ids.size()),
			system_ids.data(),
			wasm_exports.data()
		);

		if(err != ECSACTSI_WASM_OK) {
			print_load_error(err, wasm_path);
			return 2;
		}

		std::cout << "Loaded " << wasm_path << " successfully!\n";
	}

	for(int i=0; 10 >i; ++i) {
		std::cout << "execute systems count: " << i + 1 << "\n";
		test_registry.execute_systems();

		const auto& comp =
			test_registry.get_component<example::ExampleComponent>(test_entity);
		std::cout << "  ExampleComponent.num = " << comp.num << "\n";
	}

	std::cout << "Done!\n";
	return 0;
}
