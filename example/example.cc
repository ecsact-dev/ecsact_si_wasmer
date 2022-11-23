#include <vector>
#include <atomic>
#include <string>
#include <filesystem>
#include <iostream>
#include <csignal>
#include "magic_enum.hpp"
#include "ecsact/runtime/core.h"
#include "ecsact/runtime/core.hh"
#include "ecsactsi_wasm.h"

#include "example.ecsact.hh"

namespace fs = std::filesystem;

void print_load_error(ecsactsi_wasm_error err, const std::string& wasm_path) {
	std::cerr //
		<< "[ERROR] loading wasm file " << wasm_path
		<< " failed: " << magic_enum::enum_name(err) << "\n\n";
}

std::string system_name(ecsact_system_like_id sys_id) {
	using namespace example;

	if(sys_id == ecsact_id_cast<ecsact_system_like_id>(Generator::id)) {
		return "example.Generator";
	}
	if(sys_id == ecsact_id_cast<ecsact_system_like_id>(AddsSystem::id)) {
		return "example.AddsSystem";
	}
	if(sys_id == ecsact_id_cast<ecsact_system_like_id>(RemovesSystem::id)) {
		return "example.RemovesSystem";
	}
	if(sys_id == ecsact_id_cast<ecsact_system_like_id>(ExampleSystem::id)) {
		return "example.ExampleSystem";
	}

	return "UNKNOWN SYSTEM OR ACTION";
}

std::string component_name(ecsact_component_id comp_id) {
	if(comp_id == example::WillRemove::id) {
		return "example.WillRemove";
	}
	if(comp_id == example::WillAdd::id) {
		return "example.WillAdd";
	}
	if(comp_id == example::WillAdd::id) {
		return "example.WillAdd";
	}
	if(comp_id == example::Spawner::id) {
		return "example.Spawner";
	}
	if(comp_id == example::ExampleComponent::id) {
		return "example.ExampleComponent";
	}

	return "UNKNOWN COMPONENT";
}

void print_event(
	ecsact_event        event,
	ecsact_entity_id    entity_id,
	ecsact_component_id component_id,
	const void*         component_data,
	void*               user_data
) {
	std::cout //
		<< "[EVENT]: " << magic_enum::enum_name(event) << " "
		<< component_name(component_id) << "\n";
}

std::atomic_bool wasm_trap_happened = false;

void trap_handler(ecsact_system_like_id system_id, const char* trap_message) {
	wasm_trap_happened = true;
	std::string msg = "[WASM TRAP]: ";
	msg += system_name(system_id) + " triggered a trap: ";
	msg += std::string(trap_message);
	msg += "\n";

	std::cerr << msg;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> wasm_file_paths;

	for(int i = 1; argc > i; ++i) {
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

	ecsact::core::registry test_registry("Test Registry");

	auto test_entity = test_registry.create_entity();
	test_registry.add_component(test_entity, example::Spawner{});
	test_registry.add_component(test_entity, example::ExampleComponent{});

	for(auto& wasm_path : wasm_file_paths) {
		std::vector<ecsact_system_like_id> system_ids{
			ecsact_id_cast<ecsact_system_like_id>(example::ExampleSystem::id),
			ecsact_id_cast<ecsact_system_like_id>(example::Generator::id),
			ecsact_id_cast<ecsact_system_like_id>(example::AddsSystem::id),
			ecsact_id_cast<ecsact_system_like_id>(example::CheckShouldRemove::id),
			ecsact_id_cast<ecsact_system_like_id>(example::RemovesSystem::id),
		};

		std::vector<const char*> wasm_exports{
			"example__ExampleSystem",
			"example__Generator",
			"example__AddsSystem",
			"example__CheckShouldRemove",
			"example__RemovesSystem",
		};

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

	ecsactsi_wasm_set_trap_handler(&trap_handler);

	for(int i = 0; 10 > i; ++i) {
		std::cout << "\n==== EXECUTION (" << i << ") ====\n";

		ecsact_execution_events_collector ev_collector{
			.init_callback = &print_event,
			.init_callback_user_data = nullptr,
			.update_callback = &print_event,
			.update_callback_user_data = nullptr,
			.remove_callback = &print_event,
			.remove_callback_user_data = nullptr,
		};

		ecsact_execute_systems(test_registry.id(), 1, nullptr, &ev_collector);

		std::cout << "[POST-EXECUTE]: Entity Count="
							<< ecsact_count_entities(test_registry.id()) << "\n";
	}

	std::cout << "\n (( Done ))\n";

	if(wasm_trap_happened) {
		return 1;
	}

	return 0;
}
