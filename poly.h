#include <corecrt_math.h>
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

inline vec2 eigen(vec2 x, vec2 y) {
  f32 a = sqrt(x.x * x.x + y.y * y.y + 4 * x.y * y.x - 2 * x.x * y.y);
  f32 b = x.x + y.y;
  return vec2{a - b, -a - b} * 0.5f;
}

inline vec2 eigen2(mat2 m) {
  f32 a = -m.y.x / m.x.x;
  if (!isnan(a))
    return vec2{a, 1};
  return vec2{-m.y.y / m.x.y, 1};
}

inline mat2 eigenv(mat2 m) {
  vec2 e = eigen(m.x, m.y);
  mat2 i = identity<2>(1);

  return mat2{eigen2(m - i * e.x), eigen2(m - i * e.y)};
}
/*
  xx-d yx
  xy   yy-d

  (xx-d)(yy-d) - xyyx

  dd -d(xx+yy) + xxyy - xyyx

  (-(xx+yy) -+ sqrt(xxxx + yyyy + 4xyyx - 2xxyy))/2

*/