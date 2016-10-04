/*	shadow volume mesh
 *
 *		written by Alexander Zaprjagaev
 *		frustum@public.tsu.ru
 */

#ifndef __SV_MESH_H__
#define __SV_MESH_H__

typedef struct {
	float xyz[3];				/* координата */
	float normal[3];			/* нормаль */
	float st[2];				/* текстурные координаты */
} vertex_t;

typedef struct {
	float xyz[3];				/* координата */
} sv_vertex_t;

typedef struct {
	sv_vertex_t *v0,*v1;		/* вершины */
	int rv;						/* для правильного направления обхода */
	int id;						/* идентификатор */
} sv_edge_t;

typedef struct {
	sv_vertex_t *v0,*v1,*v2;	/* вершины */
	sv_edge_t *e0,*e1,*e2;		/* ребра */
	int r0,r1,r2;				/* реверс */
	float plane[4];				/* плоскость */
} sv_face_t;

typedef struct {
	int num_vertex;				/* число вершин */
	sv_vertex_t *vertex;		/* вершины */
	int num_edge;				/* число ребер */
	sv_edge_t *edge;			/* ребра */
	int num_face;				/* число граней */
	sv_face_t *face;			/* грани */
} sv_t;

typedef struct {
	int num_vertex;				/* число вершин */
	vertex_t *vertex;			/* вершины */
	float matrix[16];			/* матрица преобразования */
	sv_t *sv;					/* теневой объем */
} sv_mesh_t;

sv_mesh_t *sv_mesh_create(float *vertex,int num_vertex);
sv_mesh_t *sv_mesh_load_3ds(char *name);
void sv_mesh_set_matrix(sv_mesh_t *mesh,float *matrix);
void sv_mesh_render(sv_mesh_t *mesh);
void sv_mesh_render_to_stencil(sv_mesh_t *mesh,float *light);
void sv_mesh_render_shadow(float *color);

#endif /* __SV_MESH_H__ */
