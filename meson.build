project('fqseek', 'cpp',
  version : '1.0.0-beta.0',
  default_options : ['cpp_std=c++11','default_library=static', 'warning_level=0', 'cpp_link_args=-flto'],
)

cxx = meson.get_compiler('cpp')

fqseek_srcs = files([
   'src/fq_reader.cpp', 
   'src/fqseek.cpp', 
   'src/seq_scanner.cpp', 
   'src/util.cpp'
])

fqseek_tests = files([
   'src/fq_reader.cpp', 
   'src/seq_scanner.cpp', 
   'src/util.cpp', 
   'test/test_seq_scanner.cpp'
])

executable('fqseek', [fqseek_srcs], 
   include_directories: 'src', 
   install : true,
)

e = executable('fqseek_test', [fqseek_tests],
   include_directories: ['src'])

test('fqseek_test', e)
