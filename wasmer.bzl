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
        fail("wasmer config failed (exit code={}): {}".format(result.return_code, result.stderr))
    return result.stdout.strip()

def _wasmer_platform(os):
    if os.name == "windows 10":
        return "windows-amd64"
    if os.name == "linux":
        return "linux-amd64"

    fail("wasmer.bzl platform fail: {}".format(os.name))

def _wasmer_download_url(os, version):
    wasmer_platform = _wasmer_platform(os)
    return "https://github.com/wasmerio/wasmer/releases/download/{}/wasmer-{}.tar.gz".format(version, wasmer_platform)

def _wasmer_repo(rctx):
    using_system_wasmer = False
    wasmer = None

    if rctx.attr.wasmer_version:
        rctx.download_and_extract(
            url = _wasmer_download_url(rctx.os, rctx.attr.wasmer_version),
        )

        if rctx.os.name == "windows 10":
            wasmer = rctx.path("bin/wasmer.exe")
        else:
            wasmer = rctx.path("bin/wasmer")

    if not wasmer:
        wasmer = rctx.which("wasmer")
        if not wasmer:
            fail("Cannot find 'wasmer' in PATH")

    version = _wasmer_config(rctx, wasmer, ["--version"])
    if rctx.attr.wasmer_version:
        if not version.endswith(rctx.attr.wasmer_version):
            fail("Expected wasmer version {}, but got {}".format(rctx.attr.wasmer_version, version))

    includedir = _wasmer_config(rctx, wasmer, ["--includedir"])
    libdir = _wasmer_config(rctx, wasmer, ["--libdir"])

    if using_system_wasmer:
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
    attrs = {
        "wasmer_version": attr.string(),
    },
)
