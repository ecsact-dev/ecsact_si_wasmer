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
