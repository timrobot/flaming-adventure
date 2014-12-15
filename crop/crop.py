#!/usr/bin/env python
import sys, pygame

pygame.init()
maxres = (1200, 800)
onclick = False

if len(sys.argv) != 3:
  print "Usage: {0} [source pic] [dest pic]".format(sys.argv[0])
  sys.exit()

# get a pic
pic = pygame.image.load(sys.argv[1])
startpos = [-1, -1]
endpos = [-1, -1]
selected = False
res = [pic.get_width(), pic.get_height()]
scale_factor = 1
while res[0] > maxres[0] or res[1] > maxres[1]:
  res[0] /= 2
  res[1] /= 2
  scale_factor *= 2
temp_pic = pygame.transform.scale(pic, (res[0], res[1]))

screen = pygame.display.set_mode((res[0], res[1]))
clock = pygame.time.Clock()

while not selected:
  for event in pygame.event.get():
    if event.type == pygame.QUIT:
      pygame.quit()
      sys.exit()
    if event.type == pygame.MOUSEBUTTONDOWN:
      startpos = pygame.mouse.get_pos()
      onclick = True
    if event.type == pygame.MOUSEBUTTONUP:
      endpos = pygame.mouse.get_pos()
      onclick = False
      selected = True

  screen.fill((0, 0, 0))
  screen.blit(temp_pic, (0, 0))
  endpos = pygame.mouse.get_pos()
  if onclick:
    x1 = min([startpos[0], endpos[0]])
    y1 = min([startpos[1], endpos[1]])
    x2 = max([startpos[0], endpos[0]])
    y2 = max([startpos[1], endpos[1]])
    selection_box = pygame.Surface((x2 - x1, y2 - y1))
    selection_box = selection_box.convert_alpha()
    selection_box.fill((0, 0, 255, 128))
    screen.blit(selection_box, (x1, y1))
  pygame.display.flip()
  clock.tick(40)

x1 = min([startpos[0], endpos[0]]) * scale_factor
y1 = min([startpos[1], endpos[1]]) * scale_factor
x2 = max([startpos[0], endpos[0]]) * scale_factor
y2 = max([startpos[1], endpos[1]]) * scale_factor
w = x2 - x1
h = y2 - y1

new_pic = pic.subsurface(pygame.Rect(x1, y1, w, h))
pygame.image.save(new_pic, sys.argv[2])
pygame.quit()
sys.exit()
