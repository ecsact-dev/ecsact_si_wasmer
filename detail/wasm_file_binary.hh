#pragma once

#include <wasm.h>
#include <cstdio>

namespace ecsactsi_wasm::detail {
// Utility to load wasm file which automatically cleans up upon destruction
struct wasm_file_binary {
	wasm_byte_vec_t binary = {};

	static bool read(FILE* file, wasm_file_binary& out_file) {
		std::fseek(file, 0L, SEEK_END);
		auto file_size = std::ftell(file);
		std::fseek(file, 0L, SEEK_SET);
		wasm_byte_vec_new_uninitialized(&out_file.binary, file_size);

		if(std::fread(out_file.binary.data, file_size, 1, file) != 1) {
			return false;
		}

		return true;
	}

	~wasm_file_binary() {
		if(binary.data != nullptr) {
			wasm_byte_vec_delete(&binary);
			binary = {};
		}
	}
};
} // namespace ecsactsi_wasm::detail
