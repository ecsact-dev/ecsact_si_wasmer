#include "ecsactsi_wasi_fs.hh"

#include <map>
#include <cstdio>
#include <optional>

struct virtual_file_info {
	std::string virtual_path;
	std::string real_path;
	int32_t     pseudo_file_descriptor;

	// if set the file is open
	std::optional<std::FILE*> opened_file;
};

static auto last_file_descriptor = int32_t{10};
static auto virtual_files = std::map<int32_t, virtual_file_info>{};
static auto virtual_file_map = std::map<std::string, int32_t>{};

static auto erase_virtual_if_exists(std::string_view virtual_path) {
	const auto virtual_path_s = std::string{virtual_path};

	auto itr = virtual_file_map.find(virtual_path_s);
	if(itr != virtual_file_map.end()) {
		virtual_files.erase(itr->second);
		virtual_file_map.erase(itr);
	}
}

auto ecsactsi_wasi::detail::fs::allow_file_read_access(
	std::string_view real_path,
	std::string_view virtual_path
) -> std::int32_t {
	erase_virtual_if_exists(virtual_path);

	auto  fd = ++last_file_descriptor;
	auto& virtual_file_info = virtual_files[fd];
	virtual_file_info.pseudo_file_descriptor = fd;
	virtual_file_info.virtual_path = virtual_path;
	virtual_file_info.real_path = real_path;

	virtual_file_map[std::string{virtual_path}] = fd;

	return fd;
}

auto ecsactsi_wasi::detail::fs::real_path(int32_t fd) -> std::string {
	if(!virtual_files.contains(fd)) {
		return "";
	}

	return virtual_files.at(fd).real_path;
}

auto ecsactsi_wasi::detail::fs::real_path(std::string_view virtual_path)
	-> std::string {
	const auto virtual_path_s = std::string{virtual_path};

	auto itr = virtual_file_map.find(virtual_path_s);

	if(itr != virtual_file_map.end()) {
		return real_path(itr->second);
	}

	return "";
}

auto ecsactsi_wasi::detail::fs::ensure_open(int32_t pseudo_fd) -> std::FILE* {
	auto& info = virtual_files.at(pseudo_fd);
	if(info.opened_file) {
		return *info.opened_file;
	}

	info.opened_file = std::fopen(info.real_path.c_str(), "r");

	return *info.opened_file;
}

auto ecsactsi_wasi::detail::fs::close(int32_t pseudo_fd) -> void {
	auto& info = virtual_files.at(pseudo_fd);
	if(info.opened_file) {
		std::fclose(*info.opened_file);
		info.opened_file = std::nullopt;
	}
}
