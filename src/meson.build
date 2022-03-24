nvoptix_src = [
  'nvoptix.c',
  'nvoptix_55.c',
  'nvoptix_47.c',
  'nvoptix_41.c',
  'nvoptix_36.c',
  'nvoptix_22.c',
]

thread_dep = dependency('threads')

nvoptix_res_target = custom_target('nvoptix.res',
  output  : 'nvoptix.res',
  input   : 'nvoptix.rc',
  command : [ find_program('wrc'), '-o', '@OUTPUT@', '--nostdinc', '-I.', '-I'+meson.source_root()+'/include', '-D__WINESRC__', '@INPUT@' ],
  install : false)


nvoptix_dll = shared_library('nvoptix.dll', nvoptix_src,
  name_prefix         : '',
  dependencies        : [ thread_dep, lib_dl ],
  include_directories : include_path,
  objects             : 'nvoptix.spec',
  install             : true)


nvoptix_dll_target = custom_target('nvoptix.dll',
  output  : 'nvoptix.dll',
  input   : [ 'nvoptix.spec', nvoptix_res_target ],
  command : [ winebuild, target_arch, '--dll', '--fake-module', '-E', '@INPUT@', '-o', '@OUTPUT@', '-F', 'nvoptix.dll' ],
  install : true,
  install_dir : 'fakedll')


nvoptix_def_target = custom_target('nvoptix.def',
  output  : 'nvoptix.def',
  input   : 'nvoptix.spec',
  command : [ winebuild, target_arch, '-w', '--def', '--export', '@INPUT@', '-o', '@OUTPUT@' ],
  install : true,
  install_dir : 'defs')

