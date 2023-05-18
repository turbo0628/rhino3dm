#include "bindings.h"



BND_TUPLE BND_export_mesh_blender(std::vector<BND_Mesh> &mesh_list, double scale)
{
    std::vector<BND_TUPLE> faces;
    std::vector<BND_TUPLE> vertices;
    std::vector<BND_TUPLE> coords;

    size_t faces_count(0), vertices_count(0), coords_count(0);
    for (int i = 0; i < mesh_list.size(); ++i) {
        auto mesh = mesh_list[i];
        faces_count += mesh.GetFaces().Count();
        vertices_count += mesh.GetVertices().Count();
        coords_count += mesh.TextureCoordinates().Count();
    }

    faces.reserve(faces_count);
    vertices.reserve(vertices_count);
    coords.reserve(coords_count);

    int offset = 0;
    for (int i = 0; i < mesh_list.size(); ++i)
    {
        auto mesh = mesh_list[i];
        BND_MeshFaceList m_faces = mesh.GetFaces();
        for (int f = 0; f < m_faces.Count(); ++f)
        {
            BND_TUPLE face = m_faces.GetFaceForBlender(f, offset);
            faces.push_back(face);
        }

        auto m_vertices = mesh.GetVertices();
        offset += m_vertices.Count();
        for (int v = 0; v < m_vertices.Count(); ++v)
        {
            auto vertex = m_vertices.GetVertex(v);
            BND_TUPLE scaled_vertex = CreateTuple(3);
            SetTuple(scaled_vertex, 0, vertex.x * scale);
            SetTuple(scaled_vertex, 1, vertex.y * scale);
            SetTuple(scaled_vertex, 2, vertex.z * scale);

            vertices.push_back(scaled_vertex);
        }

        auto m_coords = mesh.TextureCoordinates();
        for (int t = 0; t < m_coords.Count(); ++t) {
            auto coord = m_coords.GetTextureCoordinate(t);
            BND_TUPLE out_coord = CreateTuple(2);
            SetTuple(out_coord, 0, coord.x);
            SetTuple(out_coord, 1, coord.y);

            coords.push_back(out_coord);
        }
    }

    BND_TUPLE ret_tuple;
    ret_tuple = CreateTuple(3);
    SetTuple<std::vector<BND_TUPLE>>(ret_tuple, 0, faces);
    SetTuple<std::vector<BND_TUPLE>>(ret_tuple, 1, vertices);
    SetTuple<std::vector<BND_TUPLE>>(ret_tuple, 2, coords);
    return ret_tuple;
}

#if defined(ON_PYTHON_COMPILE)
namespace py = pybind11;
void initBlenderExporterBindings(py::module &m)
{
    m.def("export_mesh_blender", &BND_export_mesh_blender, py::return_value_policy::move);
}
#endif