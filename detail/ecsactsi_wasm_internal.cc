#include "ecsactsi_wasm_internal.hh"

#include <vector>
#include <string>
#include <string_view>
#include <wasm.h>
#include <wasmer.h>

#include "wasm_ecsact_memory.hh"
#include "guest_imports.hh"
#include "guest_imports/wasi_snapshot_preview1.hh"
#include "guest_imports/env.hh"

static thread_local std::string _last_error_message;

auto ecsactsi_wasm::detail::get_last_error_message() -> const std::string& {
	return _last_error_message;
}

auto ecsactsi_wasm::detail::set_last_error_message(const std::string& str)
	-> void {
	_last_error_message = str;
}

void ecsactsi_wasm::detail::set_wasmer_last_error_message() {
	_last_error_message.resize(wasmer_last_error_length());
	if(!_last_error_message.empty()) {
		wasmer_last_error_message(
			_last_error_message.data(),
			static_cast<int>(_last_error_message.size())
		);
	}
}

auto ecsactsi_wasm::detail::engine() -> wasm_engine_t* {
	static wasm_engine_t* engine = wasm_engine_new();
	return engine;
}

auto ecsactsi_wasm::detail::trap_message(wasm_trap_t* trap) -> std::string {
	wasm_message_t trap_msg;
	wasm_trap_message(trap, &trap_msg);
	auto trap_msg_str = std::string{trap_msg.data, trap_msg.size - 1};
	auto origin = wasm_trap_origin(trap);

	if(origin != nullptr) {
		auto fn_index = wasm_frame_func_index(origin);
		auto fn_offset = wasm_frame_func_offset(origin);

		trap_msg_str += " (func_index=" + std::to_string(fn_index) + ", ";
		trap_msg_str += "func_offset=" + std::to_string(fn_offset) + ")";
		wasm_frame_delete(origin);
	} else {
		trap_msg_str += " (unknown trap origin)";
	}

	return trap_msg_str;
}

auto ecsactsi_wasm::detail::load_modules(
	char*                  wasm_data,
	int                    wasm_data_size,
	int                    systems_count,
	ecsact_system_like_id* system_ids,
	const char**           wasm_exports
) -> load_modules_result_t {
	const auto allowed_guest_modules = allowed_guest_modules_t{
		{"env", guest_env_module_imports},
		{"wasi_snapshot_preview1", guest_wasi_module_imports},
	};

	wasm_byte_vec_t binary{
		.size = static_cast<size_t>(wasm_data_size),
		.data = wasm_data,
	};

	auto pending_modules =
		std::map<ecsact_system_like_id, ecsact_internal_wasm_system_module_info>{};

	for(int index = 0; systems_count > index; ++index) {
		auto  system_id = system_ids[index];
		auto& pending_info = pending_modules[system_id] = {};
		pending_info.store = wasm_store_new(engine());

		// There needs to be one module and one store per system for thread safety
		pending_info.system_module = wasm_module_new(pending_info.store, &binary);

		if(!pending_info.system_module) {
			set_wasmer_last_error_message();
			return ECSACTSI_WASM_ERR_COMPILE_FAIL;
		}

		wasm_importtype_vec_t imports;
		wasm_exporttype_vec_t exports;
		wasm_module_imports(pending_info.system_module, &imports);
		wasm_module_exports(pending_info.system_module, &exports);
		int system_impl_export_memory_index = -1;
		int system_impl_export_function_index = -1;

		for(size_t expi = 0; exports.size > expi; ++expi) {
			auto export_name = wasm_exporttype_name(exports.data[expi]);
			auto export_type = wasm_exporttype_type(exports.data[expi]);
			auto export_type_kind =
				static_cast<wasm_externkind_enum>(wasm_externtype_kind(export_type));

			if(export_type_kind == WASM_EXTERN_MEMORY) {
				system_impl_export_memory_index = expi;
			}

			std::string_view export_name_str(export_name->data, export_name->size);
			if(export_name_str == std::string_view(wasm_exports[index])) {
				if(export_type_kind != WASM_EXTERN_FUNC) {
					return ECSACTSI_WASM_ERR_EXPORT_INVALID;
				}

				system_impl_export_function_index = expi;
			}
		}

		if(system_impl_export_function_index == -1) {
			return ECSACTSI_WASM_ERR_EXPORT_NOT_FOUND;
		}

		std::vector<wasm_extern_t*> externs;
		externs.reserve(std::min(static_cast<size_t>(8), imports.size));
		for(size_t impi = 0; imports.size > impi; ++impi) {
			auto import_module = wasm_importtype_module(imports.data[impi]);
			auto import_name = wasm_importtype_name(imports.data[impi]);
			auto import_type = wasm_importtype_type(imports.data[impi]);
			auto import_type_kind =
				static_cast<wasm_externkind_enum>(wasm_externtype_kind(import_type));

			auto import_name_str = std::string(import_name->data, import_name->size);
			auto module_name_str =
				std::string(import_module->data, import_module->size);

			if(!allowed_guest_modules.contains(module_name_str)) {
				ecsactsi_wasm::detail::set_last_error_message(
					"'" + module_name_str + "' is not an allowed guest import module"
				);
				return ECSACTSI_WASM_ERR_GUEST_IMPORT_UNKNOWN;
			}

			auto allowed_imports = allowed_guest_modules.at(module_name_str);

			if(!allowed_imports.contains(import_name_str)) {
				ecsactsi_wasm::detail::set_last_error_message(
					"'" + module_name_str + "'.'" + import_name_str +
					"' is not an allowed guest import. Please see "
					"https://ecsact.dev/docs/system-impl-wasm"
				);
				return ECSACTSI_WASM_ERR_GUEST_IMPORT_UNKNOWN;
			}

			if(import_type_kind != WASM_EXTERN_FUNC) {
				ecsactsi_wasm::detail::set_last_error_message(
					"'" + import_name_str +
					"' is the wrong import kind. Expected WASM_EXTERN_FUNC."
				);
				return ECSACTSI_WASM_ERR_GUEST_IMPORT_INVALID;
			}

			auto guest_import_fn =
				allowed_imports.at(import_name_str)(pending_info.store);
			externs.push_back(wasm_func_as_extern(guest_import_fn));

			// TODO(zaucy): Determine if we need to delete function here or later
			// wasm_func_delete(guest_import_fn);
		}

		wasm_extern_vec_t instance_externs{
			.size = externs.size(),
			.data = externs.data(),
		};

		pending_info.instance = wasm_instance_new(
			pending_info.store,
			pending_info.system_module,
			&instance_externs,
			nullptr
		);

		if(!pending_info.instance) {
			set_wasmer_last_error_message();
			return ECSACTSI_WASM_ERR_INSTANTIATE_FAIL;
		}

		wasm_extern_vec_t inst_exports;
		wasm_instance_exports(pending_info.instance, &inst_exports);

		{
			auto fn_extern = inst_exports.data[system_impl_export_function_index];
			pending_info.system_impl_func = wasm_extern_as_func(fn_extern);
		}

		if(system_impl_export_memory_index != -1) {
			assert(inst_exports.size > system_impl_export_memory_index);
			auto mem_extern = inst_exports.data[system_impl_export_memory_index];
			pending_info.system_impl_memory = wasm_extern_as_memory(mem_extern);
		}
	}

	return pending_modules;
}

auto ecsactsi_wasm::detail::init_module(
	ecsact_internal_wasm_system_module_info& info
) -> wasm_trap_t* {
	auto exports = wasm_exporttype_vec_t{};
	auto inst_exports = wasm_extern_vec_t{};
	wasm_instance_exports(info.instance, &inst_exports);
	wasm_module_exports(info.system_module, &exports);

	for(size_t i = 0; exports.size > i; ++i) {
		auto export_type = wasm_exporttype_type(exports.data[i]);
		auto export_type_kind =
			static_cast<wasm_externkind_enum>(wasm_externtype_kind(export_type));

		if(export_type_kind != WASM_EXTERN_FUNC) {
			continue;
		}

		auto             export_name = wasm_exporttype_name(exports.data[i]);
		std::string_view export_name_str(export_name->data, export_name->size);
		if(export_name_str == "_initialize") {
			auto init_fn_export = inst_exports.data[i];
			auto init_fn = wasm_extern_as_func(init_fn_export);

			wasm_val_vec_t args = WASM_EMPTY_VEC;
			wasm_val_vec_t result = WASM_EMPTY_VEC;

			set_current_wasm_memory(info.system_impl_memory);
			auto trap = wasm_func_call(init_fn, &args, &result);
			set_current_wasm_memory(nullptr);
			return trap;
		}
	}

	return nullptr;
}
