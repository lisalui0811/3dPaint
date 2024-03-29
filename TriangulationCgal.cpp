#define BOOST_ALL_DYN_LINK
#include <cgal_all_files.cpp>//DotNetBAR ��
#include "TriangulationCgal.h"
#include <auto_link_mpir.hpp>
#include <auto_link_mpfr.hpp>
#include <boost/chrono/time_point.hpp>
#include <iostream>
#include<cmath>
TriangulationCgal::TriangulationCgal(void)
{
}

TriangulationCgal::~TriangulationCgal(void)
{
}

void TriangulationCgal::AddPoint( float x, float y )
{
	Vertex_handle vp =m_Triangulation.insert(Point(x, y));
	ContourPoint.push_back(vp);
}

void TriangulationCgal::Compute()
{
	std::cout << "Compute Triangulation" << std::endl;
	m_Triangles.clear();
	Triangle t;
	Vector2 centrol;
	Finite_faces_iterator fc = m_Triangulation.finite_faces_begin();
	for( ; fc != m_Triangulation.finite_faces_end(); ++fc)
	{
		t.m_Points[0] = Vector2(fc->vertex(0)->point()[0], fc->vertex(0)->point()[1]);
		t.m_Points[1] = Vector2(fc->vertex(1)->point()[0], fc->vertex(1)->point()[1]);
		t.m_Points[2] = Vector2(fc->vertex(2)->point()[0], fc->vertex(2)->point()[1]);
		m_Triangles.push_back(t);
	}

}
void TriangulationCgal::DelaunayMesher2()
{
	for(int i=0;i< ContourPoint.size();i++)
	{
		m_Triangulation.insert_constraint(ContourPoint[i],ContourPoint[(i+1) % ContourPoint.size()]);
	}

	Mesher mesher(m_Triangulation);
	mesher.refine_mesh();

	mesher.set_criteria(Criteria(0.125,25));
	mesher.refine_mesh();

	std::cout << "Number of vertices: " << m_Triangulation.number_of_vertices() << std::endl;
	std::cout << "Number of finite faces: " << m_Triangulation.number_of_faces() << std::endl;

	Triangle t;
	m_Triangles.clear();
	Finite_faces_iterator fc = m_Triangulation.finite_faces_begin();
	for( ; fc != m_Triangulation.finite_faces_end(); ++fc)
	{
		t.m_Points[0] = Vector2((int)fc->vertex(0)->point()[0], (int)fc->vertex(0)->point()[1]);
		t.m_Points[1] = Vector2((int)fc->vertex(1)->point()[0], (int)fc->vertex(1)->point()[1]);
		t.m_Points[2] = Vector2((int)fc->vertex(2)->point()[0], (int)fc->vertex(2)->point()[1]);
		m_Triangles.push_back(t);
	}
}

Vector2s TriangulationCgal::MeshPointSet()
{
	Vector2s out;
	Finite_vertices_iterator p= m_Triangulation.finite_vertices_begin();
	for(;p != m_Triangulation.finite_vertices_end(); ++p)
	{
		out.push_back(Vector2((int)p->point()[0],(int)p->point()[1]));
	}
	return out;
}

int TriangulationCgal::getVertexID(float x, float y)
{	
	Finite_vertices_iterator p= m_Triangulation.finite_vertices_begin();
	int ID=0;
	for(;p != m_Triangulation.finite_vertices_end(); ++p)
	{
		if((int)p->point()[0]==x && (int)p->point()[1]==y)
			return ID;
		ID++;
	}
	std::cout<<"not found!!";
}