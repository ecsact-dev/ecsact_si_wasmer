#include "ecsact/wasm.h"

#include <map>
#include <unordered_map>
#include <mutex>
#include <variant>
#include <vector>
#include <string>
#include <optional>
#include <string_view>
#include <shared_mutex>
#include <cassert>
#include <memory>
#include <wasm.h>
#include <wasmer.h>
#include <cstdlib>
#include <cstdio>
#include <functional>
#include <iostream>
#include <array>
#include <cstddef>
#include "ecsact/runtime/dynamic.h"
#include "ecsact/runtime/meta.h"
#include "ecsact/wasm/detail/minst/minst.hh"
#include "ecsact/wasm/detail/logger.hh"
#include "ecsact/wasm/detail/wasi_fs.hh"

using namespace std::string_literals;
using ecsact::wasm::detail::clear_log_lines;
using ecsact::wasm::detail::consume_stdio_str_as_log_lines;
using ecsact::wasm::detail::get_log_lines;
using ecsact::wasm::detail::start_transaction;

namespace {

struct minst_ecsact_system_impl_export {
	ecsact_system_like_id system_id;
	size_t                export_index;
};

struct minst_ecsact_system_impls {
	std::shared_ptr<std::vector<std::byte>>      wasm_data;
	ecsact::wasm::detail::minst                  minst;
	std::vector<minst_ecsact_system_impl_export> system_impl_exports;
};

auto trap_handler = ecsactsi_wasm_trap_handler{};

void ecsactsi_wasm_system_impl(ecsact_system_execution_context* ctx) {
	auto system_id = ecsact_system_execution_context_id(ctx);
}
} // namespace

void ecsactsi_wasm_last_error_message(
	char*   out_message,
	int32_t message_max_length
) {
	// auto& last_error_message = ecsactsi_wasm::detail::get_last_error_message();
	// std::copy_n(
	// 	last_error_message.begin(),
	// 	std::min(
	// 		message_max_length,
	// 		static_cast<int32_t>(last_error_message.size())
	// 	),
	// 	out_message
	// );
}

int32_t ecsactsi_wasm_last_error_message_length() {
	return 0;
	// return static_cast<int32_t>(
	// 	ecsactsi_wasm::detail::get_last_error_message().size()
	// );
}

ecsactsi_wasm_error ecsactsi_wasm_load(
	char*                  wasm_data,
	int                    wasm_data_size,
	int                    systems_count,
	ecsact_system_like_id* system_ids,
	const char**           wasm_exports
) {
	// auto result = ecsact::wasm::detail::minst::create();
	return ECSACTSI_WASM_ERR_COMPILE_FAIL;
}

ecsactsi_wasm_error ecsactsi_wasm_load_file(
	const char*            wasm_file_path,
	int                    systems_count,
	ecsact_system_like_id* system_ids,
	const char**           wasm_exports
) {
	FILE* file = std::fopen(wasm_file_path, "rb");
	if(!file) {
		return ECSACTSI_WASM_ERR_FILE_OPEN_FAIL;
	}

	auto binary_data = std::make_shared<std::vector<std::byte>>();

	std::fseek(file, 0L, SEEK_END);
	auto file_size = std::ftell(file);
	std::fseek(file, 0L, SEEK_SET);
	binary_data->resize(file_size);

	if(std::fread(binary_data->data(), file_size, 1, file) != 1) {
		return ECSACTSI_WASM_ERR_FILE_READ_FAIL;
	}

	return ecsactsi_wasm_load(
		reinterpret_cast<char*>(binary_data->data()),
		static_cast<int>(binary_data->size()),
		systems_count,
		system_ids,
		wasm_exports
	);
}

void ecsactsi_wasm_set_trap_handler(ecsactsi_wasm_trap_handler handler) {
	trap_handler = handler;
}

void ecsactsi_wasm_unload(
	int                    systems_count,
	ecsact_system_like_id* system_ids
) {
}

void ecsactsi_wasm_reset() {
}

void ecsactsi_wasm_consume_logs(
	ecsactsi_wasm_log_consumer consumer,
	void*                      consumer_user_data
) {
	auto t = start_transaction();
	for(auto& entry : get_log_lines(t)) {
		consumer(
			entry.log_level,
			entry.message.c_str(),
			static_cast<int32_t>(entry.message.size()),
			consumer_user_data
		);
	}

	for(auto& entry : consume_stdio_str_as_log_lines(t)) {
		consumer(
			entry.log_level,
			entry.message.c_str(),
			static_cast<int32_t>(entry.message.size()),
			consumer_user_data
		);
	}

	clear_log_lines(t);
}

int32_t ecsactsi_wasm_allow_file_read_access(
	const char* real_file_path,
	int32_t     real_file_path_length,
	const char* virtual_file_path,
	int32_t     virtual_file_path_length
) {
	return ecsact::wasm::detail::wasi::fs::allow_file_read_access(
		std::string_view{
			real_file_path,
			static_cast<size_t>(real_file_path_length),
		},
		std::string_view{
			virtual_file_path,
			static_cast<size_t>(virtual_file_path_length),
		}
	);
}
