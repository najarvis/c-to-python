import ctypes
nDLL = ctypes.CDLL("mid_dis_dll.dll")

N = 9

func = nDLL['new_map']
func.restype = ctypes.c_double * (2 ** N + 1) * (2 ** N + 1)
func.argtype = ctypes.c_int

array = func(N)
print(array)
width = len(array)
print(width)

import pygame
surf = pygame.Surface((width, width))

surf.lock()
for y in xrange(width):
    for x in xrange(width):
        try:
            clr = max(min(int(array[y][x]), 255), 0)
        except ValueError:
            print array[y][x]
            clr = 0
        surf.set_at((x, y), (clr, clr, clr))
surf.unlock()

pygame.image.save(surf, "mid_dis.png")
