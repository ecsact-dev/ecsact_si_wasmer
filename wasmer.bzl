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

def _wasmer_config(rctx, wasmer, args):
    result = rctx.execute([wasmer, "config"] + args)
    if result.return_code != 0:
        fail("wasmer config failed (exit_code={}): {}".format(result.exit_code, result.stderr))
    return result.stdout.strip()

def _wasmer_repo(rctx):
    wasmer = rctx.which("wasmer")
    if not wasmer:
        fail("Cannot find 'wasmer' in PATH")

    includedir = _wasmer_config(rctx, wasmer, ["--includedir"])
    libdir = _wasmer_config(rctx, wasmer, ["--libdir"])
    rctx.symlink(includedir, "include")
    rctx.symlink(libdir, "lib")

    build_file_contents = {
        "linux": _linux_wasmer_build_file_contents,
        "windows 10": _windows_wasmer_build_file_contents,
    }

    if not rctx.os.name in build_file_contents:
        fail("unsupported repository os: {}".format(rctx.os.name))

    rctx.file("BUILD.bazel", content = build_file_contents[rctx.os.name], executable = False)

wasmer_repo = repository_rule(
    implementation = _wasmer_repo,
)
