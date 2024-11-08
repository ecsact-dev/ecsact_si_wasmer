#pragma once

#include "ecsact/wasm/detail/guest_imports.hh"
#include "ecsact/wasm/detail/util.hh"
#include "ecsact/wasm/detail/emscripten_debug.hh"

namespace ecsact::wasm::detail {

const auto guest_emscripten_debug_module_imports = allowed_guest_imports_t{
	{
		"segfault",
		[]() -> minst_import_resolve_func {
			return {
				wasm_functype_new_0_0(),
				&emscripten_debug_segfault,
			};
		},
	},
	{
		"alignfault",
		[]() -> minst_import_resolve_func {
			return {
				wasm_functype_new_0_0(),
				&emscripten_debug_alignfault,
			};
		},
	}
};

} // namespace ecsact::wasm::detail
