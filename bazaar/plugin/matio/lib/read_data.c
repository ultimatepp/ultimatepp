/** @file read_data.c
 * Matlab MAT version 5 file functions
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

/* FIXME: Implement Unicode support */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "matio_private.h"
#if defined(HAVE_ZLIB)
#   include <plugin/z/lib/zlib.h>
#endif

#if !defined(READ_BLOCK_SIZE)
#define READ_BLOCK_SIZE (256)
#endif

#define READ_DATA_NOSWAP(T) \
    do { \
        if ( len <= READ_BLOCK_SIZE ) { \
            bytesread += fread(v,data_size,len,(FILE*)mat->fp); \
            for ( j = 0; j < len; j++ ) { \
                data[j] = (T)v[j]; \
            } \
        } else { \
            for ( i = 0; i < len-READ_BLOCK_SIZE; i+=READ_BLOCK_SIZE ) { \
                bytesread += fread(v,data_size,READ_BLOCK_SIZE,(FILE*)mat->fp); \
                for ( j = 0; j < READ_BLOCK_SIZE; j++ ) { \
                    data[i+j] = (T)v[j]; \
                } \
            } \
            if ( len > i ) { \
                bytesread += fread(v,data_size,len-i,(FILE*)mat->fp); \
                for ( j = 0; j < len-i; j++ ) { \
                    data[i+j] = (T)v[j]; \
                } \
            }\
        } \
    } while (0)

#define READ_DATA(T, SwapFunc) \
    do { \
        if ( mat->byteswap ) { \
            if ( len <= READ_BLOCK_SIZE ) { \
                bytesread += fread(v,data_size,len,(FILE*)mat->fp); \
                for ( j = 0; j < len; j++ ) { \
                    data[j] = (T)SwapFunc(&v[j]); \
                } \
            } else { \
                for ( i = 0; i < len-READ_BLOCK_SIZE; i+=READ_BLOCK_SIZE ) { \
                    bytesread += fread(v,data_size,READ_BLOCK_SIZE,(FILE*)mat->fp); \
                    for ( j = 0; j < READ_BLOCK_SIZE; j++ ) { \
                        data[i+j] = (T)SwapFunc(&v[j]); \
                    } \
                } \
                if ( len > i ) { \
                    bytesread += fread(v,data_size,len-i,(FILE*)mat->fp); \
                    for ( j = 0; j < len-i; j++ ) { \
                        data[i+j] = (T)SwapFunc(&v[j]); \
                    } \
                }\
            } \
        } else { \
            READ_DATA_NOSWAP(T); \
        } \
    } while (0)

#ifdef HAVE_MAT_INT64_T
#define READ_DATA_INT64(T) \
    do { \
        if ( MAT_T_INT64 == data_type ) { \
            mat_int64_t v[READ_BLOCK_SIZE]; \
            READ_DATA(T, Mat_int64Swap); \
        } \
    } while (0)
#else
#define READ_DATA_INT64(T)
#endif /* HAVE_MAT_INT64_T */

#ifdef HAVE_MAT_UINT64_T
#define READ_DATA_UINT64(T) \
    do { \
        if ( MAT_T_UINT64 == data_type ) { \
            mat_uint64_t v[READ_BLOCK_SIZE]; \
            READ_DATA(T, Mat_uint64Swap); \
        } \
    } while (0)
#else
#define READ_DATA_UINT64(T)
#endif /* HAVE_MAT_UINT64_T */

#define READ_DATA_TYPE(T) \
    do { \
        switch ( data_type ) { \
            case MAT_T_DOUBLE: \
            { \
                double v[READ_BLOCK_SIZE]; \
                READ_DATA(T, Mat_doubleSwap); \
                break; \
            } \
            case MAT_T_SINGLE: \
            { \
                float v[READ_BLOCK_SIZE]; \
                READ_DATA(T, Mat_floatSwap); \
                break; \
            } \
            case MAT_T_INT32: \
            { \
                mat_int32_t v[READ_BLOCK_SIZE]; \
                READ_DATA(T, Mat_int32Swap); \
                break; \
            } \
            case MAT_T_UINT32: \
            { \
                mat_uint32_t v[READ_BLOCK_SIZE]; \
                READ_DATA(T, Mat_uint32Swap); \
                break; \
            } \
            case MAT_T_INT16: \
            { \
                mat_int16_t v[READ_BLOCK_SIZE]; \
                READ_DATA(T, Mat_int16Swap); \
                break; \
            } \
            case MAT_T_UINT16: \
            { \
                mat_uint16_t v[READ_BLOCK_SIZE]; \
                READ_DATA(T, Mat_uint16Swap); \
                break; \
            } \
            case MAT_T_INT8: \
            { \
                mat_int8_t v[READ_BLOCK_SIZE]; \
                READ_DATA_NOSWAP(T); \
                break; \
            } \
            case MAT_T_UINT8: \
            { \
                mat_uint8_t v[READ_BLOCK_SIZE]; \
                READ_DATA_NOSWAP(T); \
                break; \
            } \
            default: \
                READ_DATA_INT64(T); \
                READ_DATA_UINT64(T); \
                break; \
        } \
    } while (0)

#if defined(HAVE_ZLIB)
#define READ_COMPRESSED_DATA(T, SwapFunc) \
    do { \
        if ( mat->byteswap ) { \
            for ( i = 0; i < len; i++ ) { \
                InflateData(mat,z,&v,data_size); \
                data[i] = (T)SwapFunc(&v); \
            } \
        } else { \
            for ( i = 0; i < len; i++ ) { \
                InflateData(mat,z,&v,data_size); \
                data[i] = (T)v; \
            } \
        } \
    } while (0)

#ifdef HAVE_MAT_INT64_T
#define READ_COMPRESSED_DATA_INT64(T) \
    do { \
        if ( MAT_T_INT64 == data_type ) { \
            mat_int64_t v; \
            READ_COMPRESSED_DATA(T, Mat_int64Swap); \
        } \
    } while (0)
#else
#define READ_COMPRESSED_DATA_INT64(T)
#endif /* HAVE_MAT_INT64_T */

#ifdef HAVE_MAT_UINT64_T
#define READ_COMPRESSED_DATA_UINT64(T) \
    do { \
        if ( MAT_T_UINT64 == data_type ) { \
            mat_uint64_t v; \
            READ_COMPRESSED_DATA(T, Mat_uint64Swap); \
        } \
    } while (0)
#else
#define READ_COMPRESSED_DATA_UINT64(T)
#endif /* HAVE_MAT_UINT64_T */

#define READ_COMPRESSED_DATA_TYPE(T) \
    do { \
        switch ( data_type ) { \
            case MAT_T_DOUBLE: \
            { \
                double v; \
                READ_COMPRESSED_DATA(T, Mat_doubleSwap); \
                break; \
            } \
            case MAT_T_SINGLE: \
            { \
                float v; \
                READ_COMPRESSED_DATA(T, Mat_floatSwap); \
                break; \
            } \
            case MAT_T_INT32: \
            { \
                mat_int32_t v; \
                READ_COMPRESSED_DATA(T, Mat_int32Swap); \
                break; \
            } \
            case MAT_T_UINT32: \
            { \
                mat_uint32_t v; \
                READ_COMPRESSED_DATA(T, Mat_uint32Swap); \
                break; \
            } \
            case MAT_T_INT16: \
            { \
                mat_int16_t v; \
                READ_COMPRESSED_DATA(T, Mat_int16Swap); \
                break; \
            } \
            case MAT_T_UINT16: \
            { \
                mat_uint16_t v; \
                READ_COMPRESSED_DATA(T, Mat_uint16Swap); \
                break; \
            } \
            case MAT_T_UINT8: \
            { \
                mat_uint8_t v; \
                for ( i = 0; i < len; i++ ) { \
                    InflateData(mat,z,&v,data_size); \
                    data[i] = (T)v; \
                } \
                break; \
            } \
            case MAT_T_INT8: \
            { \
                mat_int8_t v; \
                for ( i = 0; i < len; i++ ) { \
                    InflateData(mat,z,&v,data_size); \
                    data[i] = (T)v; \
                } \
                break; \
            } \
            default: \
                READ_COMPRESSED_DATA_INT64(T); \
                READ_COMPRESSED_DATA_UINT64(T); \
                break; \
        } \
    } while (0)
#endif

/*
 * --------------------------------------------------------------------------
 *    Routines to read data of any type into arrays of a specific type
 * --------------------------------------------------------------------------
 */

/** @cond mat_devman */

/** @brief Reads data of type @c data_type into a double type
 *
 * Reads from the MAT file @c len elements of data type @c data_type storing
 * them as double's in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output double values (len*sizeof(double))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadDoubleData(mat_t *mat,double *data,enum matio_types data_type,int len)
{
    int bytesread = 0, i, j;
    size_t data_size;

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    data_size = Mat_SizeOf(data_type);

    switch ( data_type ) {
        case MAT_T_DOUBLE:
        {
            bytesread += fread(data,data_size,len,(FILE*)mat->fp);
            if ( mat->byteswap ) {
                for ( i = 0; i < len; i++ ) {
                    (void)Mat_doubleSwap(data+i);
                }
            }
            break;
        }
        case MAT_T_SINGLE:
        {
            float v[READ_BLOCK_SIZE];
            READ_DATA(double, Mat_floatSwap);
            break;
        }
#ifdef HAVE_MAT_INT64_T
        case MAT_T_INT64:
        {
            mat_int64_t v[READ_BLOCK_SIZE];
            READ_DATA(double, Mat_int64Swap);
            break;
        }
#endif
#ifdef HAVE_MAT_UINT64_T
        case MAT_T_UINT64:
        {
            mat_uint64_t v[READ_BLOCK_SIZE];
            READ_DATA(double, Mat_uint64Swap);
            break;
        }
#endif
        case MAT_T_INT32:
        {
            mat_int32_t v[READ_BLOCK_SIZE];
            READ_DATA(double, Mat_int32Swap);
            break;
        }
        case MAT_T_UINT32:
        {
            mat_uint32_t v[READ_BLOCK_SIZE];
            READ_DATA(double, Mat_uint32Swap);
            break;
        }
        case MAT_T_INT16:
        {
            mat_int16_t v[READ_BLOCK_SIZE];
            READ_DATA(double, Mat_int16Swap);
            break;
        }
        case MAT_T_UINT16:
        {
            mat_uint16_t v[READ_BLOCK_SIZE];
            READ_DATA(double, Mat_uint16Swap);
            break;
        }
        case MAT_T_INT8:
        {
            mat_int8_t v[READ_BLOCK_SIZE];
            READ_DATA_NOSWAP(double);
            break;
        }
        case MAT_T_UINT8:
        {
            mat_uint8_t v[READ_BLOCK_SIZE];
            READ_DATA_NOSWAP(double);
            break;
        }
        default:
            return 0;
    }
    bytesread *= data_size;
    return bytesread;
}

#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into a double type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as double's in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output double values (len*sizeof(double))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedDoubleData(mat_t *mat,z_streamp z,double *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0, i;
    unsigned int data_size;
    union _buf {
        float           f[256];
#ifdef HAVE_MAT_INT64_T
        mat_int64_t   i64[128];
#endif
#ifdef HAVE_MAT_UINT64_T
        mat_uint64_t ui64[128];
#endif
        mat_int32_t   i32[256];
        mat_uint32_t ui32[256];
        mat_int16_t   i16[512];
        mat_uint16_t ui16[512];
        mat_int8_t     i8[1024];
        mat_uint8_t   ui8[1024];
    } buf;

    data_size = (unsigned int)Mat_SizeOf(data_type);

    switch ( data_type ) {
        case MAT_T_DOUBLE:
        {
            if ( mat->byteswap ) {
                InflateData(mat,z,data,len*data_size);
                for ( i = 0; i < len; i++ )
                    (void)Mat_doubleSwap(data+i);
            } else {
                InflateData(mat,z,data,len*data_size);
            }
            break;
        }
        case MAT_T_SINGLE:
        {
            if ( mat->byteswap ) {
                if ( len <= 256 ){
                    InflateData(mat,z,buf.f,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = Mat_floatSwap(buf.f+i);
                } else {
                    int j;
                    len -= 256;
                    for ( i = 0; i < len; i+=256 ) {
                        InflateData(mat,z,buf.f,256*data_size);
                        for ( j = 0; j < 256; j++ )
                            data[i+j] = Mat_floatSwap(buf.f+j);
                    }
                    len = len-(i-256);
                    InflateData(mat,z,buf.f,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = Mat_floatSwap(buf.f+j);
                }
            } else {
                if ( len <= 256 ){
                    InflateData(mat,z,buf.f,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = buf.f[i];
                } else {
                    int j;
                    len -= 256;
                    for ( i = 0; i < len; i+=256 ) {
                        InflateData(mat,z,buf.f,256*data_size);
                        for ( j = 0; j < 256; j++ )
                            data[i+j] = buf.f[j];
                    }
                    len = len-(i-256);
                    InflateData(mat,z,buf.f,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = buf.f[j];
                }
            }
            break;
        }
#ifdef HAVE_MAT_INT64_T
        case MAT_T_INT64:
        {
            if ( mat->byteswap ) {
                if ( len <= 128 ){
                    InflateData(mat,z,buf.i64,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = (double)Mat_int64Swap(buf.i64+i);
                } else {
                    int j;
                    len -= 128;
                    for ( i = 0; i < len; i+=128 ) {
                        InflateData(mat,z,buf.i64,128*data_size);
                        for ( j = 0; j < 128; j++ )
                            data[i+j] = (double)Mat_int64Swap(buf.i64+j);
                    }
                    len = len-(i-128);
                    InflateData(mat,z,buf.i64,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = (double)Mat_int64Swap(buf.i64+j);
                }
            } else {
                if ( len <= 128 ){
                    InflateData(mat,z,buf.i64,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = (double)buf.i64[i];
                } else {
                    int j;
                    len -= 128;
                    for ( i = 0; i < len; i+=128 ) {
                        InflateData(mat,z,buf.i64,128*data_size);
                        for ( j = 0; j < 128; j++ )
                            data[i+j] = (double)buf.i64[j];
                    }
                    len = len-(i-128);
                    InflateData(mat,z,buf.i64,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = (double)buf.i64[j];
                }
            }
            break;
        }
#endif
#ifdef HAVE_MAT_UINT64_T
        case MAT_T_UINT64:
        {
            if ( mat->byteswap ) {
                if ( len <= 128 ){
                    InflateData(mat,z,buf.ui64,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = (double)Mat_uint64Swap(buf.ui64+i);
                } else {
                    int j;
                    len -= 128;
                    for ( i = 0; i < len; i+=128 ) {
                        InflateData(mat,z,buf.ui64,128*data_size);
                        for ( j = 0; j < 128; j++ )
                            data[i+j] = (double)Mat_uint64Swap(buf.ui64+j);
                    }
                    len = len-(i-128);
                    InflateData(mat,z,buf.ui64,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = (double)Mat_uint64Swap(buf.ui64+j);
                }
            } else {
                if ( len <= 128 ){
                    InflateData(mat,z,buf.ui64,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = (double)buf.ui64[i];
                } else {
                    int j;
                    len -= 128;
                    for ( i = 0; i < len; i+=128 ) {
                        InflateData(mat,z,buf.ui64,128*data_size);
                        for ( j = 0; j < 128; j++ )
                            data[i+j] = (double)buf.ui64[j];
                    }
                    len = len-(i-128);
                    InflateData(mat,z,buf.ui64,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = (double)buf.ui64[j];
                }
            }
            break;
        }
#endif
        case MAT_T_INT32:
        {
            if ( mat->byteswap ) {
                if ( len <= 256 ){
                    InflateData(mat,z,buf.i32,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = Mat_int32Swap(buf.i32+i);
                } else {
                    int j;
                    len -= 256;
                    for ( i = 0; i < len; i+=256 ) {
                        InflateData(mat,z,buf.i32,256*data_size);
                        for ( j = 0; j < 256; j++ )
                            data[i+j] = Mat_int32Swap(buf.i32+j);
                    }
                    len = len-(i-256);
                    InflateData(mat,z,buf.i32,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = Mat_int32Swap(buf.i32+j);
                }
            } else {
                if ( len <= 256 ){
                    InflateData(mat,z,buf.i32,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = buf.i32[i];
                } else {
                    int j;
                    len -= 256;
                    for ( i = 0; i < len; i+=256 ) {
                        InflateData(mat,z,buf.i32,256*data_size);
                        for ( j = 0; j < 256; j++ )
                            data[i+j] = buf.i32[j];
                    }
                    len = len-(i-256);
                    InflateData(mat,z,buf.i32,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = buf.i32[j];
                }
            }
            break;
        }
        case MAT_T_UINT32:
        {
            if ( mat->byteswap ) {
                if ( len <= 256 ){
                    InflateData(mat,z,buf.ui32,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = Mat_uint32Swap(buf.ui32+i);
                } else {
                    int j;
                    len -= 256;
                    for ( i = 0; i < len; i+=256 ) {
                        InflateData(mat,z,buf.ui32,256*data_size);
                        for ( j = 0; j < 256; j++ )
                            data[i+j] = Mat_uint32Swap(buf.ui32+j);
                    }
                    len = len-(i-256);
                    InflateData(mat,z,buf.ui32,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = Mat_uint32Swap(buf.ui32+j);
                }
            } else {
                if ( len <= 256 ) {
                    InflateData(mat,z,buf.ui32,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = buf.ui32[i];
                } else {
                    int j;
                    len -= 256;
                    for ( i = 0; i < len; i+=256 ) {
                        InflateData(mat,z,buf.ui32,256*data_size);
                        for ( j = 0; j < 256; j++ )
                            data[i+j] = buf.ui32[j];
                    }
                    len = len-(i-256);
                    InflateData(mat,z,buf.ui32,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = buf.ui32[j];
                }
            }
            break;
        }
        case MAT_T_INT16:
        {
            if ( mat->byteswap ) {
                if ( len <= 512 ){
                    InflateData(mat,z,buf.i16,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = Mat_int16Swap(buf.i16+i);
                } else {
                    int j;
                    len -= 512;
                    for ( i = 0; i < len; i+=512 ) {
                        InflateData(mat,z,buf.i16,512*data_size);
                        for ( j = 0; j < 512; j++ )
                            data[i+j] = Mat_int16Swap(buf.i16+j);
                    }
                    len = len-(i-512);
                    InflateData(mat,z,buf.i16,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = Mat_int16Swap(buf.i16+j);
                }
            } else {
                if ( len <= 512 ) {
                    InflateData(mat,z,buf.i16,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = buf.i16[i];
                } else {
                    int j;
                    len -= 512;
                    for ( i = 0; i < len; i+=512 ) {
                        InflateData(mat,z,buf.i16,512*data_size);
                        for ( j = 0; j < 512; j++ )
                            data[i+j] = buf.i16[j];
                    }
                    len = len-(i-512);
                    InflateData(mat,z,buf.i16,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = buf.i16[j];
                }
            }
            break;
        }
        case MAT_T_UINT16:
        {
            if ( mat->byteswap ) {
                if ( len <= 512 ){
                    InflateData(mat,z,buf.ui16,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = Mat_uint16Swap(buf.ui16+i);
                } else {
                    int j;
                    len -= 512;
                    for ( i = 0; i < len; i+=512 ) {
                        InflateData(mat,z,buf.ui16,512*data_size);
                        for ( j = 0; j < 512; j++ )
                            data[i+j] = Mat_uint16Swap(buf.ui16+j);
                    }
                    len = len-(i-512);
                    InflateData(mat,z,buf.ui16,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = Mat_uint16Swap(buf.ui16+j);
                }
            } else {
                if ( len <= 512 ) {
                    InflateData(mat,z,buf.ui16,len*data_size);
                    for ( i = 0; i < len; i++ )
                        data[i] = buf.ui16[i];
                } else {
                    int j;
                    len -= 512;
                    for ( i = 0; i < len; i+=512 ) {
                        InflateData(mat,z,buf.ui16,512*data_size);
                        for ( j = 0; j < 512; j++ )
                            data[i+j] = buf.ui16[j];
                    }
                    len = len-(i-512);
                    InflateData(mat,z,buf.ui16,len*data_size);
                    for ( j = 0; j < len; j++ )
                        data[i+j] = buf.ui16[j];
                }
            }
            break;
        }
        case MAT_T_UINT8:
        {
            if ( len <= 1024 ) {
                InflateData(mat,z,buf.ui8,len*data_size);
                for ( i = 0; i < len; i++ )
                    data[i] = buf.ui8[i];
            } else {
                int j;
                len -= 1024;
                for ( i = 0; i < len; i+=1024 ) {
                    InflateData(mat,z,buf.ui8,1024*data_size);
                    for ( j = 0; j < 1024; j++ )
                        data[i+j] = buf.ui8[j];
                }
                len = len-(i-1024);
                InflateData(mat,z,buf.ui8,len*data_size);
                for ( j = 0; j < len; j++ )
                    data[i+j] = buf.ui8[j];
            }
            break;
        }
        case MAT_T_INT8:
        {
            if ( len <= 1024 ) {
                InflateData(mat,z,buf.i8,len*data_size);
                for ( i = 0; i < len; i++ )
                    data[i] = buf.i8[i];
            } else {
                int j;
                len -= 1024;
                for ( i = 0; i < len; i+=1024 ) {
                    InflateData(mat,z,buf.i8,1024*data_size);
                    for ( j = 0; j < 1024; j++ )
                        data[i+j] = buf.i8[j];
                }
                len = len-(i-1024);
                InflateData(mat,z,buf.i8,len*data_size);
                for ( j = 0; j < len; j++ )
                    data[i+j] = buf.i8[j];
            }
            break;
        }
        default:
            return 0;
    }
    nBytes = len*data_size;
    return nBytes;
}
#endif

/** @brief Reads data of type @c data_type into a float type
 *
 * Reads from the MAT file @c len elements of data type @c data_type storing
 * them as float's in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output float values (len*sizeof(float))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadSingleData(mat_t *mat,float *data,enum matio_types data_type,int len)
{
    int bytesread = 0, i, j;
    size_t data_size;

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    data_size = Mat_SizeOf(data_type);
    READ_DATA_TYPE(float);
    bytesread *= data_size;
    return bytesread;
}

#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into a float type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as float's in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output float values (len*sizeof(float))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedSingleData(mat_t *mat,z_streamp z,float *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0, i;
    unsigned int data_size;

    if ( (mat == NULL) || (data == NULL) || (z == NULL) )
        return 0;

    data_size = (unsigned int)Mat_SizeOf(data_type);
    READ_COMPRESSED_DATA_TYPE(float);
    nBytes = len*data_size;
    return nBytes;
}
#endif

#ifdef HAVE_MAT_INT64_T
/** @brief Reads data of type @c data_type into a signed 64-bit integer type
 *
 * Reads from the MAT file @c len elements of data type @c data_type storing
 * them as signed 64-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output signed 64-bit integer values
 *             (len*sizeof(mat_int64_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadInt64Data(mat_t *mat,mat_int64_t *data,enum matio_types data_type,int len)
{
    int bytesread = 0, i, j;
    size_t data_size;

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    data_size = Mat_SizeOf(data_type);
    READ_DATA_TYPE(mat_int64_t);
    bytesread *= data_size;
    return bytesread;
}

#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into a signed 64-bit integer type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as signed 64-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output signed 64-bit integer values
 *             (len*sizeof(mat_int64_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedInt64Data(mat_t *mat,z_streamp z,mat_int64_t *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0, i;
    unsigned int data_size;

    if ( (mat == NULL) || (data == NULL) || (z == NULL) )
        return 0;

    data_size = (unsigned int)Mat_SizeOf(data_type);
    READ_COMPRESSED_DATA_TYPE(mat_int64_t);
    nBytes = len*data_size;
    return nBytes;
}
#endif
#endif /* HAVE_MAT_INT64_T */

#ifdef HAVE_MAT_UINT64_T
/** @brief Reads data of type @c data_type into an unsigned 64-bit integer type
 *
 * Reads from the MAT file @c len elements of data type @c data_type storing
 * them as unsigned 64-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output unsigned 64-bit integer values
 *             (len*sizeof(mat_uint64_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadUInt64Data(mat_t *mat,mat_uint64_t *data,enum matio_types data_type,int len)
{
    int bytesread = 0, i, j;
    size_t data_size;

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    data_size = Mat_SizeOf(data_type);
    READ_DATA_TYPE(mat_uint64_t);
    bytesread *= data_size;
    return bytesread;
}

#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into an unsigned 64-bit integer type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as unsigned 64-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output unsigned 64-bit integer values
 *             (len*sizeof(mat_uint64_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedUInt64Data(mat_t *mat,z_streamp z,mat_uint64_t *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0, i;
    unsigned int data_size;

    if ( (mat == NULL) || (data == NULL) || (z == NULL) )
        return 0;

    data_size = (unsigned int)Mat_SizeOf(data_type);
    READ_COMPRESSED_DATA_TYPE(mat_uint64_t);
    nBytes = len*data_size;
    return nBytes;
}
#endif /* HAVE_ZLIB */
#endif /* HAVE_MAT_UINT64_T */

/** @brief Reads data of type @c data_type into a signed 32-bit integer type
 *
 * Reads from the MAT file @c len elements of data type @c data_type storing
 * them as signed 32-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output signed 32-bit integer values
 *             (len*sizeof(mat_int32_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadInt32Data(mat_t *mat,mat_int32_t *data,enum matio_types data_type,int len)
{
    int bytesread = 0, i, j;
    size_t data_size;

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    data_size = Mat_SizeOf(data_type);
    READ_DATA_TYPE(mat_int32_t);
    bytesread *= data_size;
    return bytesread;
}

#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into a signed 32-bit integer type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as signed 32-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output signed 32-bit integer values
 *             (len*sizeof(mat_int32_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedInt32Data(mat_t *mat,z_streamp z,mat_int32_t *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0, i;
    unsigned int data_size;

    if ( (mat == NULL) || (data == NULL) || (z == NULL) )
        return 0;

    data_size = (unsigned int)Mat_SizeOf(data_type);
    READ_COMPRESSED_DATA_TYPE(mat_int32_t);
    nBytes = len*data_size;
    return nBytes;
}
#endif

/** @brief Reads data of type @c data_type into an unsigned 32-bit integer type
 *
 * Reads from the MAT file @c len elements of data type @c data_type storing
 * them as unsigned 32-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output unsigned 32-bit integer values
 *             (len*sizeof(mat_uint32_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadUInt32Data(mat_t *mat,mat_uint32_t *data,enum matio_types data_type,int len)
{
    int bytesread = 0, i, j;
    size_t data_size;

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    data_size = Mat_SizeOf(data_type);
    READ_DATA_TYPE(mat_uint32_t);
    bytesread *= data_size;
    return bytesread;
}

#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into an unsigned 32-bit integer type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as unsigned 32-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output unsigned 32-bit integer values
 *             (len*sizeof(mat_uint32_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedUInt32Data(mat_t *mat,z_streamp z,mat_uint32_t *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0, i;
    unsigned int data_size;

    if ( (mat == NULL) || (data == NULL) || (z == NULL) )
        return 0;

    data_size = (unsigned int)Mat_SizeOf(data_type);
    READ_COMPRESSED_DATA_TYPE(mat_uint32_t);
    nBytes = len*data_size;
    return nBytes;
}
#endif

/** @brief Reads data of type @c data_type into a signed 16-bit integer type
 *
 * Reads from the MAT file @c len elements of data type @c data_type storing
 * them as signed 16-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output signed 16-bit integer values
 *             (len*sizeof(mat_int16_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadInt16Data(mat_t *mat,mat_int16_t *data,enum matio_types data_type,int len)
{
    int bytesread = 0, i, j;
    size_t data_size;

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    data_size = Mat_SizeOf(data_type);
    READ_DATA_TYPE(mat_int16_t);
    bytesread *= data_size;
    return bytesread;
}

#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into a signed 16-bit integer type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as signed 16-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output signed 16-bit integer values
 *             (len*sizeof(mat_int16_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedInt16Data(mat_t *mat,z_streamp z,mat_int16_t *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0, i;
    unsigned int data_size;

    if ( (mat == NULL) || (data == NULL) || (z == NULL) )
        return 0;

    data_size = (unsigned int)Mat_SizeOf(data_type);
    READ_COMPRESSED_DATA_TYPE(mat_int16_t);
    nBytes = len*data_size;
    return nBytes;
}
#endif

/** @brief Reads data of type @c data_type into an unsigned 16-bit integer type
 *
 * Reads from the MAT file @c len elements of data type @c data_type storing
 * them as unsigned 16-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output unsigned 16-bit integer values
 *             (len*sizeof(mat_uint16_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadUInt16Data(mat_t *mat,mat_uint16_t *data,enum matio_types data_type,int len)
{
    int bytesread = 0, i, j;
    size_t data_size;

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    data_size = Mat_SizeOf(data_type);
    READ_DATA_TYPE(mat_uint16_t);
    bytesread *= data_size;
    return bytesread;
}

#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into an unsigned 16-bit integer type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as unsigned 16-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output n unsigned 16-bit integer values
 *             (len*sizeof(mat_uint16_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedUInt16Data(mat_t *mat,z_streamp z,mat_uint16_t *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0, i;
    unsigned int data_size;

    if ( (mat == NULL) || (data == NULL) || (z == NULL) )
        return 0;

    data_size = (unsigned int)Mat_SizeOf(data_type);
    READ_COMPRESSED_DATA_TYPE(mat_uint16_t);
    nBytes = len*data_size;
    return nBytes;
}
#endif

/** @brief Reads data of type @c data_type into a signed 8-bit integer type
 *
 * Reads from the MAT file @c len elements of data type @c data_type storing
 * them as signed 8-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output signed 8-bit integer values
 *             (len*sizeof(mat_int8_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadInt8Data(mat_t *mat,mat_int8_t *data,enum matio_types data_type,int len)
{
    int bytesread = 0, i, j;
    size_t data_size;

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    data_size = Mat_SizeOf(data_type);
    READ_DATA_TYPE(mat_int8_t);
    bytesread *= data_size;
    return bytesread;
}

#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into a signed 8-bit integer type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as signed 8-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output signed 8-bit integer values
 *             (len*sizeof(mat_int8_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedInt8Data(mat_t *mat,z_streamp z,mat_int8_t *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0, i;
    unsigned int data_size;

    if ( (mat == NULL) || (data == NULL) || (z == NULL) )
        return 0;

    data_size = (unsigned int)Mat_SizeOf(data_type);
    READ_COMPRESSED_DATA_TYPE(mat_int8_t);
    nBytes = len*data_size;
    return nBytes;
}
#endif

/** @brief Reads data of type @c data_type into an unsigned 8-bit integer type
 *
 * Reads from the MAT file @c len elements of data type @c data_type storing
 * them as unsigned 8-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output unsigned 8-bit integer values
 *             (len*sizeof(mat_uint8_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadUInt8Data(mat_t *mat,mat_uint8_t *data,enum matio_types data_type,int len)
{
    int bytesread = 0, i, j;
    size_t data_size;

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    data_size = Mat_SizeOf(data_type);
    READ_DATA_TYPE(mat_uint8_t);
    bytesread *= data_size;
    return bytesread;
}

#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into an unsigned 8-bit integer type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as unsigned 8-bit integers in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output 8-bit integer values
 *             (len*sizeof(mat_uint8_t))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedUInt8Data(mat_t *mat,z_streamp z,mat_uint8_t *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0, i;
    unsigned int data_size;

    if ( (mat == NULL) || (data == NULL) || (z == NULL) )
        return 0;

    data_size = (unsigned int)Mat_SizeOf(data_type);
    READ_COMPRESSED_DATA_TYPE(mat_uint8_t);
    nBytes = len*data_size;
    return nBytes;
}
#endif

#undef READ_DATA
#undef READ_DATA_TYPE
#undef READ_DATA_INT64
#undef READ_DATA_UINT64
#if defined(HAVE_ZLIB)
#undef READ_COMPRESSED_DATA
#undef READ_COMPRESSED_DATA_TYPE
#undef READ_COMPRESSED_DATA_INT64
#undef READ_COMPRESSED_DATA_UINT64
#endif
#if defined(HAVE_ZLIB)
/** @brief Reads data of type @c data_type into a char type
 *
 * Reads from the MAT file @c len compressed elements of data type @c data_type
 * storing them as char's in @c data.
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z Pointer to the zlib stream for inflation
 * @param data Pointer to store the output char values (len*sizeof(char))
 * @param data_type one of the @c matio_types enumerations which is the source
 *                  data type in the file
 * @param len Number of elements of type @c data_type to read from the file
 * @retval Number of bytes read from the file
 */
int
ReadCompressedCharData(mat_t *mat,z_streamp z,char *data,
    enum matio_types data_type,int len)
{
    int nBytes = 0;
    unsigned int data_size;

    if ( mat == NULL || data == NULL || mat->fp == NULL )
        return 0;

    data_size = (unsigned int)Mat_SizeOf(data_type);

    switch ( data_type ) {
        case MAT_T_UINT8:
        case MAT_T_UTF8:
            InflateData(mat,z,data,len*data_size);
            break;
        case MAT_T_UINT16:
        case MAT_T_UTF16:
            InflateData(mat,z,data,len*data_size);
            if ( mat->byteswap ) {
                int i;
                for ( i = 0; i < len; i++ ) {
                    Mat_uint16Swap((mat_uint16_t*)&data[2*i]);
                }
            }
            break;
        default:
            Mat_Warning("ReadCompressedCharData: %d is not a supported data "
                "type for character data", data_type);
            break;
    }
    nBytes = len*data_size;
    return nBytes;
}
#endif

int
ReadCharData(mat_t *mat,char *data,enum matio_types data_type,int len)
{
    int bytesread = 0;
    size_t data_size;

    if ( mat == NULL || data == NULL || mat->fp == NULL )
        return 0;

    data_size = Mat_SizeOf(data_type);

    switch ( data_type ) {
        case MAT_T_UINT8:
        case MAT_T_UTF8:
            bytesread += fread(data,data_size,len,(FILE*)mat->fp);
            break;
        case MAT_T_UINT16:
        case MAT_T_UTF16:
        {
            mat_uint16_t ui16;
            int i;
            if ( mat->byteswap ) {
                for ( i = 0; i < len; i++ ) {
                    bytesread += fread(&ui16,data_size,1,(FILE*)mat->fp);
                    data[i] = (char)Mat_uint16Swap(&ui16);
                }
            } else {
                for ( i = 0; i < len; i++ ) {
                    bytesread += fread(&ui16,data_size,1,(FILE*)mat->fp);
                    data[i] = (char)ui16;
                }
            }
            break;
        }
        default:
            Mat_Warning("ReadCharData: %d is not a supported data type for "
                "character data", data_type);
            break;
    }
    return bytesread;
}

/*
 *-------------------------------------------------------------------
 *  Routines to read "slabs" of data
 *-------------------------------------------------------------------
 */

#define READ_DATA_SLABN_RANK_LOOP \
    do { \
        for ( j = 1; j < rank; j++ ) { \
            cnt[j]++; \
            if ( (cnt[j] % edge[j]) == 0 ) { \
                cnt[j] = 0; \
                if ( (I % dimp[j]) != 0 ) { \
                    (void)fseek((FILE*)mat->fp,data_size*(dimp[j]-(I % dimp[j]) + dimp[j-1]*start[j]),SEEK_CUR); \
                    I += dimp[j]-(I % dimp[j]) + (ptrdiff_t)dimp[j-1]*start[j]; \
                } else if ( start[j] ) { \
                    (void)fseek((FILE*)mat->fp,data_size*(dimp[j-1]*start[j]),SEEK_CUR); \
                    I += (ptrdiff_t)dimp[j-1]*start[j]; \
                } \
            } else { \
                I += inc[j]; \
                (void)fseek((FILE*)mat->fp,data_size*inc[j],SEEK_CUR); \
                break; \
            } \
        } \
    } while (0)

#define READ_DATA_SLABN(ReadDataFunc) \
    do { \
        inc[0]  = stride[0]-1; \
        dimp[0] = dims[0]; \
        N       = edge[0]; \
        I       = 0; /* start[0]; */ \
        for ( i = 1; i < rank; i++ ) { \
            inc[i]  = stride[i]-1; \
            dimp[i] = dims[i-1]; \
            for ( j = i; j--; ) { \
                inc[i]  *= dims[j]; \
                dimp[i] *= dims[j+1]; \
            } \
            N *= edge[i]; \
            I += (ptrdiff_t)dimp[i-1]*start[i]; \
        } \
        (void)fseek((FILE*)mat->fp,I*data_size,SEEK_CUR); \
        if ( stride[0] == 1 ) { \
            for ( i = 0; i < N; i+=edge[0] ) { \
                if ( start[0] ) { \
                    (void)fseek((FILE*)mat->fp,start[0]*data_size,SEEK_CUR); \
                    I += start[0]; \
                } \
                ReadDataFunc(mat,ptr+i,data_type,edge[0]); \
                I += dims[0]-start[0]; \
                (void)fseek((FILE*)mat->fp,data_size*(dims[0]-edge[0]-start[0]), \
                    SEEK_CUR); \
                READ_DATA_SLABN_RANK_LOOP; \
            } \
        } else { \
            for ( i = 0; i < N; i+=edge[0] ) { \
                if ( start[0] ) { \
                    (void)fseek((FILE*)mat->fp,start[0]*data_size,SEEK_CUR); \
                    I += start[0]; \
                } \
                for ( j = 0; j < edge[0]; j++ ) { \
                    ReadDataFunc(mat,ptr+i+j,data_type,1); \
                    (void)fseek((FILE*)mat->fp,data_size*(stride[0]-1),SEEK_CUR); \
                    I += stride[0]; \
                } \
                I += dims[0]-(ptrdiff_t)edge[0]*stride[0]-start[0]; \
                (void)fseek((FILE*)mat->fp,data_size* \
                    (dims[0]-(ptrdiff_t)edge[0]*stride[0]-start[0]),SEEK_CUR); \
                READ_DATA_SLABN_RANK_LOOP; \
            } \
        } \
    } while (0)

/** @brief Reads data of type @c data_type by user-defined dimensions
 *
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output data
 * @param class_type Type of data class (matio_classes enumerations)
 * @param data_type Datatype of the stored data (matio_types enumerations)
 * @param rank Number of dimensions in the data
 * @param dims Dimensions of the data
 * @param start Index to start reading data in each dimension
 * @param stride Read every @c stride elements in each dimension
 * @param edge Number of elements to read in each dimension
 * @retval Number of bytes read from the file, or -1 on error
 */
int
ReadDataSlabN(mat_t *mat,void *data,enum matio_classes class_type,
    enum matio_types data_type,int rank,size_t *dims,int *start,int *stride,
    int *edge)
{
    int nBytes = 0, i, j, N, I = 0;
    int inc[10] = {0,}, cnt[10] = {0,}, dimp[10] = {0,};
    size_t data_size;

    if ( (mat   == NULL) || (data   == NULL) || (mat->fp == NULL) ||
         (start == NULL) || (stride == NULL) || (edge    == NULL) ) {
        return -1;
    } else if ( rank > 10 ) {
        return -1;
    }

    data_size = Mat_SizeOf(data_type);

    switch ( class_type ) {
        case MAT_C_DOUBLE:
        {
            double *ptr = (double*)data;
            READ_DATA_SLABN(ReadDoubleData);
            break;
        }
        case MAT_C_SINGLE:
        {
            float *ptr = (float*)data;
            READ_DATA_SLABN(ReadSingleData);
            break;
        }
#ifdef HAVE_MAT_INT64_T
        case MAT_C_INT64:
        {
            mat_int64_t *ptr = (mat_int64_t*)data;
            READ_DATA_SLABN(ReadInt64Data);
            break;
        }
#endif /* HAVE_MAT_INT64_T */
#ifdef HAVE_MAT_UINT64_T
        case MAT_C_UINT64:
        {
            mat_uint64_t *ptr = (mat_uint64_t*)data;
            READ_DATA_SLABN(ReadUInt64Data);
            break;
        }
#endif /* HAVE_MAT_UINT64_T */
        case MAT_C_INT32:
        {
            mat_int32_t *ptr = (mat_int32_t*)data;
            READ_DATA_SLABN(ReadInt32Data);
            break;
        }
        case MAT_C_UINT32:
        {
            mat_uint32_t *ptr = (mat_uint32_t*)data;
            READ_DATA_SLABN(ReadUInt32Data);
            break;
        }
        case MAT_C_INT16:
        {
            mat_int16_t *ptr = (mat_int16_t*)data;
            READ_DATA_SLABN(ReadInt16Data);
            break;
        }
        case MAT_C_UINT16:
        {
            mat_uint16_t *ptr = (mat_uint16_t*)data;
            READ_DATA_SLABN(ReadUInt16Data);
            break;
        }
        case MAT_C_INT8:
        {
            mat_int8_t *ptr = (mat_int8_t*)data;
            READ_DATA_SLABN(ReadInt8Data);
            break;
        }
        case MAT_C_UINT8:
        {
            mat_uint8_t *ptr = (mat_uint8_t*)data;
            READ_DATA_SLABN(ReadUInt8Data);
            break;
        }
        default:
            nBytes = 0;
    }
    return nBytes;
}

#undef READ_DATA_SLABN
#undef READ_DATA_SLABN_RANK_LOOP

#if defined(HAVE_ZLIB)
#define READ_COMPRESSED_DATA_SLABN_RANK_LOOP \
    do { \
        for ( j = 1; j < rank; j++ ) { \
            cnt[j]++; \
            if ( (cnt[j] % edge[j]) == 0 ) { \
                cnt[j] = 0; \
                if ( (I % dimp[j]) != 0 ) { \
                    InflateSkipData(mat,&z_copy,data_type, dimp[j]-(I % dimp[j]) + dimp[j-1]*start[j]); \
                    I += dimp[j]-(I % dimp[j]) + (ptrdiff_t)dimp[j-1]*start[j]; \
                } else if ( start[j] ) { \
                    InflateSkipData(mat,&z_copy,data_type, dimp[j-1]*start[j]); \
                    I += (ptrdiff_t)dimp[j-1]*start[j]; \
                } \
            } else { \
                if ( inc[j] ) { \
                    I += inc[j]; \
                    InflateSkipData(mat,&z_copy,data_type,inc[j]); \
                } \
                break; \
            } \
        } \
    } while (0)

#define READ_COMPRESSED_DATA_SLABN(ReadDataFunc) \
    do { \
        inc[0]  = stride[0]-1; \
        dimp[0] = dims[0]; \
        N       = edge[0]; \
        I       = 0; \
        for ( i = 1; i < rank; i++ ) { \
            inc[i]  = stride[i]-1; \
            dimp[i] = dims[i-1]; \
            for ( j = i; j--; ) { \
                inc[i]  *= dims[j]; \
                dimp[i] *= dims[j+1]; \
            } \
            N *= edge[i]; \
            I += (ptrdiff_t)dimp[i-1]*start[i]; \
        } \
        /* Skip all data to the starting indices */ \
        InflateSkipData(mat,&z_copy,data_type,I); \
        if ( stride[0] == 1 ) { \
            for ( i = 0; i < N; i+=edge[0] ) { \
                if ( start[0] ) { \
                    InflateSkipData(mat,&z_copy,data_type,start[0]); \
                    I += start[0]; \
                } \
                ReadDataFunc(mat,&z_copy,ptr+i,data_type,edge[0]); \
                InflateSkipData(mat,&z_copy,data_type,dims[0]-start[0]-edge[0]); \
                I += dims[0]-start[0]; \
                READ_COMPRESSED_DATA_SLABN_RANK_LOOP; \
            } \
        } else { \
            for ( i = 0; i < N; i+=edge[0] ) { \
                if ( start[0] ) { \
                    InflateSkipData(mat,&z_copy,data_type,start[0]); \
                    I += start[0]; \
                } \
                for ( j = 0; j < edge[0]-1; j++ ) { \
                    ReadDataFunc(mat,&z_copy,ptr+i+j,data_type,1); \
                    InflateSkipData(mat,&z_copy,data_type,(stride[0]-1)); \
                    I += stride[0]; \
                } \
                ReadDataFunc(mat,&z_copy,ptr+i+j,data_type,1); \
                I += dims[0]-(ptrdiff_t)(edge[0]-1)*stride[0]-start[0]; \
                InflateSkipData(mat,&z_copy,data_type,dims[0]-(ptrdiff_t)(edge[0]-1)*stride[0]-start[0]-1); \
                READ_COMPRESSED_DATA_SLABN_RANK_LOOP; \
            } \
        } \
    } while (0)

/** @brief Reads data of type @c data_type by user-defined dimensions
 *
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z zlib compression stream
 * @param data Pointer to store the output data
 * @param class_type Type of data class (matio_classes enumerations)
 * @param data_type Datatype of the stored data (matio_types enumerations)
 * @param rank Number of dimensions in the data
 * @param dims Dimensions of the data
 * @param start Index to start reading data in each dimension
 * @param stride Read every @c stride elements in each dimension
 * @param edge Number of elements to read in each dimension
 * @retval Number of bytes read from the file, or -1 on error
 */
int
ReadCompressedDataSlabN(mat_t *mat,z_streamp z,void *data,
    enum matio_classes class_type,enum matio_types data_type,int rank,
    size_t *dims,int *start,int *stride,int *edge)
{
    int nBytes = 0, i, j, N, I = 0;
    int inc[10] = {0,}, cnt[10] = {0,}, dimp[10] = {0,};
    z_stream z_copy = {0,};

    if ( (mat   == NULL) || (data   == NULL) || (mat->fp == NULL) ||
         (start == NULL) || (stride == NULL) || (edge    == NULL) ) {
        return 1;
    } else if ( rank > 10 ) {
        return 1;
    }

    i = inflateCopy(&z_copy,z);
    switch ( class_type ) {
        case MAT_C_DOUBLE:
        {
            double *ptr = (double*)data;
            READ_COMPRESSED_DATA_SLABN(ReadCompressedDoubleData);
            break;
        }
        case MAT_C_SINGLE:
        {
            float *ptr = (float*)data;
            READ_COMPRESSED_DATA_SLABN(ReadCompressedSingleData);
            break;
        }
#ifdef HAVE_MAT_INT64_T
        case MAT_C_INT64:
        {
            mat_int64_t *ptr = (mat_int64_t*)data;
            READ_COMPRESSED_DATA_SLABN(ReadCompressedInt64Data);
            break;
        }
#endif /* HAVE_MAT_INT64_T */
#ifdef HAVE_MAT_UINT64_T
        case MAT_C_UINT64:
        {
            mat_uint64_t *ptr = (mat_uint64_t*)data;
            READ_COMPRESSED_DATA_SLABN(ReadCompressedUInt64Data);
            break;
        }
#endif /* HAVE_MAT_UINT64_T */
        case MAT_C_INT32:
        {
            mat_int32_t *ptr = (mat_int32_t*)data;
            READ_COMPRESSED_DATA_SLABN(ReadCompressedInt32Data);
            break;
        }
        case MAT_C_UINT32:
        {
            mat_uint32_t *ptr = (mat_uint32_t*)data;
            READ_COMPRESSED_DATA_SLABN(ReadCompressedUInt32Data);
            break;
        }
        case MAT_C_INT16:
        {
            mat_int16_t *ptr = (mat_int16_t*)data;
            READ_COMPRESSED_DATA_SLABN(ReadCompressedInt16Data);
            break;
        }
        case MAT_C_UINT16:
        {
            mat_uint16_t *ptr = (mat_uint16_t*)data;
            READ_COMPRESSED_DATA_SLABN(ReadCompressedUInt16Data);
            break;
        }
        case MAT_C_INT8:
        {
            mat_int8_t *ptr = (mat_int8_t*)data;
            READ_COMPRESSED_DATA_SLABN(ReadCompressedInt8Data);
            break;
        }
        case MAT_C_UINT8:
        {
            mat_uint8_t *ptr = (mat_uint8_t*)data;
            READ_COMPRESSED_DATA_SLABN(ReadCompressedUInt8Data);
            break;
        }
        default:
            nBytes = 0;
    }
    inflateEnd(&z_copy);
    return nBytes;
}

#undef READ_COMPRESSED_DATA_SLABN
#undef READ_COMPRESSED_DATA_SLABN_RANK_LOOP
#endif

#define READ_DATA_SLAB1(ReadDataFunc) \
    do { \
        if ( !stride ) { \
            bytesread+=ReadDataFunc(mat,ptr,data_type,edge); \
        } else { \
            for ( i = 0; i < edge; i++ ) { \
                bytesread+=ReadDataFunc(mat,ptr+i,data_type,1); \
                (void)fseek((FILE*)mat->fp,stride,SEEK_CUR); \
            } \
        } \
    } while (0)

/** @brief Reads data of type @c data_type by user-defined dimensions for 1-D
 *         data
 *
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output data
 * @param class_type Type of data class (matio_classes enumerations)
 * @param data_type Datatype of the stored data (matio_types enumerations)
 * @param start Index to start reading data
 * @param stride Read every @c stride elements
 * @param edge Number of elements to read
 * @return Number of bytes read from the file, or -1 on error
 */
int
ReadDataSlab1(mat_t *mat,void *data,enum matio_classes class_type,
    enum matio_types data_type,int start,int stride,int edge)
{
    int i;
    size_t data_size;
    int    bytesread = 0;

    data_size = Mat_SizeOf(data_type);
    (void)fseek((FILE*)mat->fp,start*data_size,SEEK_CUR);
    stride = data_size*(stride-1);

    switch ( class_type ) {
        case MAT_C_DOUBLE:
        {
            double *ptr = (double*)data;
            READ_DATA_SLAB1(ReadDoubleData);
            break;
        }
        case MAT_C_SINGLE:
        {
            float *ptr = (float*)data;
            READ_DATA_SLAB1(ReadSingleData);
            break;
        }
#ifdef HAVE_MAT_INT64_T
        case MAT_C_INT64:
        {
            mat_int64_t *ptr = (mat_int64_t*)data;
            READ_DATA_SLAB1(ReadInt64Data);
            break;
        }
#endif /* HAVE_MAT_INT64_T */
#ifdef HAVE_MAT_UINT64_T
        case MAT_C_UINT64:
        {
            mat_uint64_t *ptr = (mat_uint64_t*)data;
            READ_DATA_SLAB1(ReadUInt64Data);
            break;
        }
#endif /* HAVE_MAT_UINT64_T */
        case MAT_C_INT32:
        {
            mat_int32_t *ptr = (mat_int32_t*)data;
            READ_DATA_SLAB1(ReadInt32Data);
            break;
        }
        case MAT_C_UINT32:
        {
            mat_uint32_t *ptr = (mat_uint32_t*)data;
            READ_DATA_SLAB1(ReadUInt32Data);
            break;
        }
        case MAT_C_INT16:
        {
            mat_int16_t *ptr = (mat_int16_t*)data;
            READ_DATA_SLAB1(ReadInt16Data);
            break;
        }
        case MAT_C_UINT16:
        {
            mat_uint16_t *ptr = (mat_uint16_t*)data;
            READ_DATA_SLAB1(ReadUInt16Data);
            break;
        }
        case MAT_C_INT8:
        {
            mat_int8_t *ptr = (mat_int8_t*)data;
            READ_DATA_SLAB1(ReadInt8Data);
            break;
        }
        case MAT_C_UINT8:
        {
            mat_uint8_t *ptr = (mat_uint8_t*)data;
            READ_DATA_SLAB1(ReadUInt8Data);
            break;
        }
        default:
            return 0;
    }

    return bytesread;
}

#undef READ_DATA_SLAB1

#define READ_DATA_SLAB2(ReadDataFunc) \
    do { \
        /* If stride[0] is 1 and stride[1] is 1, we are reading all of the */ \
        /* data so get rid of the loops. */ \
        if ( (stride[0] == 1 && edge[0] == dims[0]) && \
             (stride[1] == 1) ) { \
            ReadDataFunc(mat,ptr,data_type,(ptrdiff_t)edge[0]*edge[1]); \
        } else { \
            row_stride = (long)(stride[0]-1)*data_size; \
            col_stride = (long)stride[1]*dims[0]*data_size; \
            pos = ftell((FILE*)mat->fp); \
            if ( pos == -1L ) { \
                Mat_Critical("Couldn't determine file position"); \
                return -1; \
            } \
            (void)fseek((FILE*)mat->fp,(long)start[1]*dims[0]*data_size,SEEK_CUR); \
            for ( i = 0; i < edge[1]; i++ ) { \
                pos = ftell((FILE*)mat->fp); \
                if ( pos == -1L ) { \
                    Mat_Critical("Couldn't determine file position"); \
                    return -1; \
                } \
                (void)fseek((FILE*)mat->fp,(long)start[0]*data_size,SEEK_CUR); \
                for ( j = 0; j < edge[0]; j++ ) { \
                    ReadDataFunc(mat,ptr++,data_type,1); \
                    (void)fseek((FILE*)mat->fp,row_stride,SEEK_CUR); \
                } \
                pos2 = ftell((FILE*)mat->fp); \
                if ( pos2 == -1L ) { \
                    Mat_Critical("Couldn't determine file position"); \
                    return -1; \
                } \
                pos +=col_stride-pos2; \
                (void)fseek((FILE*)mat->fp,pos,SEEK_CUR); \
            } \
        } \
    } while (0)

/** @brief Reads data of type @c data_type by user-defined dimensions for 2-D
 *         data
 *
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param data Pointer to store the output data
 * @param class_type Type of data class (matio_classes enumerations)
 * @param data_type Datatype of the stored data (matio_types enumerations)
 * @param dims Dimensions of the data
 * @param start Index to start reading data in each dimension
 * @param stride Read every @c stride elements in each dimension
 * @param edge Number of elements to read in each dimension
 * @retval Number of bytes read from the file, or -1 on error
 */
int
ReadDataSlab2(mat_t *mat,void *data,enum matio_classes class_type,
    enum matio_types data_type,size_t *dims,int *start,int *stride,int *edge)
{
    int nBytes = 0, data_size, i, j;
    long pos, row_stride, col_stride, pos2;

    if ( (mat   == NULL) || (data   == NULL) || (mat->fp == NULL) ||
         (start == NULL) || (stride == NULL) || (edge    == NULL) ) {
        return 0;
    }

    data_size = Mat_SizeOf(data_type);

    switch ( class_type ) {
        case MAT_C_DOUBLE:
        {
            double *ptr = (double*)data;
            READ_DATA_SLAB2(ReadDoubleData);
            break;
        }
        case MAT_C_SINGLE:
        {
            float *ptr = (float*)data;
            READ_DATA_SLAB2(ReadSingleData);
            break;
        }
#ifdef HAVE_MAT_INT64_T
        case MAT_C_INT64:
        {
            mat_int64_t *ptr = (mat_int64_t*)data;
            READ_DATA_SLAB2(ReadInt64Data);
            break;
        }
#endif /* HAVE_MAT_INT64_T */
#ifdef HAVE_MAT_UINT64_T
        case MAT_C_UINT64:
        {
            mat_uint64_t *ptr = (mat_uint64_t*)data;
            READ_DATA_SLAB2(ReadUInt64Data);
            break;
        }
#endif /* HAVE_MAT_UINT64_T */
        case MAT_C_INT32:
        {
            mat_int32_t *ptr = (mat_int32_t*)data;
            READ_DATA_SLAB2(ReadInt32Data);
            break;
        }
        case MAT_C_UINT32:
        {
            mat_uint32_t *ptr = (mat_uint32_t*)data;
            READ_DATA_SLAB2(ReadUInt32Data);
            break;
        }
        case MAT_C_INT16:
        {
            mat_int16_t *ptr = (mat_int16_t*)data;
            READ_DATA_SLAB2(ReadInt16Data);
            break;
        }
        case MAT_C_UINT16:
        {
            mat_uint16_t *ptr = (mat_uint16_t*)data;
            READ_DATA_SLAB2(ReadUInt16Data);
            break;
        }
        case MAT_C_INT8:
        {
            mat_int8_t *ptr = (mat_int8_t*)data;
            READ_DATA_SLAB2(ReadInt8Data);
            break;
        }
        case MAT_C_UINT8:
        {
            mat_uint8_t *ptr = (mat_uint8_t*)data;
            READ_DATA_SLAB2(ReadUInt8Data);
            break;
        }
        default:
            nBytes = 0;
    }
    return nBytes;
}

#undef READ_DATA_SLAB2

#if defined(HAVE_ZLIB)
#define READ_COMPRESSED_DATA_SLAB1(ReadDataFunc) \
    do { \
        if ( !stride ) { \
            nBytes+=ReadDataFunc(mat,&z_copy,ptr,data_type,edge); \
        } else { \
            for ( i = 0; i < edge; i++ ) { \
                nBytes+=ReadDataFunc(mat,&z_copy,ptr+i,data_type,1); \
                InflateSkipData(mat,&z_copy,data_type,stride); \
            } \
        } \
    } while (0)

/** @brief Reads data of type @c data_type by user-defined dimensions for 1-D
 *         data
 *
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z zlib compression stream
 * @param data Pointer to store the output data
 * @param class_type Type of data class (matio_classes enumerations)
 * @param data_type Datatype of the stored data (matio_types enumerations)
 * @param dims Dimensions of the data
 * @param start Index to start reading data in each dimension
 * @param stride Read every @c stride elements in each dimension
 * @param edge Number of elements to read in each dimension
 * @retval Number of bytes read from the file, or -1 on error
 */
int
ReadCompressedDataSlab1(mat_t *mat,z_streamp z,void *data,
    enum matio_classes class_type,enum matio_types data_type,int start,
    int stride,int edge)
{
    int nBytes = 0, i, err;
    z_stream z_copy = {0,};

    if ( (mat == NULL) || (data == NULL) || (mat->fp == NULL) )
        return 0;

    stride--;
    err = inflateCopy(&z_copy,z);
    InflateSkipData(mat,&z_copy,data_type,start);
    switch ( class_type ) {
        case MAT_C_DOUBLE:
        {
            double *ptr = (double*)data;
            READ_COMPRESSED_DATA_SLAB1(ReadCompressedDoubleData);
            break;
        }
        case MAT_C_SINGLE:
        {
            float *ptr = (float*)data;
            READ_COMPRESSED_DATA_SLAB1(ReadCompressedSingleData);
            break;
        }
#ifdef HAVE_MAT_INT64_T
        case MAT_C_INT64:
        {
            mat_int64_t *ptr = (mat_int64_t*)data;
            READ_COMPRESSED_DATA_SLAB1(ReadCompressedInt64Data);
            break;
        }
#endif /* HAVE_MAT_INT64_T */
#ifdef HAVE_MAT_UINT64_T
        case MAT_C_UINT64:
        {
            mat_uint64_t *ptr = (mat_uint64_t*)data;
            READ_COMPRESSED_DATA_SLAB1(ReadCompressedUInt64Data);
            break;
        }
#endif /* HAVE_MAT_UINT64_T */
        case MAT_C_INT32:
        {
            mat_int32_t *ptr = (mat_int32_t*)data;
            READ_COMPRESSED_DATA_SLAB1(ReadCompressedInt32Data);
            break;
        }
        case MAT_C_UINT32:
        {
            mat_uint32_t *ptr = (mat_uint32_t*)data;
            READ_COMPRESSED_DATA_SLAB1(ReadCompressedUInt32Data);
            break;
        }
        case MAT_C_INT16:
        {
            mat_int16_t *ptr = (mat_int16_t*)data;
            READ_COMPRESSED_DATA_SLAB1(ReadCompressedInt16Data);
            break;
        }
        case MAT_C_UINT16:
        {
            mat_uint16_t *ptr = (mat_uint16_t*)data;
            READ_COMPRESSED_DATA_SLAB1(ReadCompressedUInt16Data);
            break;
        }
        case MAT_C_INT8:
        {
            mat_int8_t *ptr = (mat_int8_t*)data;
            READ_COMPRESSED_DATA_SLAB1(ReadCompressedInt8Data);
            break;
        }
        case MAT_C_UINT8:
        {
            mat_uint8_t *ptr = (mat_uint8_t*)data;
            READ_COMPRESSED_DATA_SLAB1(ReadCompressedUInt8Data);
            break;
        }
        default:
            break;
    }
    inflateEnd(&z_copy);
    return nBytes;
}

#undef READ_COMPRESSED_DATA_SLAB1

#define READ_COMPRESSED_DATA_SLAB2(ReadDataFunc) \
    do {\
        row_stride = (stride[0]-1); \
        col_stride = (stride[1]-1)*dims[0]; \
        InflateSkipData(mat,&z_copy,data_type,start[1]*dims[0]); \
        /* If stride[0] is 1 and stride[1] is 1, we are reading all of the */ \
        /* data so get rid of the loops.  If stride[0] is 1 and stride[1] */ \
        /* is not 0, we are reading whole columns, so get rid of inner loop */ \
        /* to speed up the code */ \
        if ( (stride[0] == 1 && edge[0] == dims[0]) && \
             (stride[1] == 1) ) { \
            ReadDataFunc(mat,&z_copy,ptr,data_type,(ptrdiff_t)edge[0]*edge[1]); \
        } else if ( stride[0] == 1 ) { \
            for ( i = 0; i < edge[1]; i++ ) { \
                InflateSkipData(mat,&z_copy,data_type,start[0]); \
                ReadDataFunc(mat,&z_copy,ptr,data_type,edge[0]); \
                ptr += edge[0]; \
                pos = dims[0]-(ptrdiff_t)(edge[0]-1)*stride[0]-1-start[0] + col_stride; \
                InflateSkipData(mat,&z_copy,data_type,pos); \
            } \
        } else { \
            for ( i = 0; i < edge[1]; i++ ) { \
                InflateSkipData(mat,&z_copy,data_type,start[0]); \
                for ( j = 0; j < edge[0]-1; j++ ) { \
                    ReadDataFunc(mat,&z_copy,ptr++,data_type,1); \
                    InflateSkipData(mat,&z_copy,data_type,row_stride); \
                } \
                ReadDataFunc(mat,&z_copy,ptr++,data_type,1); \
                pos = dims[0]-(ptrdiff_t)(edge[0]-1)*stride[0]-1-start[0] + col_stride; \
                InflateSkipData(mat,&z_copy,data_type,pos); \
            } \
        } \
    } while (0)

/** @brief Reads data of type @c data_type by user-defined dimensions for 2-D
 *         data
 *
 * @ingroup mat_internal
 * @param mat MAT file pointer
 * @param z zlib compression stream
 * @param data Pointer to store the output data
 * @param class_type Type of data class (matio_classes enumerations)
 * @param data_type Datatype of the stored data (matio_types enumerations)
 * @param dims Dimensions of the data
 * @param start Index to start reading data in each dimension
 * @param stride Read every @c stride elements in each dimension
 * @param edge Number of elements to read in each dimension
 * @retval Number of bytes read from the file, or -1 on error
 */
int
ReadCompressedDataSlab2(mat_t *mat,z_streamp z,void *data,
    enum matio_classes class_type,enum matio_types data_type,size_t *dims,
    int *start,int *stride,int *edge)
{
    int nBytes = 0, i, j, err;
    int pos, row_stride, col_stride;
    z_stream z_copy = {0,};

    if ( (mat   == NULL) || (data   == NULL) || (mat->fp == NULL) ||
         (start == NULL) || (stride == NULL) || (edge    == NULL) ) {
        return 0;
    }

    err = inflateCopy(&z_copy,z);
    switch ( class_type ) {
        case MAT_C_DOUBLE:
        {
            double *ptr = (double*)data;
            READ_COMPRESSED_DATA_SLAB2(ReadCompressedDoubleData);
            break;
        }
        case MAT_C_SINGLE:
        {
            float *ptr = (float*)data;
            READ_COMPRESSED_DATA_SLAB2(ReadCompressedSingleData);
            break;
        }
#ifdef HAVE_MAT_INT64_T
        case MAT_C_INT64:
        {
            mat_int64_t *ptr = (mat_int64_t*)data;
            READ_COMPRESSED_DATA_SLAB2(ReadCompressedInt64Data);
            break;
        }
#endif /* HAVE_MAT_INT64_T */
#ifdef HAVE_MAT_UINT64_T
        case MAT_C_UINT64:
        {
            mat_uint64_t *ptr = (mat_uint64_t*)data;
            READ_COMPRESSED_DATA_SLAB2(ReadCompressedUInt64Data);
            break;
        }
#endif /* HAVE_MAT_UINT64_T */
        case MAT_C_INT32:
        {
            mat_int32_t *ptr = (mat_int32_t*)data;
            READ_COMPRESSED_DATA_SLAB2(ReadCompressedInt32Data);
            break;
        }
        case MAT_C_UINT32:
        {
            mat_uint32_t *ptr = (mat_uint32_t*)data;
            READ_COMPRESSED_DATA_SLAB2(ReadCompressedUInt32Data);
            break;
        }
        case MAT_C_INT16:
        {
            mat_int16_t *ptr = (mat_int16_t*)data;
            READ_COMPRESSED_DATA_SLAB2(ReadCompressedInt16Data);
            break;
        }
        case MAT_C_UINT16:
        {
            mat_uint16_t *ptr = (mat_uint16_t*)data;
            READ_COMPRESSED_DATA_SLAB2(ReadCompressedUInt16Data);
            break;
        }
        case MAT_C_INT8:
        {
            mat_int8_t *ptr = (mat_int8_t*)data;
            READ_COMPRESSED_DATA_SLAB2(ReadCompressedInt8Data);
            break;
        }
        case MAT_C_UINT8:
        {
            mat_uint8_t *ptr = (mat_uint8_t*)data;
            READ_COMPRESSED_DATA_SLAB2(ReadCompressedUInt8Data);
            break;
        }
        default:
            nBytes = 0;
    }
    inflateEnd(&z_copy);
    return nBytes;
}

#undef READ_COMPRESSED_DATA_SLAB2
#endif

/** @endcond */
