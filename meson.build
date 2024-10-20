project('vulkan_mastery', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20'])

cxx = meson.get_compiler('cpp')

is_windows = host_machine.system() == 'windows'
if is_windows
    # =========== INCLUDE VULKAN(VOLK) ============= #
    vulkan_home = run_command('cmd', '/c', 'echo %VULKAN_SDK%', check : true).stdout().strip()
    if vulkan_home == ''
        error('VULKAN_HOME environment variable is not set')
    endif
    vulkan_inc = include_directories(vulkan_home + '/Include')
    vulkan_lib = meson.get_compiler('cpp').find_library('volk', dirs : [vulkan_home / 'Lib'])
    vulkan_dep = declare_dependency(
        include_directories : vulkan_inc,
        dependencies : vulkan_lib
    )

    # =============== INCLUDE GLFW ================= #
    glfw_home = 'C:/lib/glfw-3.4-pc'

    if glfw_home == '%GLFW_HOME%' or glfw_home == ''
        error('GLFW_HOME is not set. Please set the GLFW_HOME environment variable.')
    endif

    glfw_inc = include_directories(glfw_home / 'include')
    glfw_lib_dir = glfw_home / 'lib-vc2022'

    glfw_dep = cxx.find_library('glfw3', dirs : glfw_lib_dir)

    # =============== INCLUDE OTHERS ================ #
    stb_inc = 'C:/lib/stb'
else
    error('This platform was not supported')
endif

source_files = files(
    'src/main.cpp',
    'src/gfx/gfx_device.cpp',
    'src/gfx/gfx_device.h',
)

vulkan_mastery = executable('vulkan_mastery', sources : source_files, dependencies : [vulkan_dep, glfw_dep], include_directories : ['include', glfw_inc, stb_inc], install : true)
test('test', vulkan_mastery)
