#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "timer.h"
#include "oct.h"
#include "particle.h"

#include <GLFW/glfw3.h>

particle *p;
oct_node *o;

typedef struct opts {
  unsigned n_bodies;
  unsigned n_iters;
} opts;

opts opt;

#define AUTO_REFRESH 60
#define OFFSET 128
#define DEF_WIDTH (3*512)
#define DEF_HEIGHT (3*512)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

double t0; // global start time
double time_step;
double interactions;
double gflops;

int WIDTH = DEF_WIDTH;
int HEIGHT = DEF_HEIGHT;
int IM_W = DEF_WIDTH;
int IM_H = DEF_HEIGHT;
float pt_size = 0.002f;
float line_size = 0.5f;
float alpha = 0.2f;
int my=-1;
int mx=-1;
u8 gl_ok=0;
u8 flip_y=1;
u8 grid=1;

void insert_particle();

extern float randf();
extern float randnf(float mean, float std);

extern void
bh(unsigned n, unsigned steps,
   particle *p, float dt, float eps,
   oct_node *o);

void *work(void *args);
void randomize(void);

static GLFWwindow* window;

static void change_size_callback(GLFWwindow* window, int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  glfwSetWindowSize(window, width, height);
}

static void error_callback(int error, const char* msg) {
  fprintf(stderr, "glfw error: %d, %s\n", error, msg);
}

static void mouse_pos_callback(GLFWwindow* window, double x, double y) {
  int xpos, ypos, width, height;
  glfwGetWindowPos(window, &xpos, &ypos);
  glfwGetWindowSize(window, &width, &height);
  mx = (int)(((double)IM_W*x)/(double)WIDTH);
  my = (int)(-0.5+((double)IM_H*y)/(double)HEIGHT);
}

//static void mouse_btn_callback(GLFWwindow* window, int button, int action, int mods) { }
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
  if (action == GLFW_PRESS && key == GLFW_KEY_R) randomize();
  if (action == GLFW_PRESS && key == GLFW_KEY_I) insert_particle();
  if (action == GLFW_PRESS && key == GLFW_KEY_O) oct_print(o);
  if (action == GLFW_PRESS && key == GLFW_KEY_P) { for (unsigned i=0; i<opt.n_bodies; i++) particle_print(&p[i]); }
  if (action == GLFW_PRESS && key == GLFW_KEY_0) { pt_size += 0.0005f; printf("pt_size = %f\n", pt_size); }
  if (action == GLFW_PRESS && key == GLFW_KEY_9) { pt_size -= 0.0005f; printf("pt_size = %f\n", pt_size); }
  if (action == GLFW_PRESS && key == GLFW_KEY_8) { line_size += 0.01f; printf("line_size = %f\n", line_size); }
  if (action == GLFW_PRESS && key == GLFW_KEY_7) { line_size -= 0.01f; printf("line_size = %f\n", line_size); }
  if (action == GLFW_PRESS && key == GLFW_KEY_6) { alpha += 0.05f; printf("alpha = %f\n", alpha); }
  if (action == GLFW_PRESS && key == GLFW_KEY_5) { alpha -= 0.05f; printf("alpha = %f\n", alpha); }
  printf("scancode = %d, modes=%d\n", scancode, mods);
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
    //glfwSetMouseButtonCallback(window, mouse_btn_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetWindowSizeCallback(window, change_size_callback);

    return window;

}

void insert_particle() {
  unsigned i = o->bodies;
  if (i < opt.n_bodies) {
    p[i].pos.x = 2.0f * randf() - 1.0f;
    p[i].pos.y = 2.0f * randf() - 1.0f;
    p[i].pos.z = 2.0f * randf() - 1.0f;
    //p[i].pos.x = randnf((i%2==0) ? -0.1f : 0.1f, 0.1f);
    //p[i].pos.x = randnf(0.0f, 0.2f);
    //p[i].pos.y = randnf(0.0f, 0.2f);
    //p[i].pos.z = randnf(0.0f, 0.2f);
    //p[i].pos.y = randnf((i%2==0) ? 0.1f : -0.1f, 0.15f);
    //p[i].pos.z = randnf(0.0f, 0.1f);
    p[i].mass = randnf(2.0f, 0.5f);
    if (p[i].mass < 0) p[i].mass = 0.01f;
    p[i].idx = i;
    oct_insert(o, &p[i]);
  }
}

void draw_oct(oct_node *o) {

  if (0 == o) return;

  glLineWidth(line_size);
  glColor4f(.5f, .5f ,.5f, alpha);

  glBegin(GL_LINE_STRIP);
    /* front face */
    glVertex3f(o->a[X], o->a[Y], o->a[Z]);
    glVertex3f(o->b[X], o->a[Y], o->a[Z]);
    glVertex3f(o->b[X], o->b[Y], o->a[Z]);
    glVertex3f(o->a[X], o->b[Y], o->a[Z]);
    glVertex3f(o->a[X], o->a[Y], o->a[Z]);
    /* front face */
  glEnd();

  /* mass center */
  //glBegin(GL_TRIANGLES);

  //  float x = o->mass_center[0];
  //  float y = o->mass_center[1];
  //  float z = o->mass_center[2];
  //  float w = 0.001 * o->mass_total;
  //  float r = w*pt_size;

  //  glColor4f(0.5-z, 0.5, 0.5+z, 0.5);

  //  glVertex3f(x, y+r, z);
  //  glVertex3f(x-r, y-r, z);
  //  glVertex3f(x+r, y-r, z);

  //glEnd();

  for (int i=0; i<CAP; i++)
    draw_oct(o->c[i]);
}

static void draw() {

    /* oct tree */
    //draw_oct(o);

    /* bodies */
    glBegin(GL_QUADS);
    for (unsigned int i=0; i<o->bodies; i++) {
      glColor4f(0.5-p[i].pos.z,
      0.5, 0.5+p[i].pos.z, 0.5+p[i].pos.z*0.1);
      glVertex3f(p[i].pos.x-pt_size, p[i].pos.y-pt_size, p[i].pos.z);
      glVertex3f(p[i].pos.x+pt_size, p[i].pos.y-pt_size, p[i].pos.z);
      glVertex3f(p[i].pos.x+pt_size, p[i].pos.y+pt_size, p[i].pos.z);
      glVertex3f(p[i].pos.x-pt_size, p[i].pos.y+pt_size, p[i].pos.z);
      //glVertex3f(0.003*p[i].mass+p[i].pos.x,
      //p[i].pos.y, p[i].pos.z);
      //glVertex3f(0.003*p[i].mass+p[i].pos.x,
      //0.003*p[i].mass+p[i].pos.y, p[i].pos.z);
      //glVertex3f(p[i].pos.x,
      //0.003*p[i].mass+p[i].pos.y, p[i].pos.z  );
    }
    glEnd();

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
    sprintf(wtitle, "%5.1f FPS, %d bodies,"
    " %1.3f s/step",
      fps, opt.n_bodies, time_step);
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

  opt.n_bodies = argc > 1 ? strtol(argv[1], NULL, 10) : 5000;
  opt.n_iters  = argc > 2 ? strtol(argv[2], NULL, 10) : 1;

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

  printf("work (args=%p)\n", args);
  printf("num bodies=%u, iters=%i\n",
      opt.n_bodies, opt.n_iters);

  p =  calloc(opt.n_bodies, sizeof(particle));
  o = oct_alloc();
  oct_reset(o);

  //randomize();
  for (unsigned i=0; i<opt.n_bodies; i++)
    insert_particle();

  interactions =
    (double)(
        opt.n_bodies *
        opt.n_bodies *
        opt.n_iters);

  gflops = interactions * 20 / (1 << 30);

  while (!gl_ok) { delay(10); }
  {
    while (gl_ok) {
      double t;
      timeit2(bh(
              opt.n_bodies,
              opt.n_iters, p, dt,
              eps, o), &t);
      time_step = t;
    }
  }

  free(p); oct_release(o);
  return 0;
}

void randomize() {
  for (unsigned i=0; i<opt.n_bodies; i++) {
    p[i].pos.x = randnf(0.0, 0.5f);
    p[i].pos.y = randnf(0.0, 0.5f);
    p[i].pos.z = randnf(0.0, 0.5f);
    p[i].vel.x = 50.f * (randf() - 0.5f);
    p[i].vel.y = 50.f * (randf() - 0.5f);
    p[i].vel.z = 50.f * (randf() - 0.5f);
    p[i].mass = randnf(0.5,2); p[i].idx = i;
  }
}
