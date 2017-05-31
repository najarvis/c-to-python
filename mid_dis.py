import sys
import ctypes
import pygame
import math

pygame.init()

# Detail level. Map will be (N ^ 2 + 1) x (N ^ 2 + 1).
N = 9

def get_func():
    # Load the dll file containing the function
    nDLL = ctypes.CDLL("mid_dis_dll.dll")

    # Load the function and let python know what the datatypes are.
    func = nDLL['new_map']
    func.restype = ctypes.POINTER(ctypes.c_double * int((2 ** N + 1) ** 2))
    func.argtype = ctypes.c_int

    return func

def draw_map():
    """The main function. Calls the function and creates an image
    based on the data.
    """

    func = get_func()
    array = func(N)

    width = int(math.sqrt(len(array[0])))
    print(width)
    
    # This outer surface will just be a red border used for debugging (making
    # it easier to see in the window's image viewer).
    outer_surf = pygame.Surface((width + 2, width + 2))
    outer_surf.fill((255, 0, 0))

    # The surf on which to draw the actual data.
    surf = pygame.Surface((width, width))

    # Almost sure all the below python is working as intended.
    print("locking")
    surf.lock()
    print("going into the loop")
    for y in xrange(width):
        for x in xrange(width):

            clr = int(array[0][y * width + x] * 256)

            clr = max(min(clr, 255), 0) # Limit 0 <= clr <= 255 for pygame

            # Add in some colors to make it look more like a gameworld
            if clr < 65:
                base_color = (65, 105, 225)
                color = lerp_color(gamma_color(base_color, 0.3),
                                   gamma_color(base_color, 0.7),
                                   clr / 256.)
            elif clr < 100:
                base_color = (46, 139, 87)
                color = lerp_color(gamma_color(base_color, 0.3),
                                   gamma_color(base_color, 0.7),
                                   clr / 256.)
            elif clr < 150:
                base_color = (131, 139, 131)
                color = lerp_color(gamma_color(base_color, 0.3),
                                   gamma_color(base_color, 0.7),
                                   clr / 256.)
            else:
                color = (clr, clr, clr)

            surf.set_at((x, y), color)

    print("done with the loop")
    surf.unlock()
    print("unlocked")

    outer_surf.blit(surf, (1, 1))

    # switch these comments if just saving the file
    #pygame.image.save(outer_surf, "mid_dis.png")
    return surf

def run():
    """ This just creates a window that shows the rendered
    image. You can press SPACE to get a new map.
    """

    screen_size = (513, 513)
    screen = pygame.display.set_mode(screen_size)

    surf = draw_map()
    screen.blit(surf, (0, 0))

    done = False
    while not done:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done = True
                break

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    done = True
                    break

                if event.key == pygame.K_SPACE:
                    surf = draw_map()
                    screen.blit(surf, (0, 0))

        pygame.display.flip()

    pygame.quit()

# -- HELPER METHODS --

def lerp(a, b, t):
    return (1 - t) * a + t * b

def lerp_color(clr1, clr2, t):
    return (lerp(clr1[0], clr2[0], t),
            lerp(clr1[1], clr2[1], t),
            lerp(clr1[2], clr2[2], t))

def gamma_color(clr1, t):
    if t < 0.5:
        return lerp_color((0, 0, 0), clr1, t * 2)
    else:
        return lerp_color(clr1, (255, 255, 255), (t - 0.5) * 2)

def trace(frame, event, arg):
    """Used to track the program during a segmentation fault (since you
    can't write an exception for it).
    """

    #print "%s, %s:%d" % (event, frame.f_code.co_filename, frame.f_lineno)
    print("%d" % frame.f_lineno),
    return trace

if __name__ == "__main__":
    #sys.settrace(trace) # Uncomment for debugging purposes.
    #draw_map() # Switch comments if just saving the image.
    run()
