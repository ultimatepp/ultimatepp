/*	shadow volume demo
 *
 *		written by Alexander Zaprjagaev
 *		frustum@public.tsu.ru
 */

#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL.h"
#include "mathlib.h"
#include "load3ds.h"
#include "sv_mesh.h"
#include "mesh.h"

#define WIDTH	800
#define HEIGHT	600

int my_pause,my_view;

float fps;

float phi = -30,psi = -90,dist = 10;
float camera[3],dir[3];

float light[4] = { 0, 0, 0, 1 },light_speed = 5,light_rot = 180;
float shadow[4] = { 0, 0, 0, 0.5 };

mesh_t *room;

sv_mesh_t *staircase,*motor,*cooler,*box,*barrel,*shelf,*mesh;
sv_mesh_t *computer,*lcd,*door,*big_wire;
float staircase_pos[3] = { 0,0,0 };
float motor_pos[3] = { 0, 5.856, 2.549 };
float cooler_pos[3] = { 0, 4.024, 2.542 },cooler_speed = -30,cooler_rot;
float box_pos[3] = { -4.268, 1.343, 3.349 };
float barrel_pos[3] = { -4.301, 3.587, -0.018 };
float shelf_pos[3] = { 4.744, 1.680, 0.215 };
float mesh_pos[3] = { 0, 0, 2.000 },mesh_speed = 20,mesh_rot;
float computer_pos[3] = { 4.693, 3.367, 0.654 };
float big_wire_pos[4][3] = {{ 1.004, 0.983, 1.576 }, { 1.004,-1.041, 1.576 },
							{-1.004,-1.041, 1.576 }, {-1.004, 0.983, 1.576 }};
float big_wire_rot[4] = { 45, -45, -135, 135 };

/*
 */
void load_mesh(char *name,float *pos,mesh_t **mesh) {
	float m[16];
	*mesh = mesh_load_3ds(name);
	if(pos) {
		m_translate(pos,m);
		mesh_set_matrix(*mesh,m);
	}
}

void load_sv_mesh(char *name,float *pos,sv_mesh_t **mesh) {
	float m[16];
	*mesh = sv_mesh_load_3ds(name);
	if(pos) {
		m_translate(pos,m);
		sv_mesh_set_matrix(*mesh,m);
	}
}

/* init
 */
int init(void) {
	glClearDepth(1);
	glClearColor(0,0,0,0);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPointSize(4);

	load_mesh("data/room.3ds",NULL,&room);

	load_sv_mesh("data/staircase.3ds",staircase_pos,&staircase);
	load_sv_mesh("data/motor.3ds",motor_pos,&motor);
	load_sv_mesh("data/cooler.3ds",NULL,&cooler);
	load_sv_mesh("data/box.3ds",box_pos,&box);
	load_sv_mesh("data/barrel.3ds",barrel_pos,&barrel);
	load_sv_mesh("data/shelf.3ds",shelf_pos,&shelf);
	load_sv_mesh("data/mesh.3ds",NULL,&mesh);
	load_sv_mesh("data/computer.3ds",computer_pos,&computer);
	load_sv_mesh("data/lcd.3ds",computer_pos,&lcd);
	load_sv_mesh("data/big_wire.3ds",NULL,&big_wire);

	return 0;
}

/* get fps
 */
float getfps(void) {
	static float fps = 60;
	static int starttime,endtime,counter;
	if(counter == 10) {
		endtime = starttime;
		starttime = SDL_GetTicks();
		fps = counter * 1000.0 / (float)(starttime - endtime);
		counter = 0;
	}
	counter++;
	return fps;
}

/*
 */
void render(void) {
	int i;
	float m0[16],m1[16],m2[16];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,WIDTH / HEIGHT,1,500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera[0],camera[1],camera[2], dir[0],dir[1],dir[2], 0,0,1);
	glLightfv(GL_LIGHT0,GL_POSITION,light);

	glColor3f(1,1,1);
	/* нарисовали лампочку */
	glBegin(GL_POINTS);
	glVertex3fv(light);
	glEnd();
	/* рисуем все объекты */
	glEnable(GL_LIGHTING);
	mesh_render(room);
	sv_mesh_render(staircase);
	sv_mesh_render(motor);
	sv_mesh_render(cooler);
	sv_mesh_render(box);
	sv_mesh_render(barrel);
	sv_mesh_render(shelf);
	sv_mesh_render(mesh);
	sv_mesh_render(computer);
	sv_mesh_render(lcd);
	for(i = 0; i < 4; i++) {
		m_translate(big_wire_pos[i],m0);
		m_rotation_z(big_wire_rot[i],m1);
		m_multiply(m0,m1,m2);
		sv_mesh_set_matrix(big_wire,m2);
		sv_mesh_render(big_wire);
	}
	glDisable(GL_LIGHTING);
	/* рисуем все тени в стенсиль */
	sv_mesh_render_to_stencil(staircase,light);
	sv_mesh_render_to_stencil(motor,light);
	sv_mesh_render_to_stencil(cooler,light);
	sv_mesh_render_to_stencil(box,light);
	sv_mesh_render_to_stencil(barrel,light);
	sv_mesh_render_to_stencil(shelf,light);
	sv_mesh_render_to_stencil(mesh,light);
	sv_mesh_render_to_stencil(computer,light);
	sv_mesh_render_to_stencil(lcd,light);
	for(i = 0; i < 4; i++) {
		m_translate(big_wire_pos[i],m0);
		m_rotation_z(big_wire_rot[i],m1);
		m_multiply(m0,m1,m2);
		sv_mesh_set_matrix(big_wire,m2);
		sv_mesh_render_to_stencil(big_wire,light);
	}
	/* рисуем тень */
	sv_mesh_render_shadow(shadow);

	SDL_GL_SwapBuffers();
}

/*
 */
void idle(void) {
	float ifps,rad,v[3],q0[4],q1[4],q2[3],m0[16],m1[16],m2[16];

	fps = getfps();
	ifps = 1.0 / fps;

	if(my_pause != 2) cooler_rot += ifps * 360.0 * cooler_speed / 60.0;
	m_translate(cooler_pos,m0);
	m_rotation_y(cooler_rot,m1);
	m_multiply(m0,m1,m2);
	sv_mesh_set_matrix(cooler,m2);

	if(my_pause != 2) mesh_rot += ifps * 360.0 * mesh_speed / 60.0;
	m_translate(mesh_pos,m0);
	m_rotation_z(mesh_rot,m1);
	m_multiply(m0,m1,m2);
	m_rotation_x(mesh_rot / 1.5,m0);
	m_multiply(m2,m0,m1);
	sv_mesh_set_matrix(mesh,m1);

	if(my_pause == 0) light_rot += ifps * 360 * light_speed / 60.0;
	rad = light_rot * DEG2RAD;
	v_set(sin(rad) * 3,cos(rad) * 3,0,v);
	v_set(0,0,2.5,light);
	v_add(light,v,light);

	v_set(0,0,1,v);
	q_set(v,psi,q0);
	v_set(0,1,0,v);
	q_set(v,phi,q1);
	q_multiply(q0,q1,q2);
	q_to_matrix(q2,m0);
	v_set(dist,0,0,camera);
	v_transform(camera,m0,camera);
	if(my_view == 0) v_copy(mesh_pos,dir)
	else v_copy(light,dir)
	v_add(camera,dir,camera);
}

/*
 */
void keyboard(int key) {
	static int wareframe;
	switch(key) {
		case SDLK_ESCAPE:
			SDL_Quit();
			exit(0);
			break;
		case SDLK_SPACE:
			my_pause++;
			if(my_pause == 3) my_pause = 0;
			break;
		case SDLK_w:
			wareframe = !wareframe;
			glPolygonMode(GL_FRONT_AND_BACK,wareframe ? GL_LINE : GL_FILL);
			break;
		case SDLK_v:
			my_view = !my_view;
			break;
		case SDLK_l:
			light[3] = !light[3];
			break;
	}
}

/*
 */
void mouse(int button,int state,int x,int y) {
	if(button == 4) dist -= 1;
	if(button == 5) dist += 1;
	if(dist < 1) dist = 1;
	if(dist > 100) dist = 100;
	psi += (x - WIDTH / 2) * 0.2;
	phi += (y - HEIGHT / 2) * 0.2;
	if(phi < -89) phi = -89;
	if(phi > 89) phi = 89;
}

/*
 */
int main(int argc,char **argv) {
	SDL_Surface *screen;
	int done;

	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_OPENGL | SDL_FULLSCREEN);
	if(!screen) {
		printf("couldn`t set video mode: %s\n",SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	init();

	SDL_ShowCursor(SDL_DISABLE);
	SDL_WarpMouse(WIDTH / 2,HEIGHT / 2);

	done = 0;
	while(!done) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					done = 1;
					break;
				case SDL_KEYDOWN:
					keyboard(event.key.keysym.sym);
					break;
				case SDL_MOUSEMOTION:
					mouse(0,0,event.motion.x,event.motion.y);
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					mouse(event.button.button,event.button.state,
						event.button.x,event.button.y);
					break;
			}
		}
		SDL_WarpMouse(WIDTH / 2,HEIGHT / 2);
		idle();
		render();
	}

	SDL_Quit();
	return 0;
}
