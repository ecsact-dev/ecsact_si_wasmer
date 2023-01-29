#include "ecsact/runtime/dynamic.h"

#include "mock_set_system_impl.hh"

void ecsact_system_execution_context_action(
	ecsact_system_execution_context* context,
	void*                            out_action_data
) {
}

void ecsact_system_execution_context_add(
	ecsact_system_execution_context* context,
	ecsact_component_like_id         component_id,
	const void*                      component_data
) {
}

void ecsact_system_execution_context_remove(
	ecsact_system_execution_context* context,
	ecsact_component_like_id         component_id
) {
}

void ecsact_system_execution_context_get(
	ecsact_system_execution_context* context,
	ecsact_component_like_id         component_id,
	void*                            out_component_data
) {
}

void ecsact_system_execution_context_update(
	ecsact_system_execution_context* context,
	ecsact_component_like_id         component_id,
	const void*                      component_data
) {
}

bool ecsact_system_execution_context_has(
	ecsact_system_execution_context* context,
	ecsact_component_like_id         component_id
) {
	return false;
}

void ecsact_system_execution_context_generate(
	ecsact_system_execution_context* context,
	int                              component_count,
	ecsact_component_id*             component_ids,
	const void**                     components_data
) {
}

const ecsact_system_execution_context* ecsact_system_execution_context_parent(
	ecsact_system_execution_context* context
) {
	return nullptr;
}

bool ecsact_system_execution_context_same(const ecsact_system_execution_context*, const ecsact_system_execution_context*) {
	return true;
}

ecsact_system_execution_context* ecsact_system_execution_context_other(
	ecsact_system_execution_context* context,
	ecsact_entity_id                 entity_id
) {
	return nullptr;
}

ecsact_entity_id ecsact_system_execution_context_entity(
	const ecsact_system_execution_context* context
) {
	return static_cast<ecsact_entity_id>(0);
}

ecsact_system_like_id ecsact_system_execution_context_id(
	ecsact_system_execution_context* context
) {
	return static_cast<ecsact_system_like_id>(0);
}

bool ecsact_set_system_execution_impl(
	ecsact_system_like_id        system_id,
	ecsact_system_execution_impl system_exec_impl
) {
	ecsactsi_wasm_tester::set_impl(system_exec_impl);
	return true;
}
