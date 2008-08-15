/*	shadow volume mesh
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
#include "sv_mesh.h"

/* создаем меш
 */
sv_mesh_t *sv_mesh_create(float *vertex,int num_vertex) {
	int i,j,num,*vbuf,*ebuf,*rbuf;
	sv_vertex_t *v;
	sv_edge_t *e;
	sv_face_t *f;
	sv_mesh_t *mesh;
	mesh = calloc(1,sizeof(sv_mesh_t));
	mesh->num_vertex = num_vertex;
	mesh->vertex = calloc(1,sizeof(vertex_t) * num_vertex);
	mesh->sv = calloc(1,sizeof(sv_t));
	mesh->sv->num_vertex = num_vertex;
	mesh->sv->vertex = calloc(1,sizeof(sv_vertex_t) * num_vertex);
	mesh->sv->num_edge = num_vertex;
	mesh->sv->edge = calloc(1,sizeof(sv_edge_t) * num_vertex);
	mesh->sv->num_face = num_vertex / 3;
	mesh->sv->face = calloc(1,sizeof(sv_face_t) * num_vertex / 3);
	/* копируем вершины */
	for(i = j = 0; i < mesh->num_vertex; i++, j += 8) {
		v_copy(&vertex[j + 0],mesh->vertex[i].xyz);
		v_copy(&vertex[j + 3],mesh->vertex[i].normal);
		mesh->vertex[i].st[0] = vertex[j + 6];
		mesh->vertex[i].st[1] = vertex[j + 7];
		v_copy(&vertex[j + 0],mesh->sv->vertex[i].xyz);
	}
	/* оптимизируем вершины для теневого объема */
	vbuf = calloc(1,sizeof(int) * mesh->sv->num_vertex);
	for(i = num = 0, v = mesh->sv->vertex; i < mesh->sv->num_vertex; i++) {
		j = num - 1;
		while(j >= 0 && (v[i].xyz[0] != v[j].xyz[0] ||
						 v[i].xyz[1] != v[j].xyz[1] ||
						 v[i].xyz[2] != v[j].xyz[2])) j--;
		if(j < 0) {
			vbuf[i] = num;
			v_copy(v[i].xyz,v[num].xyz);
			num++;
		} else vbuf[i] = j;
	}
	mesh->sv->num_vertex = num;
	/* создаем ребра */
	for(i = j = 0, e = mesh->sv->edge; i < mesh->sv->num_face; i++, j += 3) {
		sv_vertex_t *v0,*v1,*v2;
		v0 = &v[vbuf[j + 0]];
		v1 = &v[vbuf[j + 1]];
		v2 = &v[vbuf[j + 2]];
		e[j + 0].v0 = v0;
		e[j + 0].v1 = v2;
		e[j + 1].v0 = v2;
		e[j + 1].v1 = v1;
		e[j + 2].v0 = v1;
		e[j + 2].v1 = v0;
	}
	/* оптимизируем ребра */
	ebuf = calloc(1,sizeof(int) * mesh->sv->num_edge);
	rbuf = calloc(1,sizeof(int) * mesh->sv->num_edge);
	for(i = num = 0; i < mesh->sv->num_edge; i++) {
		j = num - 1;
		while(j >= 0 && !(e[i].v0 == e[j].v0 && e[i].v1 == e[j].v1) &&
						!(e[i].v0 == e[j].v1 && e[i].v1 == e[j].v0)) j--;
		if(j < 0) {
			ebuf[i] = num;
			rbuf[i] = 0;
			e[num].v0 = e[i].v0;
			e[num].v1 = e[i].v1;
			num++;
		} else {
			ebuf[i] = j;
			rbuf[i] = (e[i].v0 == e[j].v1 && e[i].v1 == e[j].v0);
		}
	}
	mesh->sv->num_edge = num;
	/* создаем грани */
	for(i = j = 0, f = mesh->sv->face; i < mesh->sv->num_face; i++, j += 3) {
		float p00[3],p10[3],p01[3],v10[3],v01[3];
		f[i].v0 = &v[vbuf[j + 0]];
		f[i].v1 = &v[vbuf[j + 1]];
		f[i].v2 = &v[vbuf[j + 2]];
		f[i].e0 = &e[ebuf[j + 0]];
		f[i].e1 = &e[ebuf[j + 1]];
		f[i].e2 = &e[ebuf[j + 2]];
		f[i].r0 = rbuf[j + 0];
		f[i].r1 = rbuf[j + 1];
		f[i].r2 = rbuf[j + 2];
		v_copy(f[i].v0->xyz,p00);
		v_copy(f[i].v1->xyz,p10);
		v_copy(f[i].v2->xyz,p01);
		v_sub(p10,p00,v10);
		v_sub(p01,p00,v01);
		v_cross(v10,v01,f[i].plane);
		v_normalize(f[i].plane,f[i].plane);
		f[i].plane[3] = -v_dot(f[i].plane,f[i].v0->xyz);
	}
	free(rbuf);
	free(ebuf);
	free(vbuf);
	m_identity(mesh->matrix);
	return mesh;
}

/* закрузка 3ds файла
 */
sv_mesh_t *sv_mesh_load_3ds(char *name) {
	int num_vertex;
	float *vertex;
	sv_mesh_t *mesh;
	vertex = load_3ds(name,&num_vertex);
	if(!vertex) return NULL;
	mesh = sv_mesh_create(vertex,num_vertex);
	free(vertex);
	return mesh;
}

/* матрица преобразования
 */
void sv_mesh_set_matrix(sv_mesh_t *mesh,float *matrix) {
	memcpy(mesh->matrix,matrix,sizeof(float) * 16);
}

/* рендериг меша
 */
void sv_mesh_render(sv_mesh_t *mesh) {
	glPushMatrix();
	/* применили матрицу преобразования */
	glMultMatrixf(mesh->matrix);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(vertex_t),mesh->vertex->xyz);
	glNormalPointer(GL_FLOAT,sizeof(vertex_t),mesh->vertex->normal);
	glTexCoordPointer(2,GL_FLOAT,sizeof(vertex_t),mesh->vertex->st);
	/* рендерим */
	glDrawArrays(GL_TRIANGLES,0,mesh->num_vertex);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

/* находим границу меша
 */
static void sv_mesh_silhouette(sv_t *sv,float *matrix,float *light) {
	int i;
	/* все ребра невидимы */
	for(i = 0; i < sv->num_edge; i++) sv->edge[i].id = -1;
	for(i = 0; i < sv->num_face; i++) {
		if(sv->face[i].plane[0] * light[0] +
		   sv->face[i].plane[1] * light[1] +
		   sv->face[i].plane[2] * light[2] +
		   sv->face[i].plane[3] * light[3] > 0) {
			/* если грань видно из лампочки
			 * к идентификаторам ребер грани добавим по единичке
			 * если в итоге идентификатор равен 0 то это ребро - граница
			 * кроме того зададим направление обхода */
			sv->face[i].e0->id++;
			sv->face[i].e1->id++;
			sv->face[i].e2->id++;
			sv->face[i].e0->rv = sv->face[i].r0;
			sv->face[i].e1->rv = sv->face[i].r1;
			sv->face[i].e2->rv = sv->face[i].r2;
		}
	}
}

/* рендеринг теневого объема
 */
static void sv_mesh_render_shadow_volume(sv_t *sv,float *light) {
	int i;
	glBegin(GL_QUADS);
	for(i = 0; i < sv->num_edge; i++) {
		if(sv->edge[i].id == 0) {
			/* если ребро это граница */
			float *v0,*v1;
			if(!sv->edge[i].rv) {
				v0 = sv->edge[i].v0->xyz;
				v1 = sv->edge[i].v1->xyz;
			} else {
				v0 = sv->edge[i].v1->xyz;
				v1 = sv->edge[i].v0->xyz;
			}
			/* вершины на меше */
			glVertex4f(v0[0],v0[1],v0[2],1);
			glVertex4f(v1[0],v1[1],v1[2],1);
			/* вершины на бесконечности */
			glVertex4f(v1[0] * light[3] - light[0],
					   v1[1] * light[3] - light[1],
					   v1[2] * light[3] - light[2],0);
			glVertex4f(v0[0] * light[3] - light[0],
					   v0[1] * light[3] - light[1],
					   v0[2] * light[3] - light[2],0);
		}
	}
	glEnd();
}

/* рендеринг теневого объема в стенсиль буфер
 */
void sv_mesh_render_to_stencil(sv_mesh_t *mesh,float *light) {
	float imatrix[16],ilight[3];
	glPushMatrix();
	/* применили матрицу преобразования */
	glMultMatrixf(mesh->matrix);
	if(light[3] == 0) {
		/* если лампочка на бесконечности то к ее координатам добавим
		 * координаты объекта */
		v_translate(mesh->matrix,ilight);
		v_add(ilight,light,ilight);
	} else {
		/* лампочка рядом ее координаты не трогаем */
		v_copy(light,ilight);
	}
	/* координаты лампочки умножаем на обратную матрицу преобразования */
	m_inverse(mesh->matrix,imatrix);
	v_transform(ilight,imatrix,ilight);
	ilight[3] = light[3];
	/* нашли границу */
	sv_mesh_silhouette(mesh->sv,mesh->matrix,ilight);
	/* рендерим только в стенсиль */
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS,0,~0);
	/* передняя часть */
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	glCullFace(GL_BACK);
	sv_mesh_render_shadow_volume(mesh->sv,ilight);
	/* задняя часть */
	glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
	glCullFace(GL_FRONT);
	sv_mesh_render_shadow_volume(mesh->sv,ilight);
	glCullFace(GL_BACK);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glDisable(GL_STENCIL_TEST);
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glPopMatrix();
}

/* рендерим тень
 */
void sv_mesh_render_shadow(float *color) {
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL,0,~0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO,GL_SRC_ALPHA);
	glColor4fv(color);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(-1,-1);
	glVertex2f(1,-1);
	glVertex2f(-1,1);
	glVertex2f(1,1);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
