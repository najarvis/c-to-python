import sys
import ctypes
import pygame
pygame.init()

def get_func():
    nDLL = ctypes.CDLL("mid_dis_dll.dll")

    N = 8 # 2^9 = 512. Creates a 513 x 513 map
    func = nDLL['new_map']
    func.restype = ctypes.POINTER(ctypes.c_double * (2 ** N + 1) * (2 ** N + 1))
    #func.restype = ctypes.c_double * (2 ** N + 1) * (2 ** N + 1)
    func.argtype = ctypes.c_int

    return func

def run():
    N = 9
    func = get_func()
    array = func(N)

    print(array)
    width = len(array[0])

    surf = pygame.Surface((width, width))

    print("locking")
    surf.lock()
    print("going into the loop")
    for y in xrange(width):
        for x in xrange(width):
            print(y, x)
            try:
                clr = max(min(int(array[0][x][y] * 256), 255), 0)
            except ValueError:
                print array[0][x][x]
                clr = 0
            except OverflowError:
                print array[0][x][y]
                clr = 255

            surf.set_at((x, y), (clr, clr, clr))
    print("done with the loop")
    surf.unlock()
    print("unlocked")

    pygame.image.save(surf, "mid_dis.png")

def trace(frame, event, arg):
    print "%s, %s:%d" % (event, frame.f_code.co_filename, frame.f_lineno)
    return trace

sys.settrace(trace)
run()

"""
import pygame
pygame.init()
surf = pygame.Surface((width, width))

print("locking")
surf.lock()
print("going into the loop")
for y in xrange(width):
    print("y=%d"%y)
    for x in xrange(width):
        print ("x=%d"%x)
        try:
            clr = max(min(int(array[1][x][y] * 256), 255), 0)
        except ValueError:
            print array[0][y][x]
            clr = 0
        surf.set_at((x, y), (clr, clr, clr))
print("done with the loop")
surf.unlock()
print("unlocked")

pygame.image.save(surf, "mid_dis.png")
"""
