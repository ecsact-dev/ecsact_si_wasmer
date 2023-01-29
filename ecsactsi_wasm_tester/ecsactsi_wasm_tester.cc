#include <variant>
#include <vector>
#include <string>
#include <filesystem>
#include "docopt.h"
#include <iostream>
#include "magic_enum.hpp"

#include "ecsactsi_wasm.h"
#include "mock_set_system_impl.hh"
#include "detail/ecsactsi_wasm_internal.hh"
#include "detail/wasm_file_binary.hh"
#include "detail/wasm_ecsact_memory.hh"

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
	using ecsactsi_wasm::detail::wasm_file_binary;

	auto args = docopt::docopt(USAGE, {argv + 1, argv + argc});
	auto wasm_file = args.at("<wasm_file>").asString();
	auto system_impl_fn_name = args.at("<system_impl_fn_name>").asString();

	std::cout //
		<< "Loading wasm file: " << fs::absolute(wasm_file).string() << "\n";

	auto mock_sys_like_id = ecsact_system_like_id{};
	auto system_impl_name_cstr = system_impl_fn_name.c_str();

	auto file = std::fopen(wasm_file.c_str(), "rb");
	auto file_bin = ecsactsi_wasm::detail::wasm_file_binary{};
	{
		const bool read_success = wasm_file_binary::read(file, file_bin);
		std::fclose(file);
		if(!read_success) {
			std::cerr << "Failed to read " << wasm_file << "\n";
			return 1;
		}
	}

	auto load_result = ecsactsi_wasm::detail::load_modules(
		file_bin.binary.data,
		file_bin.binary.size,
		1,
		&mock_sys_like_id,
		&system_impl_name_cstr
	);

	if(std::holds_alternative<ecsactsi_wasm_error>(load_result)) {
		std::cerr //
			<< "EcsactSI Wasm Load Error: "
			<< ecsactsi_wasm::detail::get_last_error_message() << "\n";
		return 1;
	}

	auto& modules = std::get<1>(load_result);

	for(auto&& [system_id, info] : modules) {
		auto trap = ecsactsi_wasm::detail::init_module(info);
		if(trap) {
			std::cerr //
				<< "[Wasm Trap During Init]: "
				<< ecsactsi_wasm::detail::trap_message(trap) << "\n";
			return 1;
		}
	}

	for(int n = 0; 3 > n; ++n) {
		std::cout << "Test Execution Output (" << n << ")\n";
		for(auto&& [system_id, info] : modules) {
			ecsactsi_wasm::detail::set_current_wasm_memory(info.system_impl_memory);

			auto           args = wasm_val_vec_t{};
			wasm_val_vec_t result = WASM_EMPTY_VEC;

			wasm_val_vec_new_uninitialized(&args, 1);
			args.data[0].kind = WASM_I32;
			args.data[0].of.i32 = 0;

			auto trap = wasm_func_call(info.system_impl_func, &args, &result);
			if(trap) {
				std::cerr //
					<< "[Wasm Trap During Execution (" << static_cast<int32_t>(system_id)
					<< ")]: " << ecsactsi_wasm::detail::trap_message(trap) << "\n";

				wasm_trap_delete(trap);
			}

			ecsactsi_wasm::detail::set_current_wasm_memory(nullptr);

			wasm_val_vec_delete(&result);
		}
	}

	return 0;
}
