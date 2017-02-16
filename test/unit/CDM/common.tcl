foreach postfix {{} d} {
  set name libtclCDM3$postfix[info sharedlibextension]
  if {[file exists ../../../bin/Linux/x86_64/$name]} {
    load [file native ../../../bin/Linux/x86_64/$name] CDM
    break
  }
}
