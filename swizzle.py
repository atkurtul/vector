
n = "xyzw"

swizz = [set(),set(),set()]

for r in range(0, 3):
  for i in range(0, r+2):
      for j in range(0, r+2):
        swizz[r].add((i,j))
        for k in range(0, r+2):
          swizz[r].add((i,j,k))
          for s in range(0, r+2):
            swizz[r].add((i,j,k,s))

def sorter(e):
  c = 2048
  w = len(e) * 2048
  for i in e:
    c /= 4
    w += i * c
  return w

for i in range(0, 3):
  for j in range(i+1, 3):
    for e in swizz[i]:
      swizz[j].remove(e)
  swizz[i]=list(swizz[i])
  swizz[i].sort(key=sorter)


for i in range(len(swizz)):
  print(f"#if SWIZZ_LEVEL >= {i+2}")
  for e in swizz[i]:
    print(f"swizz<{','.join([f'{i}' for i in e])}> {''.join([n[i] for i in e])};")
  print(f"#endif")

print("#ifdef SWIZZ_LEVEL")
print("#undef SWIZZ_LEVEL")
print("#endif")
  

