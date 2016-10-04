/*	mesh
 *
 *		written by Alexander Zaprjagaev
 *		frustum@public.tsu.ru
 */

#ifndef __MESH_H__
#define __MESH_H__

typedef struct {
	float xyz[3];				/* координата */
	float normal[3];			/* нормаль */
	float st[2];				/* текстурные координаты */
} mesh_vertex_t;

typedef struct {
	int num_vertex;				/* число вершин */
	mesh_vertex_t *vertex;			/* вершины */
	float matrix[16];			/* матрица преобразования */
} mesh_t;

mesh_t *mesh_create(float *vertex,int num_vertex);
mesh_t *mesh_load_3ds(char *name);
void mesh_set_matrix(mesh_t *mesh,float *matrix);
void mesh_render(mesh_t *mesh);

#endif /* __MESH_H__ */
