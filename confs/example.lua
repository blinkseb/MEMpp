load_modules('libempty_module.so')

M_W = 80.419002
M_TOP = 173.
W_W = 2.047600e+00
W_TOP = 1.491500e+00

Flatter.flatter_s13 = {
    input = "cuba::ps_points/0",
    mass = M_W,
    width = W_W
}

Flatter.flatter_s134 = {
    input = "cuba::ps_points/1",
    mass = M_TOP,
    width = W_TOP
}

Flatter.flatter_s25 = {
    input = "cuba::ps_points/2",
    mass = M_W,
    width = W_W
}

Flatter.flatter_s256 = {
    input = "cuba::ps_points/3",
    mass = M_TOP,
    width = W_TOP
}

BlockD.blockd = {
    input = 'input::particles',

    s13 = 'flatter_s13::s',
    s134 = 'flatter_s134::s',
    s25 = 'flatter_s25::s',
    s256 = 'flatter_s256::s',
}

Boost.boost = {
    invisibles = {
        'blockd::invisibles',
    },

    particles = {
        'input::particles/0',
        'input::particles/1',
        'input::particles/2',
        'input::particles/3',
    }
}

MatrixElement.ttbar = {
  pdf = 'CT10nlo',

  initialState = 'boost::output',

  invisibles = {
    input = 'blockd::invisibles',
    jacobians = 'blockd::jacobians',
    ids = {
      {
        pdg_id = 12,
        me_index = 2,
        index = 0
      },

      {
        pdg_id = -14,
        me_index = 5,
        index = 1
      }
    }
  },

  particles = {
    input = 'input::particles',
    ids = {
      {
        pdg_id = -11,
        me_index = 1,
        index = 0
      },

      {
        pdg_id = 5,
        me_index = 3,
        index = 1
      },

      {
        pdg_id = 13,
        me_index = 4,
        index = 2
      },

      {
        pdg_id = -5,
        me_index = 6,
        index = 3
      },
    }
  },

  jacobians = {'flatter_s13::jacobian', 'flatter_s134::jacobian', 'flatter_s25::jacobian', 'flatter_s256::jacobian'}
}
