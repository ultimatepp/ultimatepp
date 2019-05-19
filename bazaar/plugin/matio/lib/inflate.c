/** @file inflate.c
 * @brief Functions to inflate data/tags
 * @ingroup MAT
 */
/*
 * Copyright (c) 2005-2019, Christopher C. Hulbert
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
#include "matio_private.h"

#if HAVE_ZLIB

/** @cond mat_devman */

/** @brief Inflate the data until @c nbytes of uncompressed data has been
 *         inflated
 *
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param z zlib compression stream
 * @param nbytes Number of uncompressed bytes to skip
 * @return Number of bytes read from the file
 */
size_t
InflateSkip(mat_t *mat, z_streamp z, int nbytes)
{
    mat_uint8_t comp_buf[512],uncomp_buf[512];
    int    n, err, cnt = 0;
    size_t bytesread = 0;

    if ( nbytes < 1 )
        return 0;

    n = (nbytes<512) ? nbytes : 512;
    if ( !z->avail_in ) {
        z->next_in = comp_buf;
        z->avail_in += fread(comp_buf,1,n,(FILE*)mat->fp);
        bytesread   += z->avail_in;
    }
    z->avail_out = n;
    z->next_out  = uncomp_buf;
    err = inflate(z,Z_FULL_FLUSH);
    if ( err == Z_STREAM_END ) {
        return bytesread;
    } else if ( err != Z_OK ) {
        Mat_Critical("InflateSkip: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    if ( !z->avail_out ) {
        cnt += n;
        n = ((nbytes-cnt)<512) ? nbytes-cnt : 512;
        z->avail_out = n;
        z->next_out  = uncomp_buf;
    }
    while ( cnt < nbytes ) {
        if ( !z->avail_in ) {
            z->next_in   = comp_buf;
            z->avail_in += fread(comp_buf,1,n,(FILE*)mat->fp);
            bytesread   += z->avail_in;
        }
        err = inflate(z,Z_FULL_FLUSH);
        if ( err == Z_STREAM_END ) {
            break;
        } else if ( err != Z_OK ) {
            Mat_Critical("InflateSkip: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            break;
        }
        if ( !z->avail_out ) {
            cnt         += n;
            n            = ((nbytes-cnt)<512) ? nbytes-cnt : 512;
            z->avail_out = n;
            z->next_out  = uncomp_buf;
        }
    }

    if ( z->avail_in ) {
        long offset = -(long)z->avail_in;
        (void)fseek((FILE*)mat->fp,offset,SEEK_CUR);
        bytesread -= z->avail_in;
        z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflate the data until @c nbytes of compressed data has been
 *         inflated
 *
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param z zlib compression stream
 * @param nbytes Number of uncompressed bytes to skip
 * @return Number of bytes read from the file
 */
size_t
InflateSkip2(mat_t *mat, matvar_t *matvar, int nbytes)
{
    mat_uint8_t comp_buf[32],uncomp_buf[32];
    int    err, cnt = 0;
    size_t bytesread = 0;

    if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    matvar->internal->z->avail_out = 1;
    matvar->internal->z->next_out = uncomp_buf;
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateSkip2: %s - inflate returned %s",matvar->name,zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    if ( !matvar->internal->z->avail_out ) {
        matvar->internal->z->avail_out = 1;
        matvar->internal->z->next_out = uncomp_buf;
    }
    while ( cnt < nbytes ) {
        if ( !matvar->internal->z->avail_in ) {
            matvar->internal->z->avail_in = 1;
            matvar->internal->z->next_in = comp_buf;
            bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
            cnt++;
        }
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateSkip2: %s - inflate returned %s",matvar->name,zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
        if ( !matvar->internal->z->avail_out ) {
            matvar->internal->z->avail_out = 1;
            matvar->internal->z->next_out = uncomp_buf;
        }
    }

    if ( matvar->internal->z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)matvar->internal->z->avail_in,SEEK_CUR);
        bytesread -= matvar->internal->z->avail_in;
        matvar->internal->z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflate the data until @c len elements of compressed data with data
 *         type @c data_type has been inflated
 *
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param z zlib compression stream
 * @param data_type Data type (matio_types enumerations)
 * @param len Number of elements of datatype @c data_type to skip
 * @return Number of bytes read from the file
 */
size_t
InflateSkipData(mat_t *mat,z_streamp z,enum matio_types data_type,int len)
{
    int data_size = 0;

    if ( (mat == NULL) || (z == NULL) )
        return 0;
    else if ( len < 1 )
        return 0;

    switch ( data_type ) {
        case MAT_T_DOUBLE:
            data_size = sizeof(double);
            break;
        case MAT_T_SINGLE:
            data_size = sizeof(float);
            break;
#ifdef HAVE_MAT_INT64_T
        case MAT_T_INT64:
            data_size = sizeof(mat_int64_t);
            break;
#endif /* HAVE_MAT_INT64_T */
#ifdef HAVE_MAT_UINT64_T
        case MAT_T_UINT64:
            data_size = sizeof(mat_uint64_t);
            break;
#endif /* HAVE_MAT_UINT64_T */
        case MAT_T_INT32:
            data_size = sizeof(mat_int32_t);
            break;
        case MAT_T_UINT32:
            data_size = sizeof(mat_uint32_t);
            break;
        case MAT_T_INT16:
            data_size = sizeof(mat_int16_t);
            break;
        case MAT_T_UINT16:
            data_size = sizeof(mat_uint16_t);
            break;
        case MAT_T_UINT8:
            data_size = sizeof(mat_uint8_t);
            break;
        case MAT_T_INT8:
            data_size = sizeof(mat_int8_t);
            break;
        default:
            return 0;
    }
    InflateSkip(mat,z,len*data_size);
    return len;
}

/** @brief Inflates the variable's tag.
 *
 * @c buf must hold at least 8 bytes
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param matvar Pointer to the MAT variable
 * @param buf Pointer to store the 8-byte variable tag
 * @return Number of bytes read from the file
 */
size_t
InflateVarTag(mat_t *mat, matvar_t *matvar, void *buf)
{
    mat_uint8_t comp_buf[32];
    int    err;
    size_t bytesread = 0, readresult = 1;

    if ( buf == NULL )
        return 0;

    if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    matvar->internal->z->avail_out = 8;
    matvar->internal->z->next_out = (Bytef*)buf;
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateVarTag: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    while ( matvar->internal->z->avail_out && !matvar->internal->z->avail_in && 1 == readresult ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateVarTag: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }

    if ( matvar->internal->z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)matvar->internal->z->avail_in,SEEK_CUR);
        bytesread -= matvar->internal->z->avail_in;
        matvar->internal->z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflates the Array Flags Tag and the Array Flags data.
 *
 * @c buf must hold at least 16 bytes
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param matvar Pointer to the MAT variable
 * @param buf Pointer to store the 16-byte array flags tag and data
 * @return Number of bytes read from the file
 */
size_t
InflateArrayFlags(mat_t *mat, matvar_t *matvar, void *buf)
{
    mat_uint8_t comp_buf[32];
    int    err;
    size_t bytesread = 0, readresult = 1;

    if ( buf == NULL )
        return 0;

    if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    matvar->internal->z->avail_out = 16;
    matvar->internal->z->next_out = (Bytef*)buf;
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateArrayFlags: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    while ( matvar->internal->z->avail_out && !matvar->internal->z->avail_in && 1 == readresult ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateArrayFlags: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }

    if ( matvar->internal->z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)matvar->internal->z->avail_in,SEEK_CUR);
        bytesread -= matvar->internal->z->avail_in;
        matvar->internal->z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflates the dimensions tag and the dimensions data
 *
 * @c buf must hold at least (8+4*rank) bytes where rank is the number of
 * dimensions. If the end of the dimensions data is not aligned on an 8-byte
 * boundary, this function eats up those bytes and stores then in @c buf.
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param matvar Pointer to the MAT variable
 * @param buf Pointer to store the dimensions flag and data
 * @param nbytes Size of buf in bytes
 * @param dims Output buffer to be allocated if (8+4*rank) > nbytes
 * @return Number of bytes read from the file
 */
size_t
InflateRankDims(mat_t *mat, matvar_t *matvar, void *buf, size_t nbytes, mat_uint32_t** dims)
{
    mat_uint8_t comp_buf[32];
    mat_int32_t tag[2];
    int    err, rank, i;
    size_t bytesread = 0, readresult = 1;

    if ( buf == NULL )
        return 0;

    if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    matvar->internal->z->avail_out = 8;
    matvar->internal->z->next_out = (Bytef*)buf;
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateRankDims: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    while ( matvar->internal->z->avail_out && !matvar->internal->z->avail_in && 1 == readresult ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateRankDims: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }
    tag[0] = *(int *)buf;
    tag[1] = *((int *)buf+1);
    if ( mat->byteswap ) {
        Mat_int32Swap(tag);
        Mat_int32Swap(tag+1);
    }
    if ( (tag[0] & 0x0000ffff) != MAT_T_INT32 ) {
        Mat_Critical("InflateRankDims: Reading dimensions expected type MAT_T_INT32");
        return bytesread;
    }
    rank = tag[1];
    if ( rank % 8 != 0 )
        i = 8-(rank %8);
    else
        i = 0;
    rank+=i;

    if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }

    matvar->internal->z->avail_out = rank;
    if ( sizeof(mat_uint32_t)*(rank + 2) <= nbytes ) {
        matvar->internal->z->next_out = (Bytef*)((mat_int32_t *)buf+2);
    } else {
        /* Cannot use too small buf, but can allocate output buffer dims */
        *dims = (mat_uint32_t*)calloc(rank, sizeof(mat_uint32_t));
        if ( NULL != *dims ) {
            matvar->internal->z->next_out = (Bytef*)*dims;
        } else {
            *((mat_int32_t *)buf+1) = 0;
            Mat_Critical("Error allocating memory for dims");
            return bytesread;
        }
    }
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateRankDims: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    readresult = 1;
    while ( matvar->internal->z->avail_out && !matvar->internal->z->avail_in && 1 == readresult ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateRankDims: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }

    if ( matvar->internal->z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)matvar->internal->z->avail_in,SEEK_CUR);
        bytesread -= matvar->internal->z->avail_in;
        matvar->internal->z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflates the variable name tag
 *
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param matvar Pointer to the MAT variable
 * @param buf Pointer to store the variables name tag
 * @return Number of bytes read from the file
 */
size_t
InflateVarNameTag(mat_t *mat, matvar_t *matvar, void *buf)
{
    mat_uint8_t comp_buf[32];
    int    err;
    size_t bytesread = 0, readresult = 1;

    if ( buf == NULL )
        return 0;

    if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    matvar->internal->z->avail_out = 8;
    matvar->internal->z->next_out = (Bytef*)buf;
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateVarNameTag: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    while ( matvar->internal->z->avail_out && !matvar->internal->z->avail_in && 1 == readresult ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateVarNameTag: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }

    if ( matvar->internal->z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)matvar->internal->z->avail_in,SEEK_CUR);
        bytesread -= matvar->internal->z->avail_in;
        matvar->internal->z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflates the variable name
 *
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param matvar Pointer to the MAT variable
 * @param buf Pointer to store the variables name
 * @param N Number of characters in the name
 * @return Number of bytes read from the file
 */
size_t
InflateVarName(mat_t *mat, matvar_t *matvar, void *buf, int N)
{
    mat_uint8_t comp_buf[32];
    int    err;
    size_t bytesread = 0, readresult = 1;

    if ( buf == NULL )
        return 0;

    if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    matvar->internal->z->avail_out = N;
    matvar->internal->z->next_out = (Bytef*)buf;
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateVarName: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    while ( matvar->internal->z->avail_out && !matvar->internal->z->avail_in && 1 == readresult ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateVarName: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }

    if ( matvar->internal->z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)matvar->internal->z->avail_in,SEEK_CUR);
        bytesread -= matvar->internal->z->avail_in;
        matvar->internal->z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflates the data's tag
 *
 * buf must hold at least 8 bytes
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param matvar Pointer to the MAT variable
 * @param buf Pointer to store the data tag
 * @return Number of bytes read from the file
 */
size_t
InflateDataTag(mat_t *mat, matvar_t *matvar, void *buf)
{
    mat_uint8_t comp_buf[32];
    int    err;
    size_t bytesread = 0, readresult = 1;

    if ( buf == NULL )
        return 0;

   if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    matvar->internal->z->avail_out = 8;
    matvar->internal->z->next_out = (Bytef*)buf;
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err == Z_STREAM_END ) {
        return bytesread;
    } else if ( err != Z_OK ) {
        Mat_Critical("InflateDataTag: %s - inflate returned %s",matvar->name,zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    while ( matvar->internal->z->avail_out && !matvar->internal->z->avail_in && 1 == readresult ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err == Z_STREAM_END ) {
            break;
        } else if ( err != Z_OK ) {
            Mat_Critical("InflateDataTag: %s - inflate returned %s",matvar->name,zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }

    if ( matvar->internal->z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)matvar->internal->z->avail_in,SEEK_CUR);
        bytesread -= matvar->internal->z->avail_in;
        matvar->internal->z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflates the data's type
 *
 * buf must hold at least 4 bytes
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param matvar Pointer to the MAT variable
 * @param buf Pointer to store the data type
 * @return Number of bytes read from the file
 */
size_t
InflateDataType(mat_t *mat, z_streamp z, void *buf)
{
    mat_uint8_t comp_buf[32];
    int    err;
    size_t bytesread = 0, readresult = 1;

    if ( buf == NULL )
        return 0;

    if ( !z->avail_in ) {
        z->avail_in = 1;
        z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    z->avail_out = 4;
    z->next_out = (Bytef*)buf;
    err = inflate(z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateDataType: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    while ( z->avail_out && !z->avail_in && 1 == readresult ) {
        z->avail_in = 1;
        z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateDataType: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }

    if ( z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)z->avail_in,SEEK_CUR);
        bytesread -= z->avail_in;
        z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflates the data
 *
 * buf must hold at least @c nBytes bytes
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param z zlib compression stream
 * @param buf Pointer to store the data type
 * @param nBytes Number of bytes to inflate
 * @return Number of bytes read from the file
 */
size_t
InflateData(mat_t *mat, z_streamp z, void *buf, unsigned int nBytes)
{
    mat_uint8_t comp_buf[1024];
    int    err;
    unsigned int bytesread = 0;

    if ( buf == NULL )
        return 0;
    if ( nBytes == 0 ) {
        return bytesread;
    }

    if ( !z->avail_in ) {
        if ( nBytes > 1024 ) {
            z->avail_in = fread(comp_buf,1,1024,(FILE*)mat->fp);
        } else {
            z->avail_in = fread(comp_buf,1,nBytes,(FILE*)mat->fp);
        }
        bytesread += z->avail_in;
        z->next_in = comp_buf;
    }
    z->avail_out = nBytes;
    z->next_out = (Bytef*)buf;
    err = inflate(z,Z_FULL_FLUSH);
    if ( err == Z_STREAM_END ) {
        return bytesread;
    } else if ( err != Z_OK ) {
        Mat_Critical("InflateData: inflate returned %s",zError( err == Z_NEED_DICT ? Z_DATA_ERROR : err ));
        return bytesread;
    }
    while ( z->avail_out && !z->avail_in ) {
        if ( nBytes > 1024 + bytesread ) {
            z->avail_in = fread(comp_buf,1,1024,(FILE*)mat->fp);
        } else if ( nBytes < 1 + bytesread ) { /* Read a byte at a time */
            z->avail_in = fread(comp_buf,1,1,(FILE*)mat->fp);
        } else {
            z->avail_in = fread(comp_buf,1,nBytes-bytesread,(FILE*)mat->fp);
        }
        bytesread += z->avail_in;
        z->next_in = comp_buf;
        err = inflate(z,Z_FULL_FLUSH);
        if ( err == Z_STREAM_END ) {
            break;
        } else if ( err != Z_OK && err != Z_BUF_ERROR ) {
            Mat_Critical("InflateData: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            break;
        }
    }

    if ( z->avail_in ) {
        long offset = -(long)z->avail_in;
        (void)fseek((FILE*)mat->fp,offset,SEEK_CUR);
        bytesread -= z->avail_in;
        z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflates the structure's fieldname length
 *
 * buf must hold at least 8 bytes
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param matvar Pointer to the MAT variable
 * @param buf Pointer to store the fieldname length
 * @return Number of bytes read from the file
 */
size_t
InflateFieldNameLength(mat_t *mat, matvar_t *matvar, void *buf)
{
    mat_uint8_t comp_buf[32];
    int    err;
    size_t bytesread = 0, readresult = 1;

    if ( buf == NULL )
        return 0;

    if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    matvar->internal->z->avail_out = 8;
    matvar->internal->z->next_out = (Bytef*)buf;
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateFieldNameLength: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    while ( matvar->internal->z->avail_out && !matvar->internal->z->avail_in && 1 == readresult ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateFieldNameLength: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }

    if ( matvar->internal->z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)matvar->internal->z->avail_in,SEEK_CUR);
        bytesread -= matvar->internal->z->avail_in;
        matvar->internal->z->avail_in = 0;
    }

    return bytesread;
}

/** @brief Inflates the structure's fieldname tag
 *
 * buf must hold at least 8 bytes
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param matvar Pointer to the MAT variable
 * @param buf Pointer to store the fieldname tag
 * @return Number of bytes read from the file
 */
size_t
InflateFieldNamesTag(mat_t *mat, matvar_t *matvar, void *buf)
{
    mat_uint8_t comp_buf[32];
    int    err;
    size_t bytesread = 0, readresult = 1;

    if ( buf == NULL )
        return 0;

    if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    matvar->internal->z->avail_out = 8;
    matvar->internal->z->next_out = (Bytef*)buf;
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateFieldNamesTag: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    while ( matvar->internal->z->avail_out && !matvar->internal->z->avail_in && 1 == readresult ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateFieldNamesTag: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }

    if ( matvar->internal->z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)matvar->internal->z->avail_in,SEEK_CUR);
        bytesread -= matvar->internal->z->avail_in;
        matvar->internal->z->avail_in = 0;
    }

    return bytesread;
}

/*
 * Inflates the structure's fieldname length.  buf must hold at least
 * nfields*fieldname_length bytes
 */
/** @brief Inflates the structure's fieldnames
 *
 * buf must hold at least @c nfields * @c fieldname_length bytes
 * @ingroup mat_internal
 * @param mat Pointer to the MAT file
 * @param matvar Pointer to the MAT variable
 * @param buf Pointer to store the fieldnames
 * @param nfields Number of fields
 * @param fieldname_length Maximum length in bytes of each field
 * @param padding Number of padding bytes
 * @return Number of bytes read from the file
 */
size_t
InflateFieldNames(mat_t *mat,matvar_t *matvar,void *buf,int nfields,
                  int fieldname_length,int padding)
{
    mat_uint8_t comp_buf[32];
    int    err;
    size_t bytesread = 0, readresult = 1;

    if ( buf == NULL )
        return 0;

    if ( !matvar->internal->z->avail_in ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        bytesread += fread(comp_buf,1,1,(FILE*)mat->fp);
    }
    matvar->internal->z->avail_out = nfields*fieldname_length+padding;
    matvar->internal->z->next_out = (Bytef*)buf;
    err = inflate(matvar->internal->z,Z_NO_FLUSH);
    if ( err != Z_OK ) {
        Mat_Critical("InflateFieldNames: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
        return bytesread;
    }
    while ( matvar->internal->z->avail_out && !matvar->internal->z->avail_in && 1 == readresult ) {
        matvar->internal->z->avail_in = 1;
        matvar->internal->z->next_in = comp_buf;
        readresult = fread(comp_buf,1,1,(FILE*)mat->fp);
        bytesread += readresult;
        err = inflate(matvar->internal->z,Z_NO_FLUSH);
        if ( err != Z_OK ) {
            Mat_Critical("InflateFieldNames: inflate returned %s",zError(err == Z_NEED_DICT ? Z_DATA_ERROR : err));
            return bytesread;
        }
    }

    if ( matvar->internal->z->avail_in ) {
        (void)fseek((FILE*)mat->fp,-(int)matvar->internal->z->avail_in,SEEK_CUR);
        bytesread -= matvar->internal->z->avail_in;
        matvar->internal->z->avail_in = 0;
    }

    return bytesread;
}

/** @endcond */

#endif
