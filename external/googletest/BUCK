load('//:subdir_glob.bzl', 'subdir_glob')

# Work-around for linker flag order issue
# https://github.com/facebook/buck/issues/1443
prebuilt_cxx_library(
  name = 'pthread',
  header_only = True,
  exported_linker_flags = [
    '-lpthread',
  ],
)

cxx_library(
  name = 'googlemock',
  header_namespace = '',
  exported_headers = subdir_glob([
    ('googlemock/include', '**/*.h'),
  ], exclude = [
    'googlemock/include/internal/**/*.h',
  ]),
  headers = subdir_glob([
    ('googlemock', 'src/*.h'),
    ('googlemock', 'src/*.cc'),
    ('googlemock/include', 'internal/**/*.h'),
    ('googletest/include', '**/*.h'),
  ]),
  srcs = [
    'googlemock/src/gmock-all.cc',
    'googlemock/src/gmock_main.cc',
  ],
  platform_deps = [
    ('linux.*', [ ':pthread' ]), 
  ], 
  visibility = [
    'PUBLIC',
  ],
)

cxx_library(
  name = 'googletest',
  header_namespace = '',
  exported_headers = subdir_glob([
    ('googletest/include', '**/*.h'),
  ], exclude = [
    'googletest/include/internal/**/*.h',
  ]),
  headers = subdir_glob([
    ('googletest', 'src/*.h'),
    ('googletest', 'src/*.cc'),
    ('googletest/include', 'internal/**/*.h'),
  ]),
  srcs = [
    'googletest/src/gtest-all.cc',
    'googletest/src/gtest_main.cc',
  ],
  platform_deps = [
    ('linux.*', [ ':pthread' ]), 
  ], 
  visibility = [
    'PUBLIC',
  ],
)
