/*
 * Copyright (c) 2012-2019, Christopher C. Hulbert
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>
#if defined(_MSC_VER) || defined(__MINGW32__)
#   define strdup _strdup
#endif
#include "matio_private.h"

/** @brief Creates a structure MATLAB variable with the given name and fields
 *
 * @ingroup MAT
 * @param name Name of the structure variable to create
 * @param rank Rank of the variable
 * @param dims array of dimensions of the variable of size rank
 * @param fields Array of @c nfields fieldnames
 * @param nfields Number of fields in the structure
 * @return Pointer to the new structure MATLAB variable on success, NULL on error
 */
matvar_t *
Mat_VarCreateStruct(const char *name,int rank,size_t *dims,const char **fields,
    unsigned nfields)
{
    size_t nelems = 1;
    int j;
    matvar_t *matvar;

    if ( NULL == dims )
        return NULL;

    matvar = Mat_VarCalloc();
    if ( NULL == matvar )
        return NULL;

    matvar->compression = MAT_COMPRESSION_NONE;
    if ( NULL != name )
        matvar->name = strdup(name);
    matvar->rank = rank;
    matvar->dims = (size_t*)malloc(matvar->rank*sizeof(*matvar->dims));
    for ( j = 0; j < matvar->rank; j++ ) {
        matvar->dims[j] = dims[j];
        nelems *= dims[j];
    }
    matvar->class_type = MAT_C_STRUCT;
    matvar->data_type  = MAT_T_STRUCT;

    matvar->data_size = sizeof(matvar_t *);

    if ( nfields ) {
        matvar->internal->num_fields = nfields;
        matvar->internal->fieldnames =
            (char**)malloc(nfields*sizeof(*matvar->internal->fieldnames));
        if ( NULL == matvar->internal->fieldnames ) {
            Mat_VarFree(matvar);
            matvar = NULL;
        } else {
            size_t i;
            for ( i = 0; i < nfields; i++ ) {
                if ( NULL == fields[i] ) {
                    Mat_VarFree(matvar);
                    matvar = NULL;
                    break;
                } else {
                    matvar->internal->fieldnames[i] = strdup(fields[i]);
                }
            }
        }
        if ( NULL != matvar && nelems > 0 ) {
            size_t nelems_x_nfields;
            SafeMul(&nelems_x_nfields, nelems, nfields);
            SafeMul(&matvar->nbytes, nelems_x_nfields, matvar->data_size);
            matvar->data = calloc(nelems_x_nfields, matvar->data_size);
        }
    }

    return matvar;
}

/** @brief Adds a field to a structure
 *
 * Adds the given field to the structure. fields should be an array of matvar_t
 * pointers of the same size as the structure (i.e. 1 field per structure
 * element).
 * @ingroup MAT
 * @param matvar Pointer to the Structure MAT variable
 * @param fieldname Name of field to be added
 * @retval 0 on success
 */
int
Mat_VarAddStructField(matvar_t *matvar,const char *fieldname)
{
    int cnt = 0;
    size_t i, nfields, nelems = 1;
    matvar_t **new_data, **old_data;
    char     **fieldnames;

    if ( matvar == NULL || fieldname == NULL )
        return -1;
    SafeMulDims(matvar, &nelems);
    nfields = matvar->internal->num_fields+1;
    matvar->internal->num_fields = nfields;
    fieldnames = (char**)realloc(matvar->internal->fieldnames,
        nfields*sizeof(*matvar->internal->fieldnames));
    if ( NULL == fieldnames )
        return -1;
    matvar->internal->fieldnames = fieldnames;
    matvar->internal->fieldnames[nfields-1] = strdup(fieldname);

    {
        size_t nelems_x_nfields;
        SafeMul(&nelems_x_nfields, nelems, nfields);
        SafeMul(&matvar->nbytes, nelems_x_nfields, sizeof(*new_data));
    }
    new_data = (matvar_t**)malloc(matvar->nbytes);
    if ( new_data == NULL ) {
        matvar->nbytes = 0;
        return -1;
    }

    old_data = (matvar_t**)matvar->data;
    for ( i = 0; i < nelems; i++ ) {
        size_t f;
        for ( f = 0; f < nfields-1; f++ )
            new_data[cnt++] = old_data[i*(nfields-1)+f];
        new_data[cnt++] = NULL;
    }

    free(matvar->data);
    matvar->data = new_data;

    return 0;
}

/** @brief Returns the number of fields in a structure variable
 *
 * Returns the number of fields in the given structure.
 * @ingroup MAT
 * @param matvar Structure matlab variable
 * @returns Number of fields
 */
unsigned
Mat_VarGetNumberOfFields(matvar_t *matvar)
{
    int nfields;
    if ( matvar == NULL || matvar->class_type != MAT_C_STRUCT   ||
        NULL == matvar->internal ) {
        nfields = 0;
    } else {
        nfields = matvar->internal->num_fields;
    }
    return nfields;
}

/** @brief Returns the fieldnames of a structure variable
 *
 * Returns the fieldnames for the given structure. The returned pointers are
 * internal to the structure and should not be free'd.
 * @ingroup MAT
 * @param matvar Structure matlab variable
 * @returns Array of fieldnames
 */
char * const *
Mat_VarGetStructFieldnames(const matvar_t *matvar)
{
    if ( matvar == NULL || matvar->class_type != MAT_C_STRUCT   ||
        NULL == matvar->internal ) {
        return NULL;
    } else {
        return matvar->internal->fieldnames;
    }
}

/** @brief Finds a field of a structure by the field's index
 *
 * Returns a pointer to the structure field at the given 0-relative index.
 * @ingroup MAT
 * @param matvar Pointer to the Structure MAT variable
 * @param field_index 0-relative index of the field.
 * @param index linear index of the structure array
 * @return Pointer to the structure field on success, NULL on error
 */
matvar_t *
Mat_VarGetStructFieldByIndex(matvar_t *matvar,size_t field_index,size_t index)
{
    matvar_t *field = NULL;
    size_t nelems = 1, nfields;

    if ( matvar == NULL || matvar->class_type != MAT_C_STRUCT ||
        matvar->data_size == 0 )
        return field;

    SafeMulDims(matvar, &nelems);
    nfields = matvar->internal->num_fields;

    if ( nelems > 0 && index >= nelems ) {
        Mat_Critical("Mat_VarGetStructField: structure index out of bounds");
    } else if ( nfields > 0 ) {
        if ( field_index > nfields ) {
            Mat_Critical("Mat_VarGetStructField: field index out of bounds");
        } else {
            field = *((matvar_t **)matvar->data+index*nfields+field_index);
        }
    }

    return field;
}

/** @brief Finds a field of a structure by the field's name
 *
 * Returns a pointer to the structure field at the given 0-relative index.
 * @ingroup MAT
 * @param matvar Pointer to the Structure MAT variable
 * @param field_name Name of the structure field
 * @param index linear index of the structure array
 * @return Pointer to the structure field on success, NULL on error
 */
matvar_t *
Mat_VarGetStructFieldByName(matvar_t *matvar,const char *field_name,
                            size_t index)
{
    int       i, nfields, field_index;
    matvar_t *field = NULL;
    size_t nelems = 1;

    if ( matvar == NULL || matvar->class_type != MAT_C_STRUCT   ||
        matvar->data_size == 0 )
        return field;

    SafeMulDims(matvar, &nelems);
    nfields = matvar->internal->num_fields;
    field_index = -1;
    for ( i = 0; i < nfields; i++ ) {
        if ( !strcmp(matvar->internal->fieldnames[i],field_name) ) {
            field_index = i;
            break;
        }
    }

    if ( index >= nelems ) {
        Mat_Critical("Mat_VarGetStructField: structure index out of bounds");
    } else if ( field_index >= 0 ) {
        field = *((matvar_t **)matvar->data+index*nfields+field_index);
    }

    return field;
}

/** @brief Finds a field of a structure
 *
 * Returns a pointer to the structure field at the given 0-relative index.
 * @ingroup MAT
 * @param matvar Pointer to the Structure MAT variable
 * @param name_or_index Name of the field, or the 1-relative index of the field
 * If the index is used, it should be the address of an integer variable whose
 * value is the index number.
 * @param opt MAT_BY_NAME if the name_or_index is the name or MAT_BY_INDEX if
 *            the index was passed.
 * @param index linear index of the structure to find the field of
 * @return Pointer to the Structure Field on success, NULL on error
 */
matvar_t *
Mat_VarGetStructField(matvar_t *matvar,void *name_or_index,int opt,int index)
{
    int       err = 0, nfields;
    matvar_t *field = NULL;
    size_t nelems = 1;

    SafeMulDims(matvar, &nelems);
    nfields = matvar->internal->num_fields;
    if ( index < 0 || (nelems > 0 && index >= nelems ))
        err = 1;
    else if ( nfields < 1 )
        err = 1;

    if ( !err && (opt == MAT_BY_INDEX) ) {
        size_t field_index = *(int *)name_or_index;
        if ( field_index > 0 )
            field = Mat_VarGetStructFieldByIndex(matvar,field_index-1,index);
    } else if ( !err && (opt == MAT_BY_NAME) ) {
        field = Mat_VarGetStructFieldByName(matvar,(const char*)name_or_index,index);
    }

    return field;
}

/** @brief Indexes a structure
 *
 * Finds structures of a structure array given a start, stride, and edge for
 * each dimension.  The structures are placed in a new structure array.  If
 * copy_fields is non-zero, the indexed structures are copied and should be
 * freed, but if copy_fields is zero, the indexed structures are pointers to
 * the original, but should still be freed. The structures have a flag set
 * so that the structure fields are not freed.
 *
 * Note that this function is limited to structure arrays with a rank less than
 * 10.
 *
 * @ingroup MAT
 * @param matvar Structure matlab variable
 * @param start vector of length rank with 0-relative starting coordinates for
 *              each dimension.
 * @param stride vector of length rank with strides for each dimension.
 * @param edge vector of length rank with the number of elements to read in
 *              each dimension.
 * @param copy_fields 1 to copy the fields, 0 to just set pointers to them.
 * @returns A new structure array with fields indexed from @c matvar.
 */
matvar_t *
Mat_VarGetStructs(matvar_t *matvar,int *start,int *stride,int *edge,
    int copy_fields)
{
    size_t i,N,I,nfields,field,idx[10] = {0,},cnt[10] = {0,},dimp[10] = {0,};
    matvar_t **fields, *struct_slab;
    int j;

    if ( (matvar == NULL) || (start == NULL) || (stride == NULL) ||
         (edge == NULL) ) {
        return NULL;
    } else if ( matvar->rank > 9 ) {
        return NULL;
    } else if ( matvar->class_type != MAT_C_STRUCT ) {
        return NULL;
    }

    struct_slab = Mat_VarDuplicate(matvar,0);
    if ( !copy_fields )
        struct_slab->mem_conserve = 1;

    nfields = matvar->internal->num_fields;

    dimp[0] = matvar->dims[0];
    N = edge[0];
    I = start[0];
    struct_slab->dims[0] = edge[0];
    idx[0] = start[0];
    for ( j = 1; j < matvar->rank; j++ ) {
        idx[j]  = start[j];
        dimp[j] = dimp[j-1]*matvar->dims[j];
        N *= edge[j];
        I += start[j]*dimp[j-1];
        struct_slab->dims[j] = edge[j];
    }
    I *= nfields;
    struct_slab->nbytes = N*nfields*sizeof(matvar_t *);
    struct_slab->data = malloc(struct_slab->nbytes);
    if ( struct_slab->data == NULL ) {
        Mat_VarFree(struct_slab);
        return NULL;
    }
    fields = (matvar_t**)struct_slab->data;
    for ( i = 0; i < N; i+=edge[0] ) {
        for ( j = 0; j < edge[0]; j++ ) {
            for ( field = 0; field < nfields; field++ ) {
                if ( copy_fields )
                    fields[(i+j)*nfields+field] =
                         Mat_VarDuplicate(*((matvar_t **)matvar->data + I),1);
                else
                    fields[(i+j)*nfields+field] =
                        *((matvar_t **)matvar->data + I);
                I++;
            }
            I += (stride[0]-1)*nfields;
        }
        idx[0] = start[0];
        I = idx[0];
        cnt[1]++;
        idx[1] += stride[1];
        for ( j = 1; j < matvar->rank; j++ ) {
            if ( cnt[j] == edge[j] ) {
                cnt[j] = 0;
                idx[j] = start[j];
                if ( j < matvar->rank - 1 ) {
                    cnt[j+1]++;
                    idx[j+1] += stride[j+1];
                }
            }
            I += idx[j]*dimp[j-1];
        }
        I *= nfields;
    }
    return struct_slab;
}

/** @brief Indexes a structure
 *
 * Finds structures of a structure array given a single (linear)start, stride,
 * and edge.  The structures are placed in a new structure array.  If
 * copy_fields is non-zero, the indexed structures are copied and should be
 * freed, but if copy_fields is zero, the indexed structures are pointers to
 * the original, but should still be freed since the mem_conserve flag is set
 * so that the structures are not freed.
 * MAT file version must be 5.
 * @ingroup MAT
 * @param matvar Structure matlab variable
 * @param start starting index (0-relative)
 * @param stride stride (1 reads consecutive elements)
 * @param edge Number of elements to read
 * @param copy_fields 1 to copy the fields, 0 to just set pointers to them.
 * @returns A new structure with fields indexed from matvar
 */
matvar_t *
Mat_VarGetStructsLinear(matvar_t *matvar,int start,int stride,int edge,
    int copy_fields)
{
    matvar_t *struct_slab;

    if ( matvar == NULL || matvar->rank > 10 ) {
       struct_slab = NULL;
    } else {
        int i, I, field, nfields;
        matvar_t **fields;

        struct_slab = Mat_VarDuplicate(matvar,0);
        if ( !copy_fields )
            struct_slab->mem_conserve = 1;

        nfields = matvar->internal->num_fields;

        struct_slab->nbytes = (size_t)edge*nfields*sizeof(matvar_t *);
        struct_slab->data = malloc(struct_slab->nbytes);
        if ( struct_slab->data == NULL ) {
            Mat_VarFree(struct_slab);
            return NULL;
        }
        struct_slab->dims[0] = edge;
        struct_slab->dims[1] = 1;
        fields = (matvar_t**)struct_slab->data;
        I = start*nfields;
        for ( i = 0; i < edge; i++ ) {
            if ( copy_fields ) {
                for ( field = 0; field < nfields; field++ ) {
                    fields[i*nfields+field] =
                        Mat_VarDuplicate(*((matvar_t **)matvar->data+I),1);
                    I++;
                }
            } else {
                for ( field = 0; field < nfields; field++ ) {
                    fields[i*nfields+field] = *((matvar_t **)matvar->data + I);
                    I++;
                }
            }
            I += (stride-1)*nfields;
        }
    }
    return struct_slab;
}

/** @brief Sets the structure field to the given variable
 *
 * Sets the structure field specified by the 0-relative field index
 * @c field_index for the given 0-relative structure index @c index to
 * @c field.
 * @ingroup MAT
 * @param matvar Pointer to the structure MAT variable
 * @param field_index 0-relative index of the field.
 * @param index linear index of the structure array
 * @param field New field variable
 * @return Pointer to the previous field (NULL if no previous field)
 */
matvar_t *
Mat_VarSetStructFieldByIndex(matvar_t *matvar,size_t field_index,size_t index,
    matvar_t *field)
{
    matvar_t *old_field = NULL;
    size_t nelems = 1, nfields;

    if ( matvar == NULL || matvar->class_type != MAT_C_STRUCT ||
        matvar->data == NULL )
        return old_field;

    SafeMulDims(matvar, &nelems);
    nfields = matvar->internal->num_fields;

    if ( index < nelems && field_index < nfields ) {
        matvar_t **fields = (matvar_t**)matvar->data;
        old_field = fields[index*nfields+field_index];
        fields[index*nfields+field_index] = field;
        if ( NULL != field->name ) {
            free(field->name);
        }
        field->name = strdup(matvar->internal->fieldnames[field_index]);
    }

    return old_field;
}

/** @brief Sets the structure field to the given variable
 *
 * Sets the specified structure fieldname at the given 0-relative @c index to
 * @c field.
 * @ingroup MAT
 * @param matvar Pointer to the Structure MAT variable
 * @param field_name Name of the structure field
 * @param index linear index of the structure array
 * @param field New field variable
 * @return Pointer to the previous field (NULL if no previous field)
 */
matvar_t *
Mat_VarSetStructFieldByName(matvar_t *matvar,const char *field_name,
    size_t index,matvar_t *field)
{
    int       i, nfields, field_index;
    matvar_t *old_field = NULL;
    size_t nelems = 1;

    if ( matvar == NULL || matvar->class_type != MAT_C_STRUCT ||
         matvar->data == NULL )
        return old_field;

    SafeMulDims(matvar, &nelems);
    nfields = matvar->internal->num_fields;
    field_index = -1;
    for ( i = 0; i < nfields; i++ ) {
        if ( !strcmp(matvar->internal->fieldnames[i],field_name) ) {
            field_index = i;
            break;
        }
    }

    if ( index < nelems && field_index >= 0 ) {
        matvar_t **fields = (matvar_t**)matvar->data;
        old_field = fields[index*nfields+field_index];
        fields[index*nfields+field_index] = field;
        if ( NULL != field->name ) {
            free(field->name);
        }
        field->name = strdup(matvar->internal->fieldnames[field_index]);
    }

    return old_field;
}
