#ifndef _Eigen_Eigen_h
#define _Eigen_Eigen_h

#include <Core/Core.h>

#define EIGEN_MATRIX_PLUGIN <plugin/Eigen/ToStringPlugin.h>
#define EIGEN_DENSEBASE_PLUGIN <plugin/Eigen/ToStringPlugin.h>

#ifndef flagSSE2
 #define EIGEN_DONT_VECTORIZE
 #define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#endif

#define EIGEN_NO_DEBUG

#include "Eigen/Eigen"

#endif
