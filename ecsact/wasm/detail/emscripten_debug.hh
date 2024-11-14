#pragma once

#include <wasm.h>

namespace ecsact::wasm::detail {
auto emscripten_debug_segfault(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) -> wasm_trap_t*;

auto emscripten_debug_alignfault(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) -> wasm_trap_t*;
} // namespace ecsact::wasm::detail
