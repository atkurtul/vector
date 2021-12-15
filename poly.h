#include "vec.h"

template <int d>
struct poly {
  vec<d> c;

  poly<d - 1> derive() const {
    poly<d - 1> dd;
    f32 i = 1;
    for (auto w : dd.c) {
      w = i * c[i];
      ++i;
    }
    return dd;
  }

  f32 eval(f32 a) const { return dot(Seq<d>::pow(a), c); }
};
