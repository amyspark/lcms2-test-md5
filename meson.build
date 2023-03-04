project('lcms2-md5-tester',
	'cpp',
	version: '0.1.0',
	default_options: [
		'cpp_std=c++17',
		'warning_level=3',
		'b_pch=false',
	],
)

lcms2 = dependency('lcms2', version: '>=2.14')

main_srcs = files(
	'main.cpp',
)

main_exe = executable(
	'main',
	main_srcs,
	dependencies: [lcms2],
)
