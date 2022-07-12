gnome = import('gnome')

sources = [
'main.c',
'loongson-window.c',
'loongson-spec.c',
'loongson-security.c',
'loongson-mainboard.c',
'loongson-perf.c',
'loongson-state.c',
'loongson-utils.c',
]

executable('loongson-info',
  sources : sources,
  dependencies : [gtk_deps],
  include_directories: top_srcdir,
  install : true,
  install_dir : get_option('bindir')
  )
