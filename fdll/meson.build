nvoptix_res = custom_target(
  'nvoptix.res',
  output: 'nvoptix.res',
  input: 'nvoptix.rc',
  command: [
    wrc,
    '-I', meson.project_source_root() + '/include',
    '-o', '@OUTPUT@',
    '@INPUT@',
  ],
  install: false,
)

nvoptix_def = custom_target(
  'nvoptix.def',
  output: 'nvoptix.def',
  input: '../src/nvoptix.spec',
  command: [winebuild, target_arch, '-w', '--def', '-o', '@OUTPUT@', '-E', '@INPUT@'],
  install: false,
)

custom_target(
  'nvoptix.dll',
  output: 'nvoptix.dll',
  input: [nvoptix_def, nvoptix_res],
  command: [
    winebuild,
    target_arch,
    '--dll',
    '--fake-module',
    '-o', '@OUTPUT@',
    '-E', '@INPUT@',
  ],
  install: true,
  install_dir: install_dir_windows,
)
