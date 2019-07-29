/*
	convert 2D to 3D model
*/

#pragma once
#include <vector>

#include "GUA_OM.h"
#include "TriangulationCgal.h"

class Mesh
{
	public:
		Mesh(Triangles triangles);
		~Mesh();
		void DelaunayMeshToOpenMesh(Triangles triangles);
		void ConnectVertices();
		void Smooth();

	int SearchVertex(Vector2 pData)

	private:
		Tri_Mesh *m_Mesh;
		std::vector<Vector2> m_Vertices;
		std::vector<OMT::VertexHandle> m_FrontPoints;
		std::vector<OMT::VertexHandle> m_BackPoints;
};