/*	mesh
 *
 *		written by Alexander Zaprjagaev
 *		frustum@public.tsu.ru
 */

#include <stdio.h>
#include <malloc.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include "load3ds.h"
#include "mathlib.h"
#include "mesh.h"

/* создаем меш
 */
mesh_t *mesh_create(float *vertex,int num_vertex) {
	int i,j;
	mesh_t *mesh;
	mesh = calloc(1,sizeof(mesh_t));
	mesh->num_vertex = num_vertex;
	mesh->vertex = calloc(1,sizeof(mesh_vertex_t) * num_vertex);
	for(i = j = 0; i < mesh->num_vertex; i++, j += 8) {
		v_copy(&vertex[j + 0],mesh->vertex[i].xyz);
		v_copy(&vertex[j + 3],mesh->vertex[i].normal);
		mesh->vertex[i].st[0] = vertex[j + 6];
		mesh->vertex[i].st[1] = vertex[j + 7];
	}
	m_identity(mesh->matrix);
	return mesh;
}

/* закрузка 3ds файла
 */
mesh_t *mesh_load_3ds(char *name) {
	int num_vertex;
	float *vertex;
	mesh_t *mesh;
	vertex = load_3ds(name,&num_vertex);
	if(!vertex) return NULL;
	mesh = mesh_create(vertex,num_vertex);
	free(vertex);
	return mesh;
}

/* матрица преобразования
 */
void mesh_set_matrix(mesh_t *mesh,float *matrix) {
	memcpy(mesh->matrix,matrix,sizeof(float) * 16);
}

/* рендериг меша
 */
void mesh_render(mesh_t *mesh) {
	glPushMatrix();
	/* применили матрицу преобразования */
	glMultMatrixf(mesh->matrix);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(mesh_vertex_t),mesh->vertex->xyz);
	glNormalPointer(GL_FLOAT,sizeof(mesh_vertex_t),mesh->vertex->normal);
	glTexCoordPointer(2,GL_FLOAT,sizeof(mesh_vertex_t),mesh->vertex->st);
	/* рендерим */
	glDrawArrays(GL_TRIANGLES,0,mesh->num_vertex);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}
