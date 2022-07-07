#include "ecsactsi_wasm.h"

#include <map>
#include <mutex>
#include <shared_mutex>
#include <wasm.h>
#include <ecsact/runtime/dynamic.h>

namespace {
	struct wasm_system_module_info {
		wasm_module_t* system_module = {};
		wasm_instance_t* instance = {};
		const wasm_func_t* system_impl_extern = {};
		wasm_store_t* store = {};
	};

	std::shared_mutex modules_mutex;
	std::map<ecsact_system_id, wasm_system_module_info> modules;

	wasm_engine_t* engine() {
		static wasm_engine_t* engine = wasm_engine_new();
		return engine;
	}

	void ecsactsi_wasm_system_impl
		( ecsact_system_execution_context* ctx
		)
	{
		std::shared_lock lk(modules_mutex);
		auto system_id = ecsact_system_execution_context_id(ctx);
		auto& m = modules.at(system_id);

		wasm_val_vec_t args = WASM_EMPTY_VEC;
		wasm_val_vec_t results = WASM_EMPTY_VEC;
		if(wasm_func_call(m.system_impl_extern, &args, &results)) {
			
		}
	}
}

ecsactsi_wasm_error ecsactsi_wasm_load_file
	( ecsact_system_id  system_id
	, wasm_module_t*    system_module
	)
{
	std::unique_lock lk(modules_mutex);
	if(modules.contains(system_id)) {
		
	}
	auto& m = modules[system_id];
	m.system_module = system_module;
	m.store = wasm_store_new(engine());
	m.instance = wasm_instance_new(m.store, m.system_module, nullptr, nullptr);
	wasm_instance_exports(m.instance, &m.exports);

	for(int i=0; m.exports.size > i; ++i) {
		wasm_externkind_t kind = wasm_extern_kind(m.exports.data[i]);
		if(kind == WASM_EXTERN_FUNC) {
			m.system_impl_extern = wasm_extern_as_func(m.exports.data[i]);
			break;
		}
	}

	if(m.system_impl_extern == nullptr) {
		return ECSACTSI_WASM_ERR_NO_VALID_EXPORT;
	}

	ecsact_set_system_execution_impl(system_id, &ecsactsi_wasm_system_impl);

	return ECSACTSI_WASM_OK;
}
