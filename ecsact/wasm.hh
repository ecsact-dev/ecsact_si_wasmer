#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string_view>
#include <span>
#include <concepts>
#include <print>
#include "ecsact/wasm.h"

namespace ecsact::wasm {

inline auto last_error_message() -> std::string {
	auto message = std::string{};
	message.resize(ecsactsi_wasm_last_error_message_length());
	ecsactsi_wasm_last_error_message(
		message.data(),
		static_cast<std::int32_t>(message.size())
	);
	return message;
}

struct system_load_options {
	ecsact_system_like_id system_id;
	std::string_view      wasm_export;
};

inline auto load_file(
	std::filesystem::path          wasm_file_path,
	std::span<system_load_options> systems
) -> ecsactsi_wasm_error {
	auto system_ids = std::vector<ecsact_system_like_id>{};
	auto wasm_exports = std::vector<std::string>{};
	system_ids.reserve(systems.size());
	wasm_exports.reserve(systems.size());

	for(auto& sys : systems) {
		system_ids.emplace_back(sys.system_id);
		wasm_exports.emplace_back(sys.wasm_export);
	}

	auto wasm_exports_c = std::vector<const char*>{};
	wasm_exports_c.reserve(systems.size());
	for(auto& wasm_export : wasm_exports) {
		wasm_exports_c.emplace_back(wasm_export.c_str());
	}

	return ecsactsi_wasm_load_file(
		wasm_file_path.string().c_str(),
		static_cast<std::int32_t>(systems.size()),
		system_ids.data(),
		wasm_exports.data()
	);
}

inline auto load(
	std::span<std::byte>           wasm_data,
	std::span<system_load_options> systems
) -> ecsactsi_wasm_error {
	auto system_ids = std::vector<ecsact_system_like_id>{};
	auto wasm_exports = std::vector<std::string>{};
	system_ids.reserve(systems.size());
	wasm_exports.reserve(systems.size());

	for(auto& sys : systems) {
		system_ids.emplace_back(sys.system_id);
		wasm_exports.emplace_back(sys.wasm_export);
	}

	auto wasm_exports_c = std::vector<const char*>{};
	wasm_exports_c.reserve(systems.size());
	for(auto& wasm_export : wasm_exports) {
		wasm_exports_c.emplace_back(wasm_export.c_str());
	}

	return ecsactsi_wasm_load(
		static_cast<char*>(wasm_data.data()),
		static_cast<std::int32_t>(wasm_data.size()),
		static_cast<std::int32_t>(systems.size()),
		system_ids.data(),
		wasm_exports.data()
	);
}

inline auto unload(std::span<ecsact_system_like_id> systems) -> void {
	ecsactsi_wasm_unload(
		static_cast<std::int32_t>(systems.size()),
		systems.data()
	);
}

inline auto reset() -> void {
	ecsactsi_wasm_reset();
}

inline auto set_trap_handler(ecsactsi_wasm_trap_handler handler) -> void {
	ecsactsi_wasm_set_trap_handler(handler);
}

enum class log_level {
	info = 0,
	warning = 1,
	error = 2,
};

template<typename T>
concept log_consumer = std::invocable<T, log_level, std::string_view>;

auto consume_logs(log_consumer auto&& f) -> void {
	void* user_data = &f;
	ecsactsi_wasm_consume_logs(
		[](
			ecsactsi_wasm_log_level level,
			const char*             message,
			std::int32_t            message_length,
			void*                   user_data
		) {
			static_cast<decltype(&f)>(user_data)(
				reinterpret_cast<log_level>(level),
				std::string_view{message, static_cast<std::size_t>(message_length)}
			);
		},
		user_data
	);
}

inline auto consume_and_print_logs() -> void {
	consume_logs([](log_level l, std::string_view m) {
		switch(l) {
			default:
			case log_level::info:
				std::println(stdout, "[INFO] {}", m);
				break;
			case log_level::warning:
				std::println(stderr, "[WARNING] {}", m);
				break;
			case log_level::error:
				std::println(stderr, "[ERROR] {}", m);
				break;
		}
	});
}

auto allow_file_read_access( //
	std::filesystem::path real_file_path,
	std::string_view      virtual_file_path
) -> std::int32_t {
	auto real_file_path_str = real_file_path.string();
	return ecsactsi_wasm_allow_file_read_access(
		real_file_path_str.c_str(),
		static_cast<std::int32_t>(real_file_path_str.size()),
		virtual_file_path.data(),
		static_cast<std::int32_t>(virtual_file_path.size())
	);
}
} // namespace ecsact::wasm
