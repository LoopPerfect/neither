cxx_library(
  name = 'neither',
  header_namespace = 'neither',
  exported_headers = subdir_glob([
    ('neither/include', '**/*.hpp'),
  ]),
  visibility = [
    'PUBLIC',
  ],
)

cxx_test(
  name = 'test',
  srcs = glob([
    'neither/tests/**/*.cpp',
  ]),
  platform_compiler_flags = [
    ('^linux.*', [ '-lpthread' ]),
  ],
  deps = [
    ':neither',
  ],
)
