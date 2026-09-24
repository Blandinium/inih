catch2_dep = dependency('catch2', version : ['>=2.13.10', '<3'],
    fallback : ['catch2', 'catch2_dep'])

# Compile Catch2's runner once for both parser configurations.
catch2_main = static_library('ini_reader_test_main', 'main.cpp',
    dependencies : catch2_dep)
reader_tests = files('INIReaderTest.cpp', 'INIReaderCopyTest.cpp',
    'INIReaderMoveTest.cpp', 'INIReaderBehaviorTest.cpp')

# Like the C tests, use controlled parser settings independent of library options.
foreach new_section : [0, 1]
    defines = ['-DINI_CALL_HANDLER_ON_NEW_SECTION=' + new_section.to_string()]
    exe = executable('ini_reader_test_' + new_section.to_string(),
        reader_tests, src_INIReader, src_inih,
        c_args : defines, cpp_args : defines,
        dependencies : catch2_dep, link_with : catch2_main)
    test('INIReader_' + new_section.to_string(), exe,
        suite : 'cpp', workdir : meson.current_source_dir())
endforeach
