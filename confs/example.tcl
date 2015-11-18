# C++ part
# add is just an alias for lappend
interp alias {} add {} lappend

# This will be a C++ function
proc load_module {name} {
    # Do nothing
}

proc module {type name cmd} {
    namespace eval $name $cmd
}

# Real configuration part starts here

load_module libmodules.so

set M_TOP 173
set M_W 80

module Flatter flatter13 {
  set input {input/phaseSpace 1}
  set mass $M_W
  set width 3
}

module Flatter flatter134 {
  set input {input/phaseSpace 3}
  set mass $M_TOP
  set width 3
}

module Flatter flatter25 {
  set input {input/phaseSpace 2}
  set mass $M_W
  set width 3
}

module Flatter flatter134 {
  set input {input/phaseSpace 4}
  set mass $M_TOP
  set width 3
}

module BlockD BlockD {
  add p TransferFunction/p3
  add p TransferFunction/p4
  add p TransferFunction/p5
  add p TransferFunction/p6

  add p Input/met
  add p Input/isr
  
  set s {flatter13/s flatter134/s flatter25/s flatter256/s}
}

puts $flatter13::mass
puts $BlockD::p
