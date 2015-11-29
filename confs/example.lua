load_modules('libempty_module.so')

M_W = 80.
M_TOP = 173.

Flatter.flatter_s13 = {
    input = "cuba::ps_points/0",
    mass = M_W,
    width = 3.
}

Flatter.flatter_s134 = {
    input = "cuba::ps_points/1",
    mass = M_TOP,
    width = 3.
}

Flatter.flatter_s25 = {
    input = "cuba::ps_points/2",
    mass = M_W,
    width = 3.
}

Flatter.flatter_s256 = {
    input = "cuba::ps_points/3",
    mass = M_TOP,
    width = 3.
}

BlockD.blockd = {
    input = 'input::particles',

    s13 = 'flatter_s13::s',
    s134 = 'flatter_s134::s',
    s25 = 'flatter_s25::s',
    s256 = 'flatter_s256::s',
}
