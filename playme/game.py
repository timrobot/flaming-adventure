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
  for img in the_man:
    img.set_colorkey((255, 255, 255), pygame.RLEACCEL)
  # states
  y = 0
  vx = -3
  key_queue = []
  jump = False
  vy = 0
  acc = -2
  objs = [ pygame.Rect(640, 240 - 50, 50, 50) ]

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
      obj.x += vx
      pygame.draw.rect(screen, (255, 0, 0), obj, 1)
    
    # draw the man
    running_man = the_man[0]
    screen.blit(running_man, (50, 240 - running_man.get_height() - y))

    # update
    if vy > 0 or y > 0:
      y += vy
      vy += acc
    else:
      the_man = the_man[1:] + [running_man]
      y = 0
      vy = 0
      jump = False

    pygame.display.flip()
    clock.tick(30) # fps

main()
