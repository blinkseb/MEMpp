load_modules('libmodules.so')

M_W = 80
M_TOP = 173

Flatter.flatter13 = {
    input = {"input/ps_points", 1},
    mass = M_W,
    width = 3
}

Flatter.flatter134 = {
    input = {"input/ps_points", 2},
    mass = M_TOP,
    width = 3
}

Flatter.flatter25 = {
    input = {"input/ps_points", 3},
    mass = M_W,
    width = 3
}

Flatter.flatter256 = {
    input = {"input/ps_points", 4},
    mass = M_TOP,
    width = 3
}

BlockD.BlockD = {
  p = {'TransferFunction/p3', 'TransferFunction/p4', 'TransferFunction/p5', 'TransferFunction/p6', 'Input/met', 'Input/isr'},
  s = {'flatter13/s', 'flatter134/s', 'flatter25/s', 'flatter256/s'}
}

print(Flatter.flatter13.mass)
for k, v in pairs(BlockD.BlockD.p) do
    print(k, v)
end
