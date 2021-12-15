#include <iosfwd>

using f32 = float;

template <int>
struct vec;

template <int... n>
struct swizz;

template <int r, int c>
struct mat;

template <class V, int n>
concept veccy = requires(V v) {
  {vec<n>(v)};
};

template <int h, int... n>
static constexpr int max = h > max<n...> ? h : max<n...>;

template <int n>
static constexpr int max<n> = n;

template <int...>
struct is_unique;

template <int...>
struct is_unique2;

template <int a, int b, int... t>
struct is_unique2<a, b, t...> {
  static constexpr bool val = a != b && is_unique2<a, t...>::val;
};

template <int a>
struct is_unique2<a> {
  static constexpr bool val = true;
};

template <int a, int b, int... t>
struct is_unique<a, b, t...> {
  static constexpr bool val =
      is_unique2<a, b, t...>::val && is_unique2<b, t...>::val;
};

template <int... t>
static constexpr bool is_unique_v = is_unique<t...>::val;

template <int h, int... t>
struct Seq0 : Seq0<h - 1, h, t...> {};

template <int n>
using Seq = Seq0<n - 1>;

template <int... t>
struct Seq0<-1, t...> {
  static constexpr int n = sizeof...(t);
  using vt = vec<n>;

  template <veccy<n> U, veccy<n> V>
  static constexpr void assign(U& l, V r) {
    ((l[t] = r[t]), ...);
  }

  static constexpr vt pow(f32 x) {
    f32 p = 1;
    vt v;
    (((v[n - t - 1] = p), p *= x), ...);
    return v;
  }

  static constexpr vt mul(vt l, vt r) { return vt{l[t] * r[t]...}; }
  static constexpr vt div(vt l, vt r) { return vt{l[t] / r[t]...}; }
  static constexpr vt add(vt l, vt r) { return vt{l[t] + r[t]...}; }
  static constexpr vt sub(vt l, vt r) { return vt{l[t] - r[t]...}; }

  static constexpr vt mul(f32 l, vt r) { return vt{l * r[t]...}; }
  static constexpr vt div(f32 l, vt r) { return vt{l / r[t]...}; }
  static constexpr vt mul(vt l, f32 r) { return vt{l[t] * r...}; }
  static constexpr vt div(vt l, f32 r) { return vt{l[t] / r...}; }

  static constexpr f32 dot(vt l, vt r) { return hadd(l * r); }
  static constexpr f32 hadd(vt v) { return (v[t] + ...); }
  static std::ostream& stream(std::ostream& s, vt v) {
    return ((s << v[t] << ' '), ...) << "\b\n";
  }

  template <int a>
  static std::ostream& stream(std::ostream& s, mat<a, n> m) {
    return ((s << m[t]), ...) << "\b\n";
  }

  template <int r>
  static constexpr vec<r> mul(mat<r, n> m, vt v) {
    return ((m[t] * v[t]) + ...);
  }

  template <int c>
  static constexpr vt mul(vec<c> v, mat<c, n> m) {
    return vt{dot(m[t], v)...};
  }

  template <int a, int b>
  static constexpr mat<a, n> mul(mat<a, b> l, mat<b, n> r) {
    return mat<a, n>{l * r[t]...};
  }

  template <int a>
  static constexpr mat<a, n> mul(mat<a, n> l, f32 r) {
    return mat<a, n>{l[t] * r...};
  }

  template <int a>
  static constexpr mat<a, n> mul(f32 l, mat<a, n> r) {
    return mat<a, n>{l * r[t]...};
  }

  template <int a>
  static constexpr mat<a, n> add(mat<a, n> l, mat<a, n> r) {
    return mat<a, n>{l[t] + r[t]...};
  }

  template <int a>
  static constexpr mat<a, n> sub(mat<a, n> l, mat<a, n> r) {
    return mat<a, n>{l[t] - r[t]...};
  }

  template <int a, int i>
  static constexpr void write(mat<a, n>& m, vt v) {
    ((m[t][i] = v[t]), ...);
  }

  template <int a>
  static constexpr mat<n, a> tpos(mat<a, n> m) {
    mat<n, a> re;
    ((Seq<a>::template write<n, t>(re, m[t])), ...);
    return re;
  }

  static constexpr mat<n, n> fill(f32 x) {
    mat<n, n> m = {};
    ((m[t][t] = x), ...);
    return m;
  }
};

template <int... n>
struct swizz {
  static constexpr int mx = max<n...> + 1;
  static constexpr int ns = sizeof...(n);
  static constexpr int idx[] = {n...};
  f32 dat[mx];

  template <int... t>
  swizz& operator=(swizz<t...> v) requires(ns == sizeof...(t) &&
                                           is_unique_v<n...>) {
    return Seq<ns>::assign(*this, v), *this;
  }

  swizz& operator=(swizz v) requires(is_unique_v<n...>) {
    return Seq<ns>::assign(*this, v), *this;
  }

  template <veccy<ns> V>
  swizz& operator=(V v) requires(is_unique_v<n...>) {
    return Seq<ns>::assign(*this, v), *this;
  }

  using vt = vec<sizeof...(n)>;
  constexpr operator vt() const { return vt{dat[n]...}; }

  f32& operator[](size_t i) { return dat[idx[i]]; };
  f32 operator[](size_t i) const { return dat[idx[i]]; };

  vt operator*(vt r) const { return vt(*this) * r; }
  vt operator/(vt r) const { return vt(*this) / r; }
  vt operator+(vt r) const { return vt(*this) + r; }
  vt operator-(vt r) const { return vt(*this) - r; }

  vt operator*(f32 r) const { return vt(*this) * r; }
  vt operator/(f32 r) const { return vt(*this) / r; }
  friend vt operator*(f32 l, swizz r) { return l * vt(r); }
  friend vt operator/(f32 l, swizz r) { return l / vt(r); }

  friend std::ostream& operator<<(std::ostream& s, swizz v) {
    return s << vt(v);
  }
};

template <>
struct vec<2> {
  union {
    f32 dat[2];
    struct {
      f32 x, y;
    };
#define SWIZZ_LEVEL 2
#include "swizz.inl"
  };

  vec& operator=(vec const& v) { return memcpy(this, &v, sizeof v), *this; };
  f32& operator[](size_t i) { return dat[i]; };
  f32 operator[](size_t i) const { return dat[i]; };
  f32* begin() { return dat; }
  f32* end() { return dat + 2; }
  const f32* begin() const { return dat; }
  const f32* end() const { return dat + 2; }
};

template <>
struct vec<3> {
  union {
    f32 dat[3];
    struct {
      f32 x, y, z;
    };
#define SWIZZ_LEVEL 3
#include "swizz.inl"
  };

  vec& operator=(vec const& v) { return memcpy(this, &v, sizeof v), *this; };
  f32& operator[](size_t i) { return dat[i]; };
  f32 operator[](size_t i) const { return dat[i]; };
  f32* begin() { return dat; }
  f32* end() { return dat + 3; }
  const f32* begin() const { return dat; }
  const f32* end() const { return dat + 3; }
};

template <>
struct vec<4> {
  union {
    f32 dat[4];
    struct {
      f32 x, y, z, w;
    };
#define SWIZZ_LEVEL 4
#include "swizz.inl"
  };

  vec& operator=(vec const& v) { return memcpy(this, &v, sizeof v), *this; };
  f32& operator[](size_t i) { return dat[i]; };
  f32 operator[](size_t i) const { return dat[i]; };
  f32* begin() { return dat; }
  f32* end() { return dat + 4; }
  const f32* begin() const { return dat; }
  const f32* end() const { return dat + 4; }
};

template <int n>
struct vec {
  f32 dat[n];
  f32& operator[](size_t i) { return dat[i]; };
  f32 operator[](size_t i) const { return dat[i]; };

  f32* begin() { return dat; }
  f32* end() { return dat + n; }
  const f32* begin() const { return dat; }
  const f32* end() const { return dat + n; }
};

using vec2 = vec<2>;
using vec3 = vec<3>;
using vec4 = vec<4>;

using mat2 = mat<2, 2>;
using mat3 = mat<3, 3>;
using mat4 = mat<4, 4>;

template <int n, veccy<n> V>
vec<n> operator*(vec<n> l, V r) {
  return Seq<n>::mul(l, r);
}

template <int n, veccy<n> V>
vec<n> operator/(vec<n> l, V r) {
  return Seq<n>::div(l, r);
}

template <int n, veccy<n> V>
vec<n> operator+(vec<n> l, V r) {
  return Seq<n>::add(l, r);
}

template <int n, veccy<n> V>
vec<n> operator-(vec<n> l, V r) {
  return Seq<n>::sub(l, r);
}

template <int n>
vec<n> operator*(vec<n> l, f32 r) {
  return Seq<n>::mul(l, r);
}
template <int n>
vec<n> operator/(vec<n> l, f32 r) {
  return Seq<n>::div(l, r);
}

template <int n>
vec<n> operator*(f32 l, vec<n> r) {
  return Seq<n>::mul(l, r);
}

template <int n>
vec<n> operator/(f32 l, vec<n> r) {
  return Seq<n>::div(l, r);
}

#undef op0
#undef op

template <int n>
f32 dot(vec<n> l, vec<n> r) {
  return Seq<n>::dot(l, r);
}

template <int n>
f32 len(vec<n> v) {
  return sqrtf(Seq<n>::dot(v, v));
}

template <int n>
f32 hadd(vec<n> v) {
  return Seq<n>::hadd(v);
}

template <int n>
vec<n> norm(vec<n> v) {
  return v / dot(v, v);
}

inline vec3 cross(vec3 l, vec3 r) {
  return l.yzx * r.zxy - r.yzx * l.zxy;
}

template <int n>
std::ostream& operator<<(std::ostream& s, vec<n> v) {
  return Seq<n>::stream(s, v);
}

template <int a, int b>
std::ostream& operator<<(std::ostream& s, mat<a, b> m) {
  return Seq<b>::stream(s, m);
}

template <int r, int... n>
struct mat_swizz {
  using mt = mat<r, sizeof...(n)>;

  static constexpr int mx = max<n...> + 1;
  static constexpr int idx[] = {n...};

  vec<r> dat[mx];

  constexpr operator mt() const { return mt{dat[n]...}; }

  vec<r>& operator[](size_t i) { return dat[idx[i]]; };
  vec<r> operator[](size_t i) const { return dat[idx[i]]; };
};

template <int r>
struct mat<r, 2> {
  template <int... n>
  using swizz = mat_swizz<r, n...>;
  union {
    vec<r> dat[2];
    struct {
      vec<r> x, y;
    };
#define SWIZZ_LEVEL 2
#include "swizz.inl"
  };

  vec<r>& operator[](size_t i) { return dat[i]; };
  vec<r> operator[](size_t i) const { return dat[i]; };
};

template <int r>
struct mat<r, 3> {
  template <int... n>
  using swizz = mat_swizz<r, n...>;
  union {
    vec<r> dat[3];
    struct {
      vec<r> x, y, z;
    };
#define SWIZZ_LEVEL 3
#include "swizz.inl"
  };

  vec<r>& operator[](size_t i) { return dat[i]; };
  vec<r> operator[](size_t i) const { return dat[i]; };
};

template <int r>
struct mat<r, 4> {
  template <int... n>
  using swizz = mat_swizz<r, n...>;
  union {
    vec<r> dat[4];
    struct {
      vec<r> x, y, z, w;
    };
#define SWIZZ_LEVEL 4
#include "swizz.inl"
  };

  vec<r>& operator[](size_t i) { return dat[i]; };
  vec<r> operator[](size_t i) const { return dat[i]; };
};

template <int n>
mat<n, n> identity(f32 x) {
  return Seq<n>::fill(x);
}

template <int r, int n, veccy<n> V>
vec<r> operator*(mat<r, n> m, V v) {
  return Seq<n>::mul(m, v);
}

template <int c, int n, veccy<c> V>
vec<n> operator*(V v, mat<c, n> m) {
  return Seq<n>::mul(vec<c>(v), m);
}

template <int a, int b, int n>
mat<a, n> operator*(mat<a, b> l, mat<b, n> r) {
  return Seq<n>::mul(l, r);
}

template <int a, int n>
mat<a, n> operator*(mat<a, n> l, f32 r) {
  return Seq<n>::mul(l, r);
}

template <int a, int n>
mat<a, n> operator*(f32 l, mat<a, n> r) {
  return Seq<n>::mul(l, r);
}

template <int a, int b>
mat<a, b> operator+(mat<a, b> l, mat<a, b> r) {
  return Seq<b>::add(l, r);
}

template <int a, int b>
mat<a, b> operator-(mat<a, b> l, mat<a, b> r) {
  return Seq<b>::sub(l, r);
}

template <int a, int b>
mat<b, a> tpos(mat<a, b> m) {
  return Seq<b>::tpos(m);
}