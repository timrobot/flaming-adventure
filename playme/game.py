import pygame, os

pygame.init()

screen = pygame.display.set_mode((640, 480))
clock = pygame.time.Clock()

def main():
  # load the man
  man_name = filter(lambda s: s.find("tmp-") != -1 and s.find(".gif") != -1, os.listdir("."))
  man_name.sort()
  the_man = map(lambda s: pygame.image.load(s), man_name)
  scalefactor = 0.25
  # shrink images, cut out line, turn white to alpha
  the_man = map(lambda s: pygame.transform.scale(s, (int(s.get_width() * scalefactor), int(s.get_height() * scalefactor))), the_man)
  the_man = map(lambda s: s.subsurface(pygame.Rect(0, 0, s.get_width(), s.get_height() - 20)), the_man)
  the_man = map(lambda s: s.convert_alpha(), the_man)
  def colorkey_filter(surf, color):
    for x in range(surf.get_width()):
      for y in range(surf.get_height()):
        if surf.get_at((x, y))[0] == color[0] and \
           surf.get_at((x, y))[1] == color[1] and \
           surf.get_at((x, y))[2] == color[2]:
          surf.set_at((x, y), (color[0], color[1], color[2], 0))
  for i in range(len(the_man)):
    colorkey_filter(the_man[i], (255, 255, 255))

  # states
  y = 0
  vx = -4
  key_queue = []
  jump = False
  vy = 0
  acc = -1
  objs = [ pygame.Rect(640, 240 - 50, 50, 50) ]
  man_mode = 0

  while True:
    # catch quit
    for event in pygame.event.get():
      if event.type == pygame.QUIT:
        pygame.quit()
        return
      if event.type == pygame.KEYDOWN:
        if event.key == pygame.K_UP:
          key_queue = ["up"] + key_queue
          if not jump:
            jump = True
            vy = 16
        elif event.key == pygame.K_DOWN:
          key_queue = ["down"] + key_queue
      if event.type == pygame.KEYUP:
        if event.key == pygame.K_UP:
          key_queue = filter(lambda s: s != "up", key_queue)
        elif event.key == pygame.K_DOWN:
          key_queue = filter(lambda s: s != "down", key_queue)

    screen.fill((255, 255, 255))

    # draw the ground
    pygame.draw.line(screen, (0, 0, 0), (0, 240), (640, 240))

    # draw the boxes
    for obj in objs:
      if obj.x + obj.w < 0:
        obj.x = 640
    for obj in objs:
      obj.x += vx
      pygame.draw.rect(screen, (255, 0, 0), obj, 0)
    
    # draw the man
    running_man = the_man[man_mode]
    screen.blit(running_man, (50, 240 - running_man.get_height() - y))

    # update
    if vy > 0 or y > 0:
      man_mode = 0
      y += vy
      vy += acc
    else:
      man_mode = (man_mode + 1) % len(the_man)
      y = 0
      vy = 0
      jump = False

    pygame.display.flip()
    clock.tick(30) # fps

main()
