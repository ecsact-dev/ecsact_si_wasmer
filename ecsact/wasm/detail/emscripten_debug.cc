#include "ecsact/wasm/detail/emscripten_debug.hh"

auto ecsact::wasm::detail::emscripten_debug_segfault(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) -> wasm_trap_t* {
	return nullptr;
}

auto ecsact::wasm::detail::emscripten_debug_alignfault(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) -> wasm_trap_t* {
	return nullptr;
}
