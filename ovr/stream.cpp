#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace cv;
VideoCapture *cam;

typedef struct gst_app {
  GstPipeline *pipeline;
  GstAppSrc *src;
  GstElement *sink;
  GstElement *decoder;
  GstElement *ffmpeg;
  GstElement *xvimagesink;
  GMainLoop *loop;
  guint sourceid;
  FILE *file;
} gst_app_t;

static gst_app_t gst_app;

#define BUFF_SIZE (1024)

void init_video() {
  cam->open(0); // get camera
  if (!cam->isOpened()) {
    printf("could not open %s\n","/dev/video0");
  }

  Size s = Size((int)cam->get(CV_CAP_PROP_FRAME_WIDTH),
                (int)cam->get(CV_CAP_PROP_FRAME_HEIGHT));
  int fps = (int)cam->get(CV_CAP_PROP_FPS);
  int ex = static_cast<int>(cam->get(CV_CAP_PROP_FOURCC));

  printf("opened video\n");
  printf("video specs: size: %d %d, fps: %d, ex: %d\n", s.width, s.height, fps, ex);
}

static gboolean read_data(gst_app_t *app) {
  GstBuffer *buffer;
  guint8 *ptr;
  gint size;
  GstFlowReturn ret;

  ptr = (guint8 *)g_malloc(BUFF_SIZE);
  g_assert(ptr);
  size = fread(ptr, 1, BUFF_SIZE, app->file);

  if (size == 0) {
    ret = gst_app_src_end_of_stream(app->src);
    g_debug("eos returned %d at %d\n", ret, __LINE__);
    return FALSE;
  }

  buffer = gst_buffer_new();
  GST_BUFFER_MALLOCDATA(buffer) = ptr;
  GST_BUFFER_SIZE(buffer) = size;
  GST_BUFFER_DATA(buffer) = GST_BUFFER_MALLOCDATA(buffer);

  ret = gst_app_src_push_buffer(app->src, buffer);

  if (ret != GST_FLOW_OK) {
    g_debug("push buffer returned %d for %d bytes\n", ret, size);
    return FALSE;
  }

  if (size != BUFF_SIZE) {
    ret = gst_app_src_end_of_stream(app->src);
    g_debug("eos returned %d at %d\n", ret, __LINE__);
    return FALSE;
  }

  return TRUE;
}

static void start_feed(GstElement *pipeline, guint size, gst_app_t *app) {
  if (app->sourceid == 0) {
    GST_DEBUG("start feeding");
    app->sourceid = g_idle_add((GSourceFunc)read_data, app);
  }
}

static void stop_feed(GstElement *pipeline, gst_app_t *app) {
  if (app->sourceid != 0) {
    GST_DEBUG("stop feeding");
    g_source_remove(app->sourceid);
    app->sourceid = 0;
  }
}

static void on_pad_added(GstElement *element, GstPad *pad) {
  GstCaps *caps;
  GstStructure *str;
  gchar *name;
  GstPad *ffmpegsink;
  GstPadLinkReturn ret;

  g_debug("pad added");

  caps = gst_pad_get_caps(pad);
  str = gst_caps_get_structure(caps, 0);

  g_assert(str);

  name = (gchar *)gst_structure_get_name(str);

  g_debug("pad name %s", name);

  if (g_strrstr(name, "video")) {
    ffmpegsink = gst_element_get_pad(gst_app.ffmpeg, "sink");
    g_assert(ffmpegsink);
    ret = gst_pad_link(pad, ffmpegsink);
    g_debug("pad link returned %d\n", ret);
    gst_object_unref(ffmpegsink);
  }

  gst_caps_unref(caps);
}

static gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer *ptr) {
  gst_app_t *app = (gst_app_t *)ptr;

  switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_ERROR: {
      gchar *debug;
      GError *err;
      
      gst_message_parse_error(message, &err, &debug);
      g_print("Error: %s\n", err->message);
      g_error_free(err);
      g_free(debug);
      g_main_loop_quit(app->loop);
    }
    break;
    case GST_MESSAGE_WARNING: {
      gchar *debug;
      GError *err;
      gchar *name;

      gst_message_parse_warning(message, &err, &debug);
      g_print("Name of src %s\n", name ? name : "nil");
      g_error_free(err);
      g_free(debug);
    }
    break;
    case GST_MESSAGE_EOS: {
      g_print("End of Stream\n");
      g_main_loop_quit(app->loop);
    }
    break;
    case GST_MESSAGE_STATE_CHANGED: {
    }
    break;
    default: {
      g_print("got message %s\n", gst_message_type_get_name(GST_MESSAGE_TYPE(message)));
    }
    break;
  }
  return TRUE;
}

static void start_video_buffer(char const *fname) {
  // filesrc ! decodebin ! ffmpegcolorspace ! xvimagesink
  gst_app_t *app = &gst_app;
  GstBus *bus;
  GstStateChangeReturn state_ret;

  app->file = fopen(fname, "r");
  g_assert(app->file);

  gst_init(NULL, NULL);

  // SET UP EVERYTHING
  app->pipeline = (GstPipeline *)gst_pipeline_new("mypipeline");
  bus = gst_pipeline_get_bus(app->pipeline);
  gst_bus_add_watch(bus, (GstBusFunc)bus_callback, app);
  gst_object_unref(bus);

  app->src = (GstAppSrc *)gst_element_factory_make("appsrc", "mysrc");
  app->decoder = gst_element_factory_make("decodebin", "mydecoder");
  app->ffmpeg = gst_element_factory_make("ffmpegcolorspace", "myffmpeg");
  app->xvimagesink = gst_element_factory_make("xvimagesink", "myvsink");

  g_assert(app->src);
  g_assert(app->decoder);
  g_assert(app->ffmpeg);
  g_assert(app->xvimagesink);

  g_signal_connect(app->src, "need-data", G_CALLBACK(start_feed), app);
  g_signal_connect(app->src, "enough-data", G_CALLBACK(stop_feed), app);
  g_signal_connect(app->decoder, "pad-added", G_CALLBACK(on_pad_added), app->decoder);

  gst_bin_add_many(GST_BIN(app->pipeline),
      (GstElement *)app->src,
      app->decoder,
      app->ffmpeg,
      app->xvimagesink,
      NULL);
  
  if (!gst_element_link((GstElement *)app->src, app->decoder)) {
    g_warning("failed to link src and decoder");
  }

  if (!gst_element_link(app->ffmpeg, app->xvimagesink)) {
    g_warning("failed to link ffmpeg and xvsink");
  }

  state_ret = gst_element_set_state((GstElement *)app->pipeline, GST_STATE_PLAYING);
  g_warning("set state returned %d\n", state_ret);

  // here is the loop
  app->loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(app->loop);

  state_ret = gst_element_set_state((GstElement *)app->pipeline, GST_STATE_NULL);
  g_warning("set state null returned %d\n", state_ret);

  return;
}

int main(int argc, char *argv[]) {
  printf("opening icarus.mp4 ...\n");
  start_video_buffer("icarus.mp4");
  printf("done with program\n");
  return 0;
}
