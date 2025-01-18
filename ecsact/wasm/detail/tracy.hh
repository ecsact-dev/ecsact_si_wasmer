#pragma once

// We only want to use tracy if its enabled in si_wasm. Some users of Ecsact do
// not have tracy available so we cannot rely on the tracy header being
// available.

#ifdef TRACY_ENABLE
#	include <tracy/Tracy.hpp>
#endif

#ifdef TRACY_ENABLE
#	define ECSACT_SI_WASM_ZONE_SCOPED ZoneScoped
#else
#	define ECSACT_SI_WASM_ZONE_SCOPED static_assert(true, "requires semi colon")
#endif
