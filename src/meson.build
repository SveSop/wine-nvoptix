nvoptix_src = [
  'nvoptix.c',
  'nvoptix_callbacks.c',
  'nvoptix_105.c',
  # 'nvoptix_93.c',
  # 'nvoptix_87.c',
  # 'nvoptix_84.c',
  # 'nvoptix_68.c',
  # 'nvoptix_60.c',
  # 'nvoptix_55.c',
  # 'nvoptix_47.c',
  # 'nvoptix_41.c',
  # 'nvoptix_36.c',
  # 'nvoptix_22.c',
]

nvoptix_unixlib_src = [
  'nvoptix_unixlib.c',
  'nvoptix_callbacks_unixlib.c',
  'nvoptix_105_unixlib.c',
  # 'nvoptix_93_unixlib.c',
  # 'nvoptix_87_unixlib.c',
  # 'nvoptix_84_unixlib.c',
  # 'nvoptix_68_unixlib.c',
  # 'nvoptix_60_unixlib.c',
  # 'nvoptix_55_unixlib.c',
  # 'nvoptix_47_unixlib.c',
  # 'nvoptix_41_unixlib.c',
  # 'nvoptix_36_unixlib.c',
  # 'nvoptix_22_unixlib.c',
]

include_dirs = include_directories('.', '../include')

if is_windows
  nvoptix_res = custom_target(
    'nvoptix.res',
    output: 'nvoptix.res',
    input: 'nvoptix.rc',
    command: [
      wrc,
      '-I', meson.project_source_root() / 'include',
      '-o', '@OUTPUT@',
      '@INPUT@',
    ],
    install: false,
  )

  shared_library(
    'nvoptix',
    [nvoptix_src, nvoptix_res],
    name_prefix: '',
    include_directories: include_dirs,
    link_depends: ['nvoptix.spec'],
    link_args: ['-Wl,--wine-builtin', meson.current_source_dir() / 'nvoptix.spec'],
    install: true,
    install_dir: install_dir,
  )
else
  shared_library(
    'nvoptix',
    nvoptix_unixlib_src,
    name_prefix: '',
    include_directories: include_dirs,
    dependencies: [dl, threads, ntdll],
    install: true,
    install_dir: install_dir,
  )
endif
