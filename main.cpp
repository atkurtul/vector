#include <iostream>

#include "vec.h"

int main() {
  vec4 v = {1, 2, 3, 4};
  vec4 u = {4, 4, 4, 4};

  std::cout << (mat4{v, v + u, v + u * 2, v + u * 3});
  std::cout << tpos(mat4{v, v + u, v + u * 2, v + u * 3});
}