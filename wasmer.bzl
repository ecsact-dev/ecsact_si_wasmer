"""
"""

_linux_wasmer_build_file_contents = """
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_import")

cc_import(
    name = "wasmer-c-api-lib",
    shared_library = "lib/libwasmer.so",
)

cc_library(
    name = "wasmer-c-api",
    visibility = ["//visibility:public"],
    strip_include_prefix = "include",
    defines = ["WASM_API_EXTERN="],
    hdrs = [
        "include/wasm.h",
        "include/wasmer.h",
        "include/wasmer_wasm.h",
    ],
    deps = [
        ":wasmer-c-api-lib",
    ],
)
"""

_windows_wasmer_build_file_contents = """
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_import")

cc_import(
    name = "wasmer-c-api-lib",
    interface_library = "lib/wasmer.lib",
    shared_library = "lib/wasmer.dll",
)

cc_library(
    name = "wasmer-c-api",
    visibility = ["//visibility:public"],
    strip_include_prefix = "include",
    defines = ["WASM_API_EXTERN="],
    linkopts = [
        "-DEFAULTLIB:ws2_32",
        "-DEFAULTLIB:Advapi32",
        "-DEFAULTLIB:Userenv",
        "-DEFAULTLIB:Bcrypt",
        "-DEFAULTLIB:ntdll",
        "-DEFAULTLIB:Shell32",
        "-DEFAULTLIB:Ole32",
    ],
    hdrs = [
        "include/wasm.h",
        "include/wasmer.h",
        "include/wasmer_wasm.h",
    ],
    deps = [
        ":wasmer-c-api-lib",
    ],
)
"""

def _wasmer_version(rctx, wasmer, using_system_wasmer = False):
    env = {}
    if not using_system_wasmer:
        env["WASMER_DIR"] = str(rctx.path("."))
    result = rctx.execute([wasmer, "--version"], environment = env)
    if result.return_code != 0:
        fail("wasmer config failed (exit code={}): {}".format(result.return_code, result.stderr))
    return result.stdout.strip()

def _wasmer_config(rctx, wasmer, args, using_system_wasmer = False):
    env = {}
    if not using_system_wasmer:
        env["WASMER_DIR"] = str(rctx.path("."))
    result = rctx.execute([wasmer, "config"] + args, environment = env)
    if result.return_code != 0:
        fail("wasmer config failed (exit code={}): {}".format(result.return_code, result.stderr))
    return result.stdout.strip()

def _is_windows(os):
    return os.name == "windows 10" or os.name == "windows server 2019"

def _wasmer_platform(os):
    if _is_windows(os):
        return "windows-amd64"
    if os.name == "linux":
        return "linux-amd64"

    fail("wasmer.bzl platform fail: {}".format(os.name))

def _wasmer_download_url(os, version):
    wasmer_platform = _wasmer_platform(os)
    return "https://github.com/wasmerio/wasmer/releases/download/v{}/wasmer-{}.tar.gz".format(version, wasmer_platform)

def _wasmer_repo(rctx):
    using_system_wasmer = False
    wasmer = None

    if rctx.attr.wasmer_version:
        rctx.download_and_extract(
            url = _wasmer_download_url(rctx.os, rctx.attr.wasmer_version),
        )

        if _is_windows(rctx.os):
            wasmer = rctx.path("bin/wasmer.exe")
        else:
            wasmer = rctx.path("bin/wasmer")

    if not wasmer:
        using_system_wasmer = True
        wasmer = rctx.which("wasmer")
        if not wasmer:
            fail("Cannot find 'wasmer' in PATH")

    version = _wasmer_version(rctx, wasmer, using_system_wasmer = using_system_wasmer)
    if rctx.attr.wasmer_version:
        if not version.endswith(rctx.attr.wasmer_version):
            fail("Expected wasmer version {}, but got {}".format(rctx.attr.wasmer_version, version))

    includedir = _wasmer_config(rctx, wasmer, ["--includedir"], using_system_wasmer = using_system_wasmer)
    libdir = _wasmer_config(rctx, wasmer, ["--libdir"], using_system_wasmer = using_system_wasmer)

    if using_system_wasmer:
        rctx.symlink(includedir, "include")
        rctx.symlink(libdir, "lib")

    build_file_contents = {
        "linux": _linux_wasmer_build_file_contents,
        "windows 10": _windows_wasmer_build_file_contents,
        "windows server 2019": _windows_wasmer_build_file_contents,
    }

    if not rctx.os.name in build_file_contents:
        fail("unsupported repository os: {}".format(rctx.os.name))

    rctx.file("BUILD.bazel", content = build_file_contents[rctx.os.name], executable = False)

wasmer_repo = repository_rule(
    implementation = _wasmer_repo,
    attrs = {
        "wasmer_version": attr.string(mandatory = True),
    },
)
