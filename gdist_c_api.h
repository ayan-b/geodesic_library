#include <iostream>
#include <fstream>
#include <inttypes.h>

#include "geodesic_algorithm_exact.h"


#if defined(_WIN32)
#  if defined(DLL_EXPORTS)
#    define DLL_EXPORT_API __declspec(dllexport)
#  else
#    define DLL_EXPORT_API __declspec(dllimport)
#  endif
#else
#  define DLL_EXPORT_API
#endif


double* local_gdist_matrix_impl(
    unsigned number_of_vertices,
    unsigned number_of_triangles,
    double *vertices,
    unsigned *triangles,
    unsigned *sparse_matrix_size,
    double max_distance
);

void free_memory_impl(double *ptr);

extern "C" {
    DLL_EXPORT_API double* local_gdist_matrix(
        unsigned number_of_vertices,
        unsigned number_of_triangles,
        double *vertices,
        unsigned *triangles,
        unsigned *sparse_matrix_size,
        double max_distance
    );

    DLL_EXPORT_API void free_memory(double *ptr);
};
