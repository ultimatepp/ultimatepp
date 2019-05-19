/*
 * Copyright (c) 2008-2019, Christopher C. Hulbert
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

#ifndef MATIO_PRIVATE_H
#define MATIO_PRIVATE_H

#include "matioConfig.h"
#include "matio.h"
#if defined(HAVE_ZLIB)
#   include <plugin/z/lib/zlib.h>
#endif
#if defined(MAT73) && MAT73
#   include <hdf5.h>
#endif

#ifndef EXTERN
#   ifdef __cplusplus
#       define EXTERN extern "C"
#   else
#       define EXTERN extern
#   endif
#endif

#if defined(HAVE_ZLIB) && HAVE_ZLIB
#   define ZLIB_BYTE_PTR(a) ((Bytef *)(a))
#endif

/** @if mat_devman
 * @brief Matlab MAT File information
 *
 * Contains information about a Matlab MAT file
 * @ingroup mat_internal
 * @endif
 */
struct _mat_t {
    void  *fp;              /**< File pointer for the MAT file */
    char  *header;          /**< MAT file header string */
    char  *subsys_offset;   /**< Offset */
    char  *filename;        /**< Filename of the MAT file */
    int    version;         /**< MAT file version */
    int    byteswap;        /**< 1 if byte swapping is required, 0 otherwise */
    int    mode;            /**< Access mode */
    long   bof;             /**< Beginning of file not including any header */
    size_t next_index;      /**< Index/File position of next variable to read */
    size_t num_datasets;    /**< Number of datasets in the file */
#if defined(MAT73) && MAT73
    hid_t  refs_id;         /**< Id of the /#refs# group in HDF5 */
#endif
    char **dir;             /**< Names of the datasets in the file */
};

/** @if mat_devman
 * @brief internal structure for MAT variables
 * @ingroup mat_internal
 * @endif
 */
struct matvar_internal {
#if defined(MAT73) && MAT73
    char      *hdf5_name;   /**< Name */
    hobj_ref_t hdf5_ref;    /**< Reference */
    hid_t      id;          /**< Id */
#endif
    long       datapos;     /**< Offset from the beginning of the MAT file to the data */
    unsigned   num_fields;  /**< Number of fields */
    char     **fieldnames;  /**< Pointer to fieldnames */
#if defined(HAVE_ZLIB)
    z_streamp  z;           /**< zlib compression state */
    void      *data;        /**< Inflated data array */
#endif
};

/* snprintf.c */
#if !HAVE_VSNPRINTF
int rpl_vsnprintf(char *, size_t, const char *, va_list);
#define mat_vsnprintf rpl_vsnprintf
#else
#define mat_vsnprintf vsnprintf
#endif /* !HAVE_VSNPRINTF */
#if !HAVE_SNPRINTF
int rpl_snprintf(char *, size_t, const char *, ...);
#define mat_snprintf rpl_snprintf
#else
#define mat_snprintf snprintf
#endif /* !HAVE_SNPRINTF */
#if !HAVE_VASPRINTF
int rpl_vasprintf(char **, const char *, va_list);
#define mat_vasprintf rpl_vasprintf
#else
#define mat_vasprintf vasprintf
#endif /* !HAVE_VASPRINTF */
#if !HAVE_ASPRINTF
int rpl_asprintf(char **, const char *, ...);
#define mat_asprintf rpl_asprintf
#else
#define mat_asprintf asprintf
#endif /* !HAVE_ASPRINTF */

/* endian.c */
EXTERN double        Mat_doubleSwap(double  *a);
EXTERN float         Mat_floatSwap(float   *a);
#ifdef HAVE_MAT_INT64_T
EXTERN mat_int64_t   Mat_int64Swap(mat_int64_t  *a);
#endif /* HAVE_MAT_INT64_T */
#ifdef HAVE_MAT_UINT64_T
EXTERN mat_uint64_t  Mat_uint64Swap(mat_uint64_t *a);
#endif /* HAVE_MAT_UINT64_T */
EXTERN mat_int32_t   Mat_int32Swap(mat_int32_t  *a);
EXTERN mat_uint32_t  Mat_uint32Swap(mat_uint32_t *a);
EXTERN mat_int16_t   Mat_int16Swap(mat_int16_t  *a);
EXTERN mat_uint16_t  Mat_uint16Swap(mat_uint16_t *a);

/* read_data.c */
EXTERN int ReadDoubleData(mat_t *mat,double  *data,enum matio_types data_type,
               int len);
EXTERN int ReadSingleData(mat_t *mat,float   *data,enum matio_types data_type,
               int len);
#ifdef HAVE_MAT_INT64_T
EXTERN int ReadInt64Data (mat_t *mat,mat_int64_t *data,
               enum matio_types data_type,int len);
#endif /* HAVE_MAT_INT64_T */
#ifdef HAVE_MAT_UINT64_T
EXTERN int ReadUInt64Data(mat_t *mat,mat_uint64_t *data,
               enum matio_types data_type,int len);
#endif /* HAVE_MAT_UINT64_T */
EXTERN int ReadInt32Data (mat_t *mat,mat_int32_t *data,
               enum matio_types data_type,int len);
EXTERN int ReadUInt32Data(mat_t *mat,mat_uint32_t *data,
               enum matio_types data_type,int len);
EXTERN int ReadInt16Data (mat_t *mat,mat_int16_t *data,
               enum matio_types data_type,int len);
EXTERN int ReadUInt16Data(mat_t *mat,mat_uint16_t *data,
               enum matio_types data_type,int len);
EXTERN int ReadInt8Data  (mat_t *mat,mat_int8_t  *data,
               enum matio_types data_type,int len);
EXTERN int ReadUInt8Data (mat_t *mat,mat_uint8_t  *data,
               enum matio_types data_type,int len);
EXTERN int ReadCharData  (mat_t *mat,char  *data,enum matio_types data_type,
               int len);
EXTERN int ReadDataSlab1(mat_t *mat,void *data,enum matio_classes class_type,
               enum matio_types data_type,int start,int stride,int edge);
EXTERN int ReadDataSlab2(mat_t *mat,void *data,enum matio_classes class_type,
               enum matio_types data_type,size_t *dims,int *start,int *stride,
               int *edge);
EXTERN int ReadDataSlabN(mat_t *mat,void *data,enum matio_classes class_type,
               enum matio_types data_type,int rank,size_t *dims,int *start,
               int *stride,int *edge);
#if defined(HAVE_ZLIB)
EXTERN int ReadCompressedDoubleData(mat_t *mat,z_streamp z,double  *data,
               enum matio_types data_type,int len);
EXTERN int ReadCompressedSingleData(mat_t *mat,z_streamp z,float   *data,
               enum matio_types data_type,int len);
#ifdef HAVE_MAT_INT64_T
EXTERN int ReadCompressedInt64Data(mat_t *mat,z_streamp z,mat_int64_t *data,
               enum matio_types data_type,int len);
#endif /* HAVE_MAT_INT64_T */
#ifdef HAVE_MAT_UINT64_T
EXTERN int ReadCompressedUInt64Data(mat_t *mat,z_streamp z,mat_uint64_t *data,
               enum matio_types data_type,int len);
#endif /* HAVE_MAT_UINT64_T */
EXTERN int ReadCompressedInt32Data(mat_t *mat,z_streamp z,mat_int32_t *data,
               enum matio_types data_type,int len);
EXTERN int ReadCompressedUInt32Data(mat_t *mat,z_streamp z,mat_uint32_t *data,
               enum matio_types data_type,int len);
EXTERN int ReadCompressedInt16Data(mat_t *mat,z_streamp z,mat_int16_t *data,
               enum matio_types data_type,int len);
EXTERN int ReadCompressedUInt16Data(mat_t *mat,z_streamp z,mat_uint16_t *data,
               enum matio_types data_type,int len);
EXTERN int ReadCompressedInt8Data(mat_t *mat,z_streamp z,mat_int8_t  *data,
               enum matio_types data_type,int len);
EXTERN int ReadCompressedUInt8Data(mat_t *mat,z_streamp z,mat_uint8_t  *data,
               enum matio_types data_type,int len);
EXTERN int ReadCompressedCharData(mat_t *mat,z_streamp z,char *data,
               enum matio_types data_type,int len);
EXTERN int ReadCompressedDataSlab1(mat_t *mat,z_streamp z,void *data,
               enum matio_classes class_type,enum matio_types data_type,
               int start,int stride,int edge);
EXTERN int ReadCompressedDataSlab2(mat_t *mat,z_streamp z,void *data,
               enum matio_classes class_type,enum matio_types data_type,
               size_t *dims,int *start,int *stride,int *edge);
EXTERN int ReadCompressedDataSlabN(mat_t *mat,z_streamp z,void *data,
               enum matio_classes class_type,enum matio_types data_type,
               int rank,size_t *dims,int *start,int *stride,int *edge);

/* inflate.c */
EXTERN size_t InflateSkip(mat_t *mat, z_streamp z, int nbytes);
EXTERN size_t InflateSkip2(mat_t *mat, matvar_t *matvar, int nbytes);
EXTERN size_t InflateSkipData(mat_t *mat,z_streamp z,enum matio_types data_type,int len);
EXTERN size_t InflateVarTag(mat_t *mat, matvar_t *matvar, void *buf);
EXTERN size_t InflateArrayFlags(mat_t *mat, matvar_t *matvar, void *buf);
EXTERN size_t InflateRankDims(mat_t *mat, matvar_t *matvar, void *buf, size_t nbytes, mat_uint32_t** dims);
EXTERN size_t InflateVarNameTag(mat_t *mat, matvar_t *matvar, void *buf);
EXTERN size_t InflateVarName(mat_t *mat,matvar_t *matvar,void *buf,int N);
EXTERN size_t InflateDataTag(mat_t *mat, matvar_t *matvar, void *buf);
EXTERN size_t InflateDataType(mat_t *mat, z_stream *z, void *buf);
EXTERN size_t InflateData(mat_t *mat, z_streamp z, void *buf, unsigned int nBytes);
EXTERN size_t InflateFieldNameLength(mat_t *mat,matvar_t *matvar,void *buf);
EXTERN size_t InflateFieldNamesTag(mat_t *mat,matvar_t *matvar,void *buf);
EXTERN size_t InflateFieldNames(mat_t *mat,matvar_t *matvar,void *buf,int nfields,
               int fieldname_length,int padding);
#endif

/* mat.c */
EXTERN mat_complex_split_t *ComplexMalloc(size_t nbytes);
EXTERN enum matio_types ClassType2DataType(enum matio_classes class_type);
EXTERN int SafeMul(size_t* res, size_t a, size_t b);
EXTERN int SafeMulDims(const matvar_t *matvar, size_t* nelems);

#endif
