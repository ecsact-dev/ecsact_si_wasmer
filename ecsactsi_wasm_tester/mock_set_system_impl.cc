#include "mock_set_system_impl.hh"

static ecsact_system_execution_impl _impl = nullptr;

auto ecsactsi_wasm_tester::get_impl() -> ecsact_system_execution_impl {
	return _impl;
}

auto ecsactsi_wasm_tester::set_impl(ecsact_system_execution_impl impl) -> void {
	_impl = impl;
}
