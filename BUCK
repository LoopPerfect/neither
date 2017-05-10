
cxx_library(
  name = 'neither',
  header_namespace = 'neither',
  exported_headers = subdir_glob([
    ('neither/include', '**/*.hpp'),
  ]),
)



cxx_test(
  name = 'neither-tests',
  deps = [':neither'],
  srcs = glob([
    'neither/tests/**/*.cpp',
  ]),
)
