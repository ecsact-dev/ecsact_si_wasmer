#include <vector>
#include <string>
#include <filesystem>
#include "docopt.h"
#include <iostream>
#include "magic_enum.hpp"

#include "ecsactsi_wasm.h"
#include "mock_set_system_impl.hh"

namespace fs = std::filesystem;

constexpr auto USAGE = R"(
Test Ecsact system implementation with mock ecsact_system_execution_context_*
methods.

Usage:
	ecsactsi_wasm_tester <wasm_file> <system_impl_fn_name>

Options:
	<wasm_file>
		Wasm file containing <system_impl_fn_name> function export.
	
	<system_impl_fn_name>
		Export name of <wasm_file> for wasm function that implements an Ecsact
		system implementation function. https://ecsact.dev/docs/system-impl-wasm
)";

auto main(int argc, char* argv[]) -> int {
	auto args = docopt::docopt(USAGE, {argv + 1, argv + argc});
	auto wasm_file = args.at("<wasm_file>").asString();
	auto system_impl_fn_name = args.at("<system_impl_fn_name>").asString();

	std::cout //
		<< "Loading wasm file: " << fs::absolute(wasm_file).string() << "\n";

	auto mock_sys_like_id = ecsact_system_like_id{};
	auto system_impl_name_cstr = system_impl_fn_name.c_str();
	auto load_err = ecsactsi_wasm_load_file(
		wasm_file.c_str(),
		1,
		&mock_sys_like_id,
		&system_impl_name_cstr
	);

	if(load_err != ECSACTSI_WASM_OK) {
		std::cerr << "Load Error: " << magic_enum::enum_name(load_err) << "\n";
		return 1;
	}

	auto impl = ecsactsi_wasm_tester::get_impl();
	if(impl == nullptr) {
		std::cerr << "impl is nullptr\n";
		return 1;
	}

	impl(nullptr);

	return 0;
}
