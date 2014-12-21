#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct MwmHints {
  unsigned long flags;
  unsigned long functions;
  unsigned long decorations;
  long input_mode;
  unsigned long status;
};
enum {
  MWM_HINTS_FUNCTIONS = (1L << 0),
  MWM_HINTS_DECORATIONS =  (1L << 1),

  MWM_FUNC_ALL = (1L << 0),
  MWM_FUNC_RESIZE = (1L << 1),
  MWM_FUNC_MOVE = (1L << 2),
  MWM_FUNC_MINIMIZE = (1L << 3),
  MWM_FUNC_MAXIMIZE = (1L << 4),
  MWM_FUNC_CLOSE = (1L << 5)
};

int main(int argc, char* argv[])
{
  Display* display = XOpenDisplay(NULL);

  /* try to match the visual info */
  XVisualInfo vinfo;
  XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);

  /* attributes of the window */
  XSetWindowAttributes attr;
  attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
  attr.border_pixel = 0;
  attr.background_pixel = 0;

  /* create window with attributes */
  Window win = XCreateWindow(display, DefaultRootWindow(display), 0, 0, 300, 200, 0, vinfo.depth, InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel, &attr);
  XSelectInput(display, win, StructureNotifyMask);

  /* graphics context */
  GC gc = XCreateGC(display, win, 0, 0);

  /* delete border */
  Atom mwmHintsProperty = XInternAtom(display, "_MOTIF_WM_HINTS", 0);
  struct MwmHints hints;
  hints.flags = MWM_HINTS_DECORATIONS;
  hints.decorations = 0;
  XChangeProperty(display, win, mwmHintsProperty, mwmHintsProperty, 32,
      PropModeReplace, (unsigned char *)&hints, 5);

  Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
  XSetWMProtocols(display, win, &wm_delete_window, 1);

  XMapWindow(display, win);

  /* draw a white line */
  int whiteColor = WhitePixel(display, DefaultScreen(display));
  XSetForeground(display, gc, whiteColor);

  int keep_running = 1;
  XEvent event;

  while (keep_running) {
    XNextEvent(display, &event);

    switch(event.type) {
      case ClientMessage:
        if (event.xclient.message_type == XInternAtom(display, "WM_PROTOCOLS", 1) && (Atom)event.xclient.data.l[0] == XInternAtom(display, "WM_DELETE_WINDOW", 1))
          keep_running = 0;

        break;

      default:
        break;
    }
    XDrawLine(display, win, gc, 10, 60, 50, 80);
    XFlush(display);
  }

  XDestroyWindow(display, win);
  XCloseDisplay(display);
  return 0;
}
