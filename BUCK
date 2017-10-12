prebuilt_cxx_library(
  name = 'neither',
  header_only = True,
  header_namespace = 'neither',
  exported_headers = subdir_glob([
    ('neither/include', '**/*.hpp'),
  ]),
  licenses = [
    'LICENSE.txt',
  ],
  visibility = [
    'PUBLIC'
  ],
)

cxx_test(
  name = 'test',
  srcs = glob([
    'neither/tests/**/*.cpp',
  ]),
  platform_linker_flags = [
    ('^linux.*', [ '-lpthread', ]),
  ],
  link_style = 'shared',
  deps = [
    ':neither',
  ],
)
