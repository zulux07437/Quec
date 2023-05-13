project('Quec', 'cpp', version: '1.0.0')

link_args = ['-L./', '-lbass', '-ltags', '-lbass_fx', '-lbassmix'] # ライブラリのPATHが通らずともビルドしたい
compiler_args = ['-std=c++20']
dependencies = [dependency('gtk+-3.0'), dependency('gtkmm-3.0')]

src_files = [
    'src/main.cpp',
    'src/Common/Bass.cpp',
    'src/Common/Logger.cpp',
    'src/MainWnd/MainWnd.cpp',
    'src/MainWnd/PlayList.cpp'
]

configure_file(
    input: 'include/project.h.in',
    output: 'project.h',
    configuration: {
        'PROJECT_VERSION': meson.project_version(),
        'PROJECT_NAME': meson.project_name()
    }
)

inc_dirs = include_directories('include', 'build') # 環境によってビルドディレクトリは違うからあとで修正

custom_target('glade',
    input: 'MainWnd.glade',
    output: 'MainWnd.glade',
    command: ['cp', '@INPUT@', '@OUTPUT@'],
    build_by_default: true,
)

executable('Quec', src_files, dependencies: dependencies, cpp_args: compiler_args, link_args: link_args, include_directories: inc_dirs)