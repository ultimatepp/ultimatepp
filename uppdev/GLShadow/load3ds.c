/*	load 3ds file
 *
 *			written by Alexander Zaprjagaev
 *			frustum@public.tsu.ru
 *
 *	usage:
 *	float *load_3ds(char *name,int *num_vertex);
 *	1,2,3 - xyz
 *	4,5,6 - normal
 *	7,8 - st
 */

#include <math.h>
#include <stdio.h>
#include <malloc.h>

#define CHUNK_MAIN		0x4d4d
#define CHUNK_OBJMESH	0x3d3d
#define CHUNK_OBJBLOCK	0x4000
#define CHUNK_TRIMESH	0x4100
#define CHUNK_VERTLIST	0x4110
#define CHUNK_FACELIST	0x4120
#define CHUNK_MAPLIST	0x4140
#define CHUNK_SMOOLIST	0x4150

typedef struct {
	float x,y,z;
} vector_t;

typedef struct {
	float u,v;
} uvmap_t;

typedef struct {
	vector_t *vertex;
	int num_vertex;
	uvmap_t *uvmap;
	int num_uvmap;
	int *face;
	int *smoothgroup;
	vector_t *normal;
	int num_face;
} trimesh_t;

typedef struct {
	trimesh_t *trimesh;
	int num_trimesh;
} mesh_t;

typedef int (*process_chunk)(FILE *file,
		unsigned short type,int size,void *data);

/*
 */
static void vector_copy(vector_t *a,vector_t *b) {
	b->x = a->x;
	b->y = a->y;
	b->z = a->z;
}

static void vector_add(vector_t *a,vector_t *b,vector_t *c) {
	c->x = a->x + b->x;
	c->y = a->y + b->y;
	c->z = a->z + b->z;
}

static void vector_sub(vector_t *a,vector_t *b,vector_t *c) {
	c->x = a->x - b->x;
	c->y = a->y - b->y;
	c->z = a->z - b->z;
}

static void vector_cross(vector_t *a,vector_t *b,vector_t *c) {
	c->x = a->y * b->z - a->z * b->y;
	c->y = a->z * b->x - a->x * b->z;
	c->z = a->x * b->y - a->y * b->x;
}

static float vector_normalize(vector_t *a,vector_t *b) {
	float length = sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
	if(length) {
		float ilength = 1.0 / length;
		b->x = a->x * ilength;
		b->y = a->y * ilength;
		b->z = a->z * ilength;
		return length;
	}
	b->x = 0.0;
	b->y = 0.0;
	b->z = 0.0;
	return 0.0;
}

/*
 */
static void skeep_bytes(FILE *file,int bytes) {
	fseek(file,bytes,SEEK_CUR);
}

static int skeep_string(FILE *file) {
	int i = 0;
	while(fgetc(file) != '\0') i++;
	return ++i;
}

static unsigned short read_ushort(FILE *file) {
	unsigned short ret;
	fread(&ret,1,sizeof(unsigned short),file);
	return ret;
}

static int read_int(FILE *file) {
	int ret;
	fread(&ret,1,sizeof(int),file);
	return ret;
}

static float read_float(FILE *file) {
	float ret;
	fread(&ret,1,sizeof(float),file);
	return ret;
}

static int read_chunk(FILE *file,process_chunk chunk_func,void *data) {
	unsigned short chunk_type;
	int chunk_size,content_size;
	chunk_type = read_ushort(file);
	chunk_size = read_int(file);
	content_size = chunk_size - 6;
	if(!chunk_func(file,chunk_type,content_size,data))
		skeep_bytes(file,content_size);
	return chunk_size;
}

static int read_chunks(FILE *file,int bytes,
	process_chunk chunk_func,void *data) {
	int bytes_read = 0;
	while(bytes_read < bytes)
		bytes_read += read_chunk(file,chunk_func,data);
	if(bytes_read != bytes)
		fprintf(stderr,"expected %d bytes but read %d\n",bytes_read,bytes);
	return bytes_read;
}

/*
 */
static int process_smoolist(FILE *file,unsigned short type,
	int size,void *data) {
	int i;
	trimesh_t *trimesh = (trimesh_t*)data;
	if(type == CHUNK_SMOOLIST) {
		trimesh->smoothgroup = malloc(sizeof(int) * trimesh->num_face);
		for(i = 0; i < trimesh->num_face; i++) {
			trimesh->smoothgroup[i] = read_int(file);
		}
		return 1;
	}
	return 0;
}

static int process_trimesh(FILE *file,unsigned short type,
	int size,void *data) {
	int i;
	trimesh_t *trimesh = (trimesh_t*)data;
	if(type == CHUNK_VERTLIST) { /* vertlist */
		trimesh->num_vertex = read_ushort(file);
		trimesh->vertex = malloc(sizeof(vector_t) * trimesh->num_vertex);
		for(i = 0; i < trimesh->num_vertex; i++) {
			trimesh->vertex[i].x = read_float(file);
			trimesh->vertex[i].y = read_float(file);
			trimesh->vertex[i].z = read_float(file);
		}
		return 1;
	} else if(type == CHUNK_MAPLIST) { /* maplist */
		trimesh->num_uvmap = read_ushort(file);
		trimesh->uvmap = malloc(sizeof(uvmap_t) * trimesh->num_uvmap);
		for(i = 0; i < trimesh->num_uvmap; i++) {
			trimesh->uvmap[i].u = read_float(file);
			trimesh->uvmap[i].v = 1.0 - read_float(file);
		}
		return 1;
	} else if(type == CHUNK_FACELIST) { /* facelist */
		int bytes_left;
		trimesh->num_face = read_ushort(file);
		trimesh->face = malloc(sizeof(int) * trimesh->num_face * 3);
		for(i = 0; i < trimesh->num_face * 3; i += 3) {
			trimesh->face[i + 0] = read_ushort(file);
			trimesh->face[i + 1] = read_ushort(file);
			trimesh->face[i + 2] = read_ushort(file);
			read_ushort(file);
		}
		bytes_left = size - trimesh->num_face * sizeof(unsigned short) * 4 - 2;
		if(bytes_left > 0)
			read_chunks(file,bytes_left,process_smoolist,(void*)trimesh);
		return 1;
	}
	return 0;
}

static int process_objblock(FILE *file,unsigned short type,
	int size,void *data) {
	mesh_t *mesh = (mesh_t*)data;
	if(type == CHUNK_TRIMESH) {
		mesh->num_trimesh++;
		if(mesh->trimesh == NULL) mesh->trimesh = malloc(sizeof(trimesh_t));
		else mesh->trimesh = realloc(mesh->trimesh,
			sizeof(trimesh_t) * mesh->num_trimesh);
		memset(&mesh->trimesh[mesh->num_trimesh - 1],0,sizeof(trimesh_t));
		read_chunks(file,size,process_trimesh,
			(void*)&mesh->trimesh[mesh->num_trimesh - 1]);
		return 1;
	}
	return 0;
}

static int process_objmesh(FILE *file,unsigned short type,
	int size,void *data) {
	if(type == CHUNK_OBJBLOCK) {
		size -= skeep_string(file);
		read_chunks(file,size,process_objblock,data);
		return 1;
	}
	return 0;
}

static int process_main(FILE *file,unsigned short type,
	int size,void *data) {
	if(type == CHUNK_OBJMESH) {
		read_chunks(file,size,process_objmesh,data);
		return 1;
	}
	return 0;
}


/*
 */
static void create_trimesh(trimesh_t *trimesh) {
	int i;
	uvmap_t *uvmap;
	vector_t *vertex,*normal_face,*normal_vertex;
	int *face,*vertex_count,**vertex_face,*smoothgroup;
	/* calc normals */
	normal_face = calloc(1,sizeof(vector_t) * trimesh->num_face);
	normal_vertex = calloc(1,sizeof(vector_t) * trimesh->num_face * 3);
	vertex_count = calloc(1,sizeof(int) * trimesh->num_face * 3);
	vertex_face = calloc(1,sizeof(int*) * trimesh->num_face * 3);
	trimesh->normal = normal_vertex;
	vertex = trimesh->vertex;
	face = trimesh->face;
	smoothgroup = trimesh->smoothgroup;
	for(i = 0; i < trimesh->num_face; i++) {
		int j = i * 3;
		vector_t a,b;
		int v0 = face[j + 0];
		int v1 = face[j + 1];
		int v2 = face[j + 2];
		vertex_count[v0]++;
		vertex_count[v1]++;
		vertex_count[v2]++;
		vector_sub(&vertex[v1],&vertex[v0],&a);
		vector_sub(&vertex[v2],&vertex[v0],&b);
		vector_cross(&a,&b,&normal_face[i]);
		vector_normalize(&normal_face[i],&normal_face[i]);
	}
	for(i = 0; i < trimesh->num_face * 3; i++) {
		vertex_face[i] = malloc(sizeof(int) * (vertex_count[i] + 1));
		vertex_face[i][0] = vertex_count[i];
	}
	for(i = 0; i < trimesh->num_face; i++) {
		int j = i * 3;
		int v0 = face[j + 0];
		int v1 = face[j + 1];
		int v2 = face[j + 2];
		vertex_face[v0][vertex_count[v0]--] = i;
		vertex_face[v1][vertex_count[v1]--] = i;
		vertex_face[v2][vertex_count[v2]--] = i;
	}
	for(i = 0; i < trimesh->num_face; i++) {
		int j = i * 3,k;
		int v0 = face[j + 0];
		int v1 = face[j + 1];
		int v2 = face[j + 2];
		for(k = 1; k <= vertex_face[v0][0]; k++) {
			int l = vertex_face[v0][k];
			if(i == l || (smoothgroup && smoothgroup[i] & smoothgroup[l]))
				vector_add(&normal_vertex[j + 0],&normal_face[l],
					&normal_vertex[j + 0]);
		}
		for(k = 1; k <= vertex_face[v1][0]; k++) {
			int l = vertex_face[v1][k];
			if(i == l || (smoothgroup && smoothgroup[i] & smoothgroup[l]))
				vector_add(&normal_vertex[j + 1],&normal_face[l],
					&normal_vertex[j + 1]);
		}
		for(k = 1; k <= vertex_face[v2][0]; k++) {
			int l = vertex_face[v2][k];
			if(i == l || (smoothgroup && smoothgroup[i] & smoothgroup[l]))
				vector_add(&normal_vertex[j + 2],&normal_face[l],
					&normal_vertex[j + 2]);
		}
	}
	for(i = 0; i < trimesh->num_face * 3; i++) {
		vector_normalize(&normal_vertex[i],&normal_vertex[i]);
		free(vertex_face[i]);
	}
	free(normal_face);
	free(vertex_count);
	free(vertex_face);
	/* create linear arrays for vertex and uvmap */
	vertex = calloc(1,sizeof(vector_t) * trimesh->num_face * 3);
	uvmap = calloc(1,sizeof(uvmap_t) * trimesh->num_face * 3);
	for(i = 0; i < trimesh->num_face * 3; i += 3) {
		vector_copy(&trimesh->vertex[trimesh->face[i + 0]],&vertex[i + 0]);
		vector_copy(&trimesh->vertex[trimesh->face[i + 1]],&vertex[i + 1]);
		vector_copy(&trimesh->vertex[trimesh->face[i + 2]],&vertex[i + 2]);
	}
	free(trimesh->vertex);
	trimesh->vertex = vertex;
	trimesh->num_vertex = trimesh->num_face * 3;
	if(trimesh->uvmap) {
		for(i = 0; i < trimesh->num_face * 3; i += 3) {
			uvmap[i + 0].u = trimesh->uvmap[trimesh->face[i + 0]].u;
			uvmap[i + 0].v = trimesh->uvmap[trimesh->face[i + 0]].v;
			uvmap[i + 1].u = trimesh->uvmap[trimesh->face[i + 1]].u;
			uvmap[i + 1].v = trimesh->uvmap[trimesh->face[i + 1]].v;
			uvmap[i + 2].u = trimesh->uvmap[trimesh->face[i + 2]].u;
			uvmap[i + 2].v = trimesh->uvmap[trimesh->face[i + 2]].v;
		}
		free(trimesh->uvmap);
	}
	trimesh->uvmap = uvmap;
	trimesh->num_uvmap = trimesh->num_face * 3;
	free(trimesh->face);
	trimesh->face = NULL;
}

static float *create_mesh(mesh_t *mesh,int *num_vertex) {
	int i,j;
	float *vertex;
	for(i = 0, *num_vertex = 0; i < mesh->num_trimesh; i++) {
		create_trimesh(&mesh->trimesh[i]);
		*num_vertex += mesh->trimesh[i].num_face;
	}
	*num_vertex *= 3;
	vertex = malloc(sizeof(float) * *num_vertex * 8);
	for(i = 0, j = 0; i < mesh->num_trimesh; i++) {
		int k;
		for(k = 0; k < mesh->trimesh[i].num_face * 3; k++, j += 8) {
			vertex[j + 0] = mesh->trimesh[i].vertex[k].x;
			vertex[j + 1] = mesh->trimesh[i].vertex[k].y;
			vertex[j + 2] = mesh->trimesh[i].vertex[k].z;
			vertex[j + 3] = mesh->trimesh[i].normal[k].x;
			vertex[j + 4] = mesh->trimesh[i].normal[k].y;
			vertex[j + 5] = mesh->trimesh[i].normal[k].z;
			vertex[j + 6] = mesh->trimesh[i].uvmap[k].u;
			vertex[j + 7] = mesh->trimesh[i].uvmap[k].v;
		}
		if(mesh->trimesh[i].vertex) free(mesh->trimesh[i].vertex);
		if(mesh->trimesh[i].normal) free(mesh->trimesh[i].normal);
		if(mesh->trimesh[i].uvmap) free(mesh->trimesh[i].uvmap);
		if(mesh->trimesh[i].face) free(mesh->trimesh[i].face);
		if(mesh->trimesh[i].smoothgroup) free(mesh->trimesh[i].smoothgroup);
	}
	free(mesh->trimesh);
	return vertex;
}

/*
 */
float *load_3ds(char *name,int *num_vertex) {
	FILE *file;
	mesh_t *mesh;
	float *vertex;
	unsigned short chunk_type;
	int chunk_size,content_size;
	file = fopen(name,"rb");
	if(!file) {
		fprintf(stderr,"error open %s file\n",name);
		return NULL;
	}
	chunk_type = read_ushort(file);
	chunk_size = read_int(file);
	content_size = chunk_size - 6;
	if(chunk_type != CHUNK_MAIN) {
		fprintf(stderr,"wrong main chunk\n");
		fclose(file);
		return NULL;
	}
	mesh = calloc(1,sizeof(mesh_t));
	read_chunks(file,content_size,process_main,(void*)mesh);
	fclose(file);
	vertex = create_mesh(mesh,num_vertex);
	free(mesh);
	return vertex;
}
