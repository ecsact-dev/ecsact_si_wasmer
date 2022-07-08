// #include <ecsact/runtime/dynamic.h>

#include "example/example.ecsact.hh"
#include "example/example.ecsact.systems.hh"

void example::ExampleSystem(ExampleSystem::context& ctx) {
	auto comp = ctx.get<example::ExampleComponent>();
	comp.num += 1;
	ctx.update(comp);
}
