# namespace eval CDM {
#   namespace export tip_*
# }

foreach postfix {{} d} {
  set name libtclCDM3$postfix[info sharedlibextension]
  if {[file exists ../../../bin/Linux/x86_64/$name]} {
    load [file native ../../../bin/Linux/x86_64/$name] CDM
    break
  }
}

namespace import CDM::tip_*
