#pragma once

#include "../guest_imports.hh"
#include "../wasm_util.hh"
#include "../wasm_ecsact_system_execution.h"

namespace ecsact::wasm::detail {

const auto guest_env_module_imports = allowed_guest_imports_t{
	{
		"ecsact_system_execution_context_action",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_2_0(
				wasm_valtype_new(WASM_I32), // context
				wasm_valtype_new(WASM_I32) // out_action_data
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_action
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"ecsact_system_execution_context_parent",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_1_1(
				wasm_valtype_new(WASM_I32), // context
				wasm_valtype_new(WASM_I32) // parent context (return)
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_parent
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"ecsact_system_execution_context_same",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_2_1(
				wasm_valtype_new(WASM_I32), // context a
				wasm_valtype_new(WASM_I32), // context b
				wasm_valtype_new(WASM_I32) // same bool (return)
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_same
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"ecsact_system_execution_context_get",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_3_0(
				wasm_valtype_new(WASM_I32), // context
				wasm_valtype_new(WASM_I32), // component_id
				wasm_valtype_new(WASM_I32) // out_component_data
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_get
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"ecsact_system_execution_context_update",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_3_0(
				wasm_valtype_new(WASM_I32), // context
				wasm_valtype_new(WASM_I32), // component_id
				wasm_valtype_new(WASM_I32) // component_data
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_update
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"ecsact_system_execution_context_has",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_2_0(
				wasm_valtype_new(WASM_I32), // context
				wasm_valtype_new(WASM_I32) // component_id
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_has
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"ecsact_system_execution_context_generate",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_4_0(
				wasm_valtype_new(WASM_I32), // context
				wasm_valtype_new(WASM_I32), // component_count
				wasm_valtype_new(WASM_I32), // component_ids
				wasm_valtype_new(WASM_I32) // components_data
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_generate
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"ecsact_system_execution_context_add",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_3_0(
				wasm_valtype_new(WASM_I32), // context
				wasm_valtype_new(WASM_I32), // component_id
				wasm_valtype_new(WASM_I32) // component_data
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_add
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"ecsact_system_execution_context_remove",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_2_0(
				wasm_valtype_new(WASM_I32), // context
				wasm_valtype_new(WASM_I32) // component_id
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_remove
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"ecsact_system_execution_context_other",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_2_1(
				wasm_valtype_new(WASM_I32), // context
				wasm_valtype_new(WASM_I32), // entity_id
				wasm_valtype_new(WASM_I32) // other context (return)
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_other
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"ecsact_system_execution_context_entity",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_1_1(
				wasm_valtype_new(WASM_I32), // context
				wasm_valtype_new(WASM_I32) // entity 9return)
			);
			wasm_func_t* fn = wasm_func_new(
				store,
				fn_type,
				&wasm_ecsact_system_execution_context_entity
			);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
};

}
