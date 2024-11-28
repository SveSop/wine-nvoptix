nvoptix_src = [
  'nvoptix.c',
  'nvoptix_callbacks.c',
  'nvoptix_93.c',
  'nvoptix_87.c',
  'nvoptix_84.c',
  'nvoptix_68.c',
  'nvoptix_60.c',
  'nvoptix_55.c',
  'nvoptix_47.c',
  'nvoptix_41.c',
  'nvoptix_36.c',
  'nvoptix_22.c',
]

thread_dep = dependency('threads')

nvoptix_dll = shared_library('nvoptix.dll', nvoptix_src,
  name_prefix         : '',
  dependencies        : [ thread_dep, lib_dl, lpthread ],
  include_directories : include_path,
  objects             : 'nvoptix.spec',
  install             : true)
