project('NetCpp', 'cpp', license: 'MIT', version: '1.0.0', license_files: 'LICENSE', default_options: ['warning_level=3'])


src = [
  'src/request.cc',
  'src/response.cc',
  'src/utils/http_version.cc',
  'src/utils/method.cc',
  'src/utils/status.cc',
  'src/utils/parse_headers.cc',
  'src/posix/descriptor.cc',
  'src/server.cc'
]

test_src = [
  'test/test.cc',
  'test/core.cc'
]

# Test dependencies
gtest_dep = dependency('gtest', required: false)

library('NetCpp', sources: src, include_directories: 'include', soversion: '1.0.0')

test_exe = executable('NetCppTest', sources: [test_src, src], dependencies: [gtest_dep])

test('NetCppTest', test_exe)
