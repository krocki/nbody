#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"

#define GL_SILENCE_DEPRECATION 1
#include <GLFW/glfw3.h>

float *x;
float *y;
float *z;
float *vx;
float *vy;
float *vz;
float *m;

typedef struct opts {
  unsigned n_bodies;
  unsigned n_iters;
} opts;

opts opt;

#define AUTO_REFRESH 60
#define OFFSET 64

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

double t0; // global start time
int WIDTH = 512;
int HEIGHT = 512;
int IM_W = 512;
int IM_H = 512;
int my=-1;
int mx=-1;
u8 gl_ok=0;
u8 flip_y=1;
u8 grid=1;

extern float randf();
extern float randnf(float mean, float std);

extern void
sim(unsigned n, unsigned steps,
    float *x,  float *y,  float *z,
    float *vx, float *vy, float *vz,
    float *m,  float dt,  float eps);

void *work(void *args);
void randomize(void);

static GLFWwindow* window;

static void change_size_callback(GLFWwindow* window, int width, int height) {
  WIDTH = width;
  HEIGHT = height;
}

static void error_callback(int error, const char* msg) { }
static void mouse_pos_callback(GLFWwindow* window, double x, double y) {
  int xpos, ypos, width, height;
  glfwGetWindowPos(window, &xpos, &ypos);
  glfwGetWindowSize(window, &width, &height);
  mx = (int)(((double)IM_W*x)/(double)WIDTH);
  my = (int)(-0.5+((double)IM_H*y)/(double)HEIGHT);
}

static void mouse_btn_callback(GLFWwindow* window, int button, int action, int mods) { }
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (action == GLFW_PRESS && key == GLFW_KEY_R) randomize();
}

static GLFWwindow* open_window(const char* title, GLFWwindow* share, int posX, int posY) {

    GLFWwindow* window;

    //glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, share);
    if (!window) return NULL;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetWindowPos(window, posX, posY);
    glfwShowWindow(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_btn_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetWindowSizeCallback(window, change_size_callback);

    return window;

}

void draw_point(float x, float y, float r, float g, float b, float a) {

    float incr_x = 1.0f/(float)IM_W;
    float incr_y = 1.0f/(float)IM_H;
    glColor4f(r, g, b, a);
    float i = x * incr_x;
    float j = y * incr_y;
    j = flip_y ? 1-j : j; // FLIP vert
    glVertex2f(i,      j     );     glVertex2f(i+incr_x, j     );
    glVertex2f(i+incr_x, j+incr_y); glVertex2f(i,      j+incr_y);
}

static void draw() {

//    int width, height;
//    glfwGetFramebufferSize(glfwGetCurrentContext(),
//        &width, &height);
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glMatrixMode ( GL_MODELVIEW );
//    glLoadIdentity();
//    //glOrtho(0.f, 1.f, 0.f, 1.f, 0.f, 1.f);
//
//    if (1 == grid) {
//      glLineWidth(1.0f);
//      glBegin(GL_LINES);
//
//      int num = 4;
//      float wf = 1.0f/((float)num);
//
//      // horizontal
//      for (int i=1; i<num; i++) {
//        glColor4f(0.35f, 0.35f, 0.35f, (i%16==0)? 0.3f : 0.15f);
//        glVertex2f( 0.0f, i * wf); glVertex2f( 1.0f, i * wf);
//      }
//
//      // vertical
//      for (int i=1; i<num; i++) {
//        glColor4f(0.35f, 0.35f, 0.35f, (i%16==0)? 0.3f : 0.15f);
//        glVertex2f( i * wf, 1.0f ); glVertex2f( i * wf, 0.0f );
//      }
//      glEnd();
//    }
//
    /* bodies */
    glBegin(GL_QUADS);
    for (unsigned int i=0; i<opt.n_bodies; i++) {
      glColor4f(0.5-z[i], 0.5, 0.5+z[i], 0.5+z[i]*0.1);
      glVertex3f(x[i], y[i], z[i]  );
      glVertex3f(0.003*m[i]+x[i], y[i], z[i]  );
      glVertex3f(0.003*m[i]+x[i], 0.003*m[i]+y[i], z[i]  );
      glVertex3f(x[i], 0.003*m[i]+y[i], z[i]  );
    }
    glEnd();

    //glBegin(GL_QUADS);
    //// cursor
    //draw_point(mx, my, 1, 0, 0, 0.8);
    //glEnd();

}

int display_init() {
  int x, y, width;
  t0=get_time();
  //glfwSetErrorCallback(error_callback);
  if (!glfwInit()) return -1;

  window = open_window("quad", NULL, OFFSET, OFFSET);
  if (!window) { glfwTerminate(); return -1; }

  glfwGetWindowPos(window, &x, &y);
  glfwGetWindowSize(window, &width, NULL);
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  gl_ok=1;
  printf("%9.6f, GL_init OK\n", get_time()-t0);

  glEnable ( GL_POINT_SMOOTH );
  glEnable( GL_LINE_SMOOTH );
  glEnable( GL_POLYGON_SMOOTH );
  glHint ( GL_POINT_SMOOTH_HINT, GL_NICEST );
  glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
  glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
  glEnable ( GL_BLEND );
  glBlendFunc ( GL_SRC_ALPHA, GL_ONE );

  double frame_start=get_time();
  while (!glfwWindowShouldClose(window))
  {
    double fps = 1.0/(get_time()-frame_start);
    char wtitle[256]; wtitle[255] = '\0';
    frame_start = get_time();
    sprintf(wtitle, "%4.1f FPS", fps);
    glfwSetWindowTitle(window, wtitle);
    glfwMakeContextCurrent(window);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    glfwSwapBuffers(window);
    if (AUTO_REFRESH > 0) glfwWaitEventsTimeout(1.0/(double)AUTO_REFRESH - (get_time()-frame_start));
    else glfwWaitEvents();
  }

  glfwTerminate();
  printf("%9.6f, GL terminating\n", get_time()-t0);
  gl_ok=0;
  return 0;
}

int main(int argc, char **argv) {

  opt.n_bodies = argc > 1 ? strtol(argv[1], NULL, 10) : 7500;
  opt.n_iters  = argc > 2 ? strtol(argv[2], NULL, 10) : 10;

  pthread_t work_thread;
  if(pthread_create(&work_thread, NULL, work, NULL)) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
  }

  display_init();

  if(pthread_join(work_thread, NULL)) {
    fprintf(stderr, "Error joining thread\n");
    return 2;
  }

  return 0;

}

void *work(void *args) {

  float dt  = 0.00005f; /* time step */
  float eps = 1e-5f; /* softening */

  printf("num bodies=%u, iters=%i\n",
      opt.n_bodies, opt.n_iters);

  x =  calloc(opt.n_bodies, sizeof(float));
  y =  calloc(opt.n_bodies, sizeof(float));
  z =  calloc(opt.n_bodies, sizeof(float));
  vx = calloc(opt.n_bodies, sizeof(float));
  vy = calloc(opt.n_bodies, sizeof(float));
  vz = calloc(opt.n_bodies, sizeof(float));
  m =  calloc(opt.n_bodies, sizeof(float));

  randomize();

  while (!gl_ok) usleep(10);
  {
    while (gl_ok) {
      double t;
      timeit2(sim(
              opt.n_bodies,
              opt.n_iters, x,  y,  z,
              vx, vy, vz, m, dt, eps), &t);

      double mflops = 18 *
        (double)(
            opt.n_bodies *
            opt.n_bodies *
            opt.n_iters) / (1 << 20);

      printf("mflops: %.3f, time: %8.4f s\n"
             "time/iteration: %10.6f s\n%5.2f mflop/s, "
             "%5.3f M interactions/s\n",
             mflops, t, t/opt.n_iters, mflops/t,
             (opt.n_bodies * opt.n_bodies)/(t * (1 << 20)));
    }

  }

  free(x), free(y), free(z);
  free(vx), free(vy), free(vz);
  free(m);

  return 0;
}

void randomize() {
  for (unsigned i=0; i<opt.n_bodies; i++) {
    x[i] = randnf(0.0, 0.5f);
    y[i] = randnf(0.0, 0.5f);
    z[i] = randnf(0.0, 0.5f);
    vx[i] = 50.f * (randf() - 0.5f);
    vy[i] = 50.f * (randf() - 0.5f);
    vz[i] = 50.f * (randf() - 0.5f);
    m[i] = randnf(0.5,2);
  }
}
