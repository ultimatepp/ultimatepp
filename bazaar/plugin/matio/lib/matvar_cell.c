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
#include "matio_private.h"

/** @brief Returns a pointer to the Cell array at a specific index
 *
 * Returns a pointer to the Cell Array Field at the given 1-relative index.
 * MAT file must be a version 5 matlab file.
 * @ingroup MAT
 * @param matvar Pointer to the Cell Array MAT variable
 * @param index linear index of cell to return
 * @return Pointer to the Cell Array Field on success, NULL on error
 */
matvar_t *
Mat_VarGetCell(matvar_t *matvar,int index)
{
    size_t nelems = 1;
    matvar_t *cell = NULL;

    if ( matvar == NULL )
        return NULL;

    SafeMulDims(matvar, &nelems);

    if ( 0 <= index && index < nelems )
        cell = *((matvar_t **)matvar->data + index);

    return cell;
}

/** @brief Indexes a cell array
 *
 * Finds cells of a cell array given a start, stride, and edge for each.
 * dimension.  The cells are placed in a pointer array.  The cells should not
 * be freed, but the array of pointers should be.  If copies are needed,
 * use Mat_VarDuplicate on each cell.
 *
 * Note that this function is limited to structure arrays with a rank less than
 * 10.
 *
 * @ingroup MAT
 * @param matvar Cell Array matlab variable
 * @param start vector of length rank with 0-relative starting coordinates for
 *              each dimension.
 * @param stride vector of length rank with strides for each dimension.
 * @param edge vector of length rank with the number of elements to read in
 *              each dimension.
 * @returns an array of pointers to the cells
 */
matvar_t **
Mat_VarGetCells(matvar_t *matvar,int *start,int *stride,int *edge)
{
    int i, j, N, I;
    size_t idx[10] = {0,}, cnt[10] = {0,}, dimp[10] = {0,};
    matvar_t **cells;

    if ( (matvar == NULL) || (start == NULL) || (stride == NULL) ||
        (edge == NULL) ) {
        return NULL;
    } else if ( matvar->rank > 9 ) {
        return NULL;
    }

    dimp[0] = matvar->dims[0];
    N = edge[0];
    I = start[0];
    idx[0] = start[0];
    for ( i = 1; i < matvar->rank; i++ ) {
        idx[i]  = start[i];
        dimp[i] = dimp[i-1]*matvar->dims[i];
        N *= edge[i];
        I += start[i]*dimp[i-1];
    }
    cells = (matvar_t**)malloc(N*sizeof(matvar_t *));
    for ( i = 0; i < N; i+=edge[0] ) {
        for ( j = 0; j < edge[0]; j++ ) {
            cells[i+j] = *((matvar_t **)matvar->data + I);
            I += stride[0];
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
    }
    return cells;
}

/** @brief Indexes a cell array
 *
 * Finds cells of a cell array given a linear indexed start, stride, and edge.
 * The cells are placed in a pointer array.  The cells themself should not
 * be freed as they are part of the original cell array, but the pointer array
 * should be.  If copies are needed, use Mat_VarDuplicate on each of the cells.
 * MAT file version must be 5.
 * @ingroup MAT
 * @param matvar Cell Array matlab variable
 * @param start starting index
 * @param stride stride
 * @param edge Number of cells to get
 * @returns an array of pointers to the cells
 */
matvar_t **
Mat_VarGetCellsLinear(matvar_t *matvar,int start,int stride,int edge)
{
    matvar_t **cells = NULL;

    if ( matvar != NULL ) {
        int i, I;
        cells = (matvar_t**)malloc(edge*sizeof(matvar_t *));
        I = start;
        for ( i = 0; i < edge; i++ ) {
            cells[i] = *((matvar_t **)matvar->data + I);
            I += stride;
        }
    }
    return cells;
}

/** @brief Sets the element of the cell array at the specific index
 *
 * Sets the element of the cell array at the given 0-relative index to @c cell.
 * @ingroup MAT
 * @param matvar Pointer to the cell array variable
 * @param index 0-relative linear index of the cell to set
 * @param cell Pointer to the cell to set
 * @return Pointer to the previous cell element, or NULL if there was no
*          previous cell element or error.
 */
matvar_t *
Mat_VarSetCell(matvar_t *matvar,int index,matvar_t *cell)
{
    size_t nelems = 1;
    matvar_t **cells, *old_cell = NULL;

    if ( matvar == NULL || matvar->rank < 1 )
        return NULL;

   SafeMulDims(matvar, &nelems);
    cells = (matvar_t**)matvar->data;
    if ( 0 <= index && index < nelems ) {
        old_cell = cells[index];
        cells[index] = cell;
    }

    return old_cell;
}
