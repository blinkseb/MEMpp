load_modules('libempty_module.so')

M_W = 80.
M_TOP = 173.

Flatter.flatter_s13 = {
    input = {"cuba::ps_points", 0},
    mass = M_W,
    width = 3.
}

Flatter.flatter_s134 = {
    input = {"cuba::ps_points", 1},
    mass = M_TOP,
    width = 3.
}

Flatter.flatter_s25 = {
    input = {"cuba::ps_points", 2},
    mass = M_W,
    width = 3.
}

Flatter.flatter_s256 = {
    input = {"cuba::ps_points", 3},
    mass = M_TOP,
    width = 3.
}

BlockD.blockd = {
  p = {'TransferFunction::p3', 'TransferFunction::p4', 'TransferFunction::p5', 'TransferFunction::p6', 'Input::met', 'Input::isr'},
  s = {'flatter13::s', 'flatter134::s', 'flatter25::s', 'flatter256::s'},
  r = {25, 8, 5, 6, 78},
  t = {1., 5.5, 7.}
}
