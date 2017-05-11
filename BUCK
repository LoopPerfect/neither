cxx_library(
  name = 'neither',
  header_namespace = 'neither',
  exported_headers = subdir_glob([
    ('neither/include', '**/*.hpp'),
  ]),
  visibility = ['PUBLIC']
)

cxx_test(
  name = 'test',
  deps = [':neither'],
  srcs = glob([
    'neither/tests/**/*.cpp',
  ]),
)
