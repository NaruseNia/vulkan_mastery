project('vulkan_mastery', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20'])

is_windows = host_machine.system() == 'windows'

if is_windows
        # ============== INCLUDE VULKAN ================ #
        vulkan_dep = dependency('vulkan')
        if not vulkan_dep.found()
                vulkan_home = run_command('cmd', '/c', 'echo %VULKAN_HOME%', check: true).stdout().strip()
                if vulkan_home == ''
                        error('VULKAN_HOME environment variable is not set')
                endif
                vulkan_inc = include_directories(vulkan_home + '/Include')
                vulkan_lib = meson.get_compiler('c').find_library('vulkan', dirs : [vulkan_home / 'Lib'])
                vulkan_dep = declare_dependency(
                        include_directories : vulkan_inc,
                        dependencies : vulkan_lib
                )
        endif

        # =============== INCLUDE GLFW ================= #
        glfw_home = 'C:/lib/glfw-3.4-pc'

        if glfw_home == '%GLFW_HOME%' or glfw_home == ''
                error('GLFW_HOME is not set. Please set the GLFW_HOME environment variable.')
        endif

        glfw_inc = include_directories(glfw_home / 'include')
        glfw_lib_dir = glfw_home / 'lib-mingw-w64'

        cpp = meson.get_compiler('cpp')
        glfw_dep = cpp.find_library('glfw3', dirs: glfw_lib_dir)

        # =============== INCLUDE OTHERS ================ #
        stb_inc = 'C:/lib/stb'
else
        error('This platform was not supported')
endif

source_files = files(
        'src/main.cpp',
        'src/main.hpp'
)

vulkan_mastery = executable('vulkan_mastery', sources: source_files, dependencies: [vulkan_dep, glfw_dep], include_directories: [glfw_inc, stb_inc], install : true)

test('test', vulkan_mastery)