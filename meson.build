project('nvoptix', ['c'], version: 'v0.3', license: 'MIT')

is_windows = target_machine.system() == 'windows'
arch_dir_prefix = target_machine.cpu_family() == 'x86_64' ? 'x86_64' : 'i386'
os_dir_suffix = is_windows ? 'windows' : 'unix'
install_dir = get_option('libdir') / 'wine' / arch_dir_prefix + '-' + os_dir_suffix

if not is_windows
    dl = dependency('dl')
    threads = dependency('threads')
    ntdll = meson.get_compiler('c').find_library(':ntdll.so')
endif

wrc = find_program('wrc')

subdir('src')
