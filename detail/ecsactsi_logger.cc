#include "ecsactsi_wasm.h"
#include "ecsactsi_logger.hh"

#include <map>
#include <string>
#include <string_view>
#include <ranges>

static auto _logger_mutex = std::recursive_mutex{};
static auto _logger_entries =
	std::vector<ecsactsi_wasm::detail::log_line_entry>{};
static auto _stdio_strings = [] {
	return std::map<ecsactsi_wasm_log_level, std::string>{
		{ECSACTSI_WASM_LOG_LEVEL_INFO, {}},
		{ECSACTSI_WASM_LOG_LEVEL_ERROR, {}},
	};
}();

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

auto ecsactsi_wasm::detail::push_stdio_str(
	ecsactsi_wasm_log_level level,
	std::string_view        str
) -> void {
	std::scoped_lock lk(_logger_mutex);
	_stdio_strings[level] += std::string(str);
}

auto ecsactsi_wasm::detail::
	consume_stdio_str_as_log_lines(const log_transaction&)
		-> std::vector<log_line_entry> {
	using std::views::split;
	using namespace std::string_view_literals;

	auto result = std::vector<log_line_entry>{};

	for(auto&& [log_level, str] : _stdio_strings) {
		for(const auto& line : str | split("\n"sv)) {
			auto message = std::string(line.begin(), line.end());
			if(message.empty()) {
				continue;
			}

			result.push_back(log_line_entry{
				.log_level = log_level,
				.message = message,
			});
		}
	}

	_stdio_strings.clear();

	return result;
}
