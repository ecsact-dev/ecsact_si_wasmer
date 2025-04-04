#pragma once

#include "ecsact/si/wasmer/detail/guest_imports.hh"
#include "ecsact/si/wasmer/detail/util.hh"
#include "ecsact/si/wasmer/detail/wasm_ecsact_system_execution.h"

namespace ecsact::wasm::detail {

const auto guest_env_module_imports = allowed_guest_imports_t{
	{
		"memory",
		[](const minst_import imp) -> minst_import_resolve_t {
			return minst_import_resolve_memory{
				.memory_limits = wasm_limits_t{1, 10},
			};
		},
	},
	{
		"__indirect_function_table",
		[](const minst_import imp) -> minst_import_resolve_t {
			return minst_import_resolve_table{
				.table_type = imp.as_tabletype(),
			};
		},
	},
	{
		"__stack_pointer",
		[](const minst_import imp) -> minst_import_resolve_t {
			return minst_import_resolve_global{};
		},
	},
	{
		"__memory_base",
		[](const minst_import imp) -> minst_import_resolve_t {
			return minst_import_resolve_global{};
		},
	},
	{
		"__table_base",
		[](const minst_import imp) -> minst_import_resolve_t {
			return minst_import_resolve_global{};
		},
	},
	{
		"ecsact_system_execution_context_action",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_2_0(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32) // out_action_data
				),
				&wasm_ecsact_system_execution_context_action,
			};
		},
	},
	{
		"ecsact_system_execution_context_parent",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_1_1(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32) // parent context (return)
				),
				&wasm_ecsact_system_execution_context_parent,
			};
		},
	},
	{
		"ecsact_system_execution_context_same",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_2_1(
					wasm_valtype_new(WASM_I32), // context a
					wasm_valtype_new(WASM_I32), // context b
					wasm_valtype_new(WASM_I32) // same bool (return)
				),
				&wasm_ecsact_system_execution_context_same,
			};
		},
	},
	{
		"ecsact_system_execution_context_get",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_4_0(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32), // component_id
					wasm_valtype_new(WASM_I32), // out_component_data
					wasm_valtype_new(WASM_I32) // indexed_fields
				),
				&wasm_ecsact_system_execution_context_get,
			};
		},
	},
	{
		"ecsact_system_execution_context_update",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_4_0(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32), // component_id
					wasm_valtype_new(WASM_I32), // component_data
					wasm_valtype_new(WASM_I32) // indexed_fields
				),
				&wasm_ecsact_system_execution_context_update,
			};
		},
	},
	{
		"ecsact_system_execution_context_has",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_3_0(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32), // component_id
					wasm_valtype_new(WASM_I32) // indexed_fields
				),
				&wasm_ecsact_system_execution_context_has,
			};
		},
	},
	{
		"ecsact_system_execution_context_generate",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_4_0(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32), // component_count
					wasm_valtype_new(WASM_I32), // component_ids
					wasm_valtype_new(WASM_I32) // components_data
				),
				&wasm_ecsact_system_execution_context_generate,
			};
		},
	},
	{
		"ecsact_system_execution_context_add",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_3_0(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32), // component_id
					wasm_valtype_new(WASM_I32) // component_data
				),
				&wasm_ecsact_system_execution_context_add,
			};
		},
	},
	{
		"ecsact_system_execution_context_remove",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_3_0(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32), // component_id
					wasm_valtype_new(WASM_I32) // indexed_fields
				),
				&wasm_ecsact_system_execution_context_remove,
			};
		},
	},
	{
		"ecsact_system_execution_context_other",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_2_1(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32), // entity_id
					wasm_valtype_new(WASM_I32) // other context (return)
				),
				&wasm_ecsact_system_execution_context_other,
			};
		},
	},
	{
		"ecsact_system_execution_context_entity",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_1_1(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32) // entity 9return)
				),
				&wasm_ecsact_system_execution_context_entity,
			};
		},
	},
	{
		"ecsact_system_execution_context_stream_toggle",
		[](const minst_import) -> minst_import_resolve_t {
			return minst_import_resolve_func{
				wasm_functype_new_4_0(
					wasm_valtype_new(WASM_I32), // context
					wasm_valtype_new(WASM_I32), // component_id
					wasm_valtype_new(WASM_I32), // streaming_enabled
					wasm_valtype_new(WASM_I32) // indexed_fields
				),
				&wasm_ecsact_system_execution_context_stream_toggle,
			};
		},
	},
};

} // namespace ecsact::wasm::detail
