#include "ecsactsi_wasm.h"
#include "ecsactsi_logger.hh"

static auto _logger_mutex = std::recursive_mutex{};
static auto _logger_entries =
	std::vector<ecsactsi_wasm::detail::log_line_entry>{};

ecsactsi_wasm::detail::log_transaction::log_transaction(std::recursive_mutex& m)
	: _lk(m) {
}

ecsactsi_wasm::detail::log_transaction::log_transaction(log_transaction&& other)
	: _lk(std::move(other._lk)) {
}

ecsactsi_wasm::detail::log_transaction::~log_transaction() {
}

auto ecsactsi_wasm::detail::start_transaction() -> log_transaction {
	return log_transaction{_logger_mutex};
}

auto ecsactsi_wasm::detail::push_log_line(
	const log_transaction&,
	log_line_entry entry
) -> void {
	_logger_entries.emplace_back(std::move(entry));
}

auto ecsactsi_wasm::detail::get_log_lines(const log_transaction&)
	-> const std::vector<log_line_entry>& {
	return _logger_entries;
}

auto ecsactsi_wasm::detail::clear_log_lines(const log_transaction&) -> void {
	_logger_entries.clear();
}
