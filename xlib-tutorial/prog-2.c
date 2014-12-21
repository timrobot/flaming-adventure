#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds
#include <stdio.h>

#define NIL (0)       // A name for the void pointer

int main() {
  // Open the display

  Display *dpy = XOpenDisplay(NIL);
  assert(dpy);

  //// Get Screen
  int screen = DefaultScreen(dpy);
  int depth = DefaultDepth(dpy, screen);
  printf("depth: %d\n", depth);

  // Get some colors

  int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
  int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

  // Create the window

  Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
      200, 100, 0, blackColor, blackColor);

  // We want to get MapNotify events

  XSelectInput(dpy, w, StructureNotifyMask);

  // "Map" the window (that is, make it appear on the screen)

  XMapWindow(dpy, w);

  // Create a "Graphics Context"

  GC gc = XCreateGC(dpy, w, 0, NIL);

  // Tell the GC we draw using the white color

  XSetForeground(dpy, gc, whiteColor);

  // Wait for the MapNotify event

  for(;;) {
    XEvent e;
    XNextEvent(dpy, &e);
    if (e.type == MapNotify)
      break;
  }

  // Draw the line

  XDrawLine(dpy, w, gc, 10, 60, 180, 20);

  // Send the "DrawLine" request to the server

  XFlush(dpy);

  // Wait for 10 seconds

  sleep(3);

  return 0;
}

