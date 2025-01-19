project('NetCpp', 'cpp', license: 'MIT', version: '1.0.0', license_files: 'LICENSE', default_options: ['warning_level=3'])


src = [
  'src/http1/request.cc',
  'src/http1/response.cc',
  'src/http2/request.cc',
  'src/http2/response.cc',
  'src/http2/frames.cc',
  'src/utils/http_version.cc',
  'src/utils/method.cc',
  'src/utils/status.cc',
  'src/utils/parse_headers.cc',
  'src/posix/descriptor.cc',
  'src/server.cc'
]

test_src = [
  'test/test.cc',
  'test/core.cc',
  'test/http2.cc'
]

# Test dependencies
gtest_dep = dependency('gtest', required: false)

library('NetCpp', sources: src, include_directories: 'include', soversion: '1.0.0')

test_exe = executable('NetCppTest', sources: [test_src, src], dependencies: [gtest_dep])

test('NetCppTest', test_exe)
