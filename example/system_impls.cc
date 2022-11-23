#include "example.ecsact.hh"
#include "example.ecsact.systems.hh"

void example__ExampleSystem(ecsact_system_execution_context* c_ctx) {
	example::ExampleSystem::context ctx{ecsact::execution_context{c_ctx}};
	example::ExampleSystem::impl(ctx);
}

void example::ExampleSystem::impl(context& ctx) {
	auto comp = ctx.get<example::ExampleComponent>();
	comp.num += 1;
	ctx.update(comp);
}

void example__Generator(ecsact_system_execution_context* c_ctx) {
	example::Generator::context ctx{ecsact::execution_context{c_ctx}};
	example::Generator::impl(ctx);
}

void example::Generator::impl(context& ctx) {
	ctx._ctx.generate(example::WillAdd{});
}

void example__AddsSystem(ecsact_system_execution_context* c_ctx) {
	example::AddsSystem::context ctx{ecsact::execution_context{c_ctx}};
	example::AddsSystem::impl(ctx);
}

void example::AddsSystem::impl(context& ctx) {
	ctx.add(example::ExampleComponent{.num = 400});
}

void example__CheckShouldRemove(ecsact_system_execution_context* c_ctx) {
	example::CheckShouldRemove::context ctx{ecsact::execution_context{c_ctx}};
	example::CheckShouldRemove::impl(ctx);
}

void example::CheckShouldRemove::impl(context& ctx) {
	auto comp = ctx.get<example::ExampleComponent>();
	if(comp.num > 405) {
		ctx.add<example::WillRemove>();
	}
}
