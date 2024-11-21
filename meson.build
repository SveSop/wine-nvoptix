project('nvoptix', ['c'], version: 'v0.3', license: 'MIT')

cpu_family = target_machine.cpu_family()

include_path = include_directories('./include')

lib_dl      = declare_dependency(link_args: [ '-ldl' ])
lpthread    = declare_dependency(link_args: [ '-lpthread' ])

winebuild   = find_program('winebuild')
winegcc     = find_program('winegcc')
widl        = find_program('widl')

target_arch = cpu_family == 'x86_64' ? '-m64' : '-m32'

subdir('src')
