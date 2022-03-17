project('gpuRecorder', ['c', 'cpp'], default_options: ['optimization=0', 'debug=true', 'warning_level=1'])

windows = import('windows')
cpp = meson.get_compiler('cpp')
host_system = host_machine.system()

gpuRecorder_src = [
	'gpuRecorder/about.cpp',
	'gpuRecorder/config.cpp',
	'gpuRecorder/externals.cpp',
	'gpuRecorder/folder.cpp',
	'gpuRecorder/gpu.cpp',
	'gpuRecorder/gpuRecorder.cpp',
	'gpuRecorder/keys.cpp',
	'gpuRecorder/log.cpp',
	'gpuRecorder/psemu.cpp',
	'gpuRecorder/record.cpp',
	'gpuRecorder/reg.cpp',
	'gpuRecorder/setup.cpp',
	'gpuRecorder/stdafx.cpp',
	'external/zlib/adler32.c',
	'external/zlib/compress.c',
	'external/zlib/crc32.c',
	'external/zlib/deflate.c',
	'external/zlib/inffast.c',
	'external/zlib/inflate.c',
	'external/zlib/inftrees.c',
	'external/zlib/trees.c',
	'external/zlib/uncompr.c',
	'external/zlib/zutil.c',
]
gpuRecorder_rc_src = [
	'gpuRecorder/gpuRecorder.rc',
]
gpuRecorder_src += windows.compile_resources(gpuRecorder_rc_src)
gpuRecorder_include_directories = [
	'gpuRecorder',
	'external/zlib',
]
gpuRecorder_cppflags = []
gpuRecorder_ldflags = []
if host_system == 'windows' and cpp.get_id() != 'msvc' and cpp.get_id() != 'clang-cl'
	gpuRecorder_ldflags += ['-static']
endif
gpuRecorder_dependson = []

gpuRecorder = shared_library('gpuRecorder', sources: gpuRecorder_src, include_directories: include_directories(gpuRecorder_include_directories), cpp_args: gpuRecorder_cppflags, link_args: gpuRecorder_ldflags, dependencies: gpuRecorder_dependson, install: true)

spuRecorder_src = [
	'spuRecorder/about.cpp',
	'spuRecorder/config.cpp',
	'spuRecorder/externals.cpp',
	'spuRecorder/folder.cpp',
	'spuRecorder/log.cpp',
	'spuRecorder/psemu.cpp',
	'spuRecorder/record.cpp',
	'spuRecorder/reg.cpp',
	'spuRecorder/setup.cpp',
	'spuRecorder/spu.cpp',
	'spuRecorder/spuRecorder.cpp',
	'spuRecorder/stdafx.cpp',
]
spuRecorder_rc_src = [
	'spuRecorder/spuRecorder.rc',
]
spuRecorder_src += windows.compile_resources(spuRecorder_rc_src)
spuRecorder_include_directories = [
	'spuRecorder',
	'external/zlib',
]
spuRecorder_cppflags = []
spuRecorder_ldflags = []
if host_system == 'windows' and cpp.get_id() != 'msvc' and cpp.get_id() != 'clang-cl'
	spuRecorder_ldflags += ['-static']
endif
spuRecorder_dependson = []

spuRecorder = shared_library('spuRecorder', sources: spuRecorder_src, include_directories: include_directories(spuRecorder_include_directories), cpp_args: spuRecorder_cppflags, link_args: spuRecorder_ldflags, dependencies: spuRecorder_dependson, install: true)

if host_system == 'windows' and cpp.get_id() != 'msvc' and cpp.get_id() != 'clang-cl'
archive = custom_target(
    'archive',
    output: meson.project_name() + '-' + meson.project_version() + '-win32.7z',
    input: [gpuRecorder, spuRecorder, 'Readme.txt', 'License.txt'],
    build_by_default: true,
    command: [find_program('7z'), 'a', '@OUTPUT@', '@INPUT@']
)
endif
