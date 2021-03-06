#include "gdist_c_api.h"


double* local_gdist_matrix_impl(
    unsigned number_of_vertices,
    unsigned number_of_triangles,
    double *vertices,
    unsigned *triangles,
    unsigned *sparse_matrix_size,
    double max_distance
) {
    std::vector<double> points (vertices, vertices + number_of_vertices);
    std::vector<unsigned> faces (triangles, triangles + number_of_triangles);
    
    geodesic::Mesh mesh;
    mesh.initialize_mesh_data(points, faces); // create internal mesh data structure including edges
    geodesic::GeodesicAlgorithmExact algorithm(&mesh); // create exact algorithm for the mesh
    std::vector <unsigned> rows_vector, columns_vector;
    std::vector <double> data_vector;

    double distance = 0;

    std::vector<geodesic::SurfacePoint> targets(number_of_vertices), source;

    for (unsigned i = 0; i < number_of_vertices; ++i) {
        targets[i] = geodesic::SurfacePoint(&mesh.vertices()[i]);
    }
    for (unsigned i = 0; i < number_of_vertices / 3; ++i) {
        source.push_back(geodesic::SurfacePoint(&mesh.vertices()[i]));
        algorithm.propagate(source, max_distance, NULL);
        source.pop_back();
        for (unsigned j = 0; j < number_of_vertices / 3; ++j) {
            algorithm.best_source(targets[j], distance);
            if (distance != geodesic::GEODESIC_INF && distance != 0 && distance <= max_distance) {
                rows_vector.push_back(i);
                columns_vector.push_back(j);
                data_vector.push_back(distance);
            }
        }
    }

    double *data;
    data = new double[3 * rows_vector.size()];
    assert (data != NULL); // memory allocation should not fail
    *sparse_matrix_size = rows_vector.size();

    std::copy(rows_vector.begin(), rows_vector.end(), data);
    std::copy(columns_vector.begin(), columns_vector.end(), data + data_vector.size());
    std::copy(data_vector.begin(), data_vector.end(), data + 2 * data_vector.size());

    return data;
}


void free_memory_impl(double *ptr) {
    delete[] ptr;
}


extern "C" {
    double* local_gdist_matrix(
        unsigned number_of_vertices,
        unsigned number_of_triangles,
        double *vertices,
        unsigned *triangles,
        unsigned *sparse_matrix_size,
        double max_distance
    ) {
        return local_gdist_matrix_impl(
            number_of_vertices,
            number_of_triangles,
            vertices,
            triangles,
            sparse_matrix_size,
            max_distance
        );
    }

    void free_memory(double *ptr) {
        free_memory_impl(ptr);
    }
};
