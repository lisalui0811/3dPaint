#include "Mesh.h"
Mesh::Mesh(Triangles triangles)
{
  DelaunayMeshToOpenMesh(triangles);
}

Mesh::~Mesh()
{
  delete m_Mesh;
}

void Mesh::DelaunayMeshToOpenMesh(Triangles triangles)
{
  std::vector<OMT::VertexHandle> vecFaceVH;
  std::vector<OMT::VertexHandle> vecVH;
  OMT::Points p;
  int idx[3];

  double modelcenterX = 0;
  double thick = 0;
  double modelcenterY = 0;
  double thickY = 0;

  if (m_Mesh != NULL)
    delete m_Mesh;
  m_Mesh = new Tri_Mesh();
  m_Mesh->Add_property();

  vecVH.clear();
  m_Vertices.clear();
  //================center===================
  for (int i = 0; i < triangles.size(); i++)
  {
    for (int j = 0; j < 3; j++)
    {
      Vector2 point2D = triangles[i].m_Points[j];
      if (SearchVertex(point2D) == -1)
      {
        idx[j] = vecVH.size();
        modelcenterX += point2D[0];
        modelcenterY += point2D[1];
      }
      else
        idx[j] = SearchVertex(point2D);
    }
  }

  modelcenterX /= m_Vertices.size();
  modelcenterY /= m_Vertices.size();

  for (int i = 0; i < m_Vertices.size(); i++)
  {
    if (abs(m_Vertices[i][0] - modelcenterX) > thick)
      thick = abs(m_Vertices[i][0] - modelcenterX);
    if (abs(m_Vertices[i][1] - modelcenterY) > thickY)
      thickY = abs(m_Vertices[i][1] - modelcenterY);
  }
  cout << modelcenterX << endl;

  //=================backface=========================
  vecVH.clear();
  m_Vertices.clear();
  for (int i = 0; i < triangles.size(); i++)
  {
    for (int j = 0; j < 3; j++)
    {
      Vector2 point2D = triangles[i].m_Points[j];
      if (SearchVertex(point2D) == -1)
      {
        idx[j] = vecVH.size();
        vecVH.push_back(m_Mesh->add_vertex(OMT::Point((point2D[0] - (hkoglPanelControl1->Width) / 2) / 500, ((hkoglPanelControl1->Height) / 2 - point2D[1]) / 500,
        -abs(point2D[0] - modelcenterX) / 2000 - abs(point2D[1] - modelcenterY) / 5000)));
      }
      else
        idx[j] = SearchVertex(point2D);
    }
    vecFaceVH.clear();
    vecFaceVH.push_back(vecVH[idx[0]]);
    vecFaceVH.push_back(vecVH[idx[1]]);
    vecFaceVH.push_back(vecVH[idx[2]]);
    m_Mesh->add_face(vecFaceVH);
  }

  //=====================frontface===================
  vecVH.clear();
  m_Vertices.clear();
  for (int i = 0; i < triangles.size(); i++)
  {
      for (int j = 0; j < 3; j++)
      {
        Vector2 point2D = triangles[i].m_Points[j];
        if (SearchVertex(point2D) == -1)
        {
          idx[j] = vecVH.size();
          vecVH.push_back(m_Mesh->add_vertex(OMT::Point((point2D[0] - (hkoglPanelControl1->Width) / 2) / 500, ((hkoglPanelControl1->Height) / 2 - point2D[1]) / 500,
          abs(point2D[0] - modelcenterX) / thick / 10 + abs(point2D[1] - modelcenterY) / thickY / 100)));
        }
        else
          idx[j] = SearchVertex(point2D);
    }
    vecFaceVH.clear();
    vecFaceVH.push_back(vecVH[idx[0]]);
    vecFaceVH.push_back(vecVH[idx[2]]);
    vecFaceVH.push_back(vecVH[idx[1]]);
    m_Mesh->add_face(vecFaceVH);
  }
}

void Mesh::ConnectVertices()
{
  OMT::VIter v_it_f, v_it_b;
  OMT::Point temp_pt;

  for (v_it_b = m_Mesh->vertices_begin(); v_it_b != m_Mesh->vertices_end(); v_it_b++)
  {
    if (m_Mesh->is_boundary(v_it_b))
    {
      temp_pt = m_Mesh->point(v_it_b.handle());
      v_it_f = v_it_b;
      break;
    }
  }

  for (v_it_b = m_Mesh->vertices_begin(); v_it_b != m_Mesh->vertices_end(); v_it_b++)
  {
    if (m_Mesh->point(v_it_b.handle()).data()[0] == temp_pt[0] && m_Mesh->point(v_it_b.handle()).data()[1] == temp_pt[1] && m_Mesh->point(v_it_b.handle()).data()[2] != temp_pt[2])
    break;
  }

  OMT::HEHandle heh_f, heh_init_f, heh_b, heh_init_b;
  heh_f = heh_init_f = m_Mesh->halfedge_handle(v_it_f.handle());
  heh_b = m_Mesh->halfedge_handle(v_it_b.handle());
  heh_b = heh_init_b = m_Mesh->prev_halfedge_handle(heh_b);

  m_FrontPoints.clear();
  m_BackPoints.clear();
  do {
    m_FrontPoints.push_back(m_Mesh->from_vertex_handle(heh_f));
    m_BackPoints.push_back(m_Mesh->from_vertex_handle(heh_b));

    heh_f = m_Mesh->next_halfedge_handle(heh_f);
    heh_b = m_Mesh->prev_halfedge_handle(heh_b);
  } while (heh_f != heh_init_f);

  std::vector<OMT::VertexHandle> vecFaceVH;
  for (int i = 0; i < m_FrontPoints.size(); i++)
  {
    vecFaceVH.clear();
    vecFaceVH.push_back(m_FrontPoints[i]);
    vecFaceVH.push_back(m_FrontPoints[(i + 1) % m_FrontPoints.size()]);
    vecFaceVH.push_back(m_BackPoints[i]);
    m_Mesh->add_face(vecFaceVH);

    vecFaceVH.clear();
    vecFaceVH.push_back(m_BackPoints[i]);
    vecFaceVH.push_back(m_FrontPoints[(i + 1) % m_FrontPoints.size()]);
    vecFaceVH.push_back(m_BackPoints[(i + 1) % m_FrontPoints.size()]);
    m_Mesh->add_face(vecFaceVH);
  }
}

void Mesh::Smooth()
{
  int v_num = m_Mesh->n_vertices();

  Eigen::SparseMatrix<double> A(v_num + 2 * m_FrontPoints.size(), v_num);
  std::vector<Eigen::VectorXd> B;
  B.resize(3);
  B[0].resize(v_num + 2 * m_FrontPoints.size());
  B[1].resize(v_num + 2 * m_FrontPoints.size());
  B[2].resize(v_num + 2 * m_FrontPoints.size());

  for (int i = 0; i < v_num; i++)
  {
    B[0][i] = 0;
    B[1][i] = 0;
    B[2][i] = 0;
  }

  for (OMT::VIter v_it = m_Mesh->vertices_begin(); v_it != m_Mesh->vertices_end(); v_it++)
  {
    OMT::VVIter vv_it;
    int oring_pnum = 0;
    for (vv_it = m_Mesh->vv_begin(v_it); vv_it != m_Mesh->vv_end(v_it); vv_it++)
      oring_pnum++;

    for (vv_it = m_Mesh->vv_begin(v_it); vv_it != m_Mesh->vv_end(v_it); vv_it++)
      A.insert(v_it.handle().idx(), vv_it.handle().idx()) = -1.0 / oring_pnum;
    A.insert(v_it.handle().idx(), v_it.handle().idx()) = 1;
  }
  for (int i = 0; i < m_FrontPoints.size(); i++)
  {
    A.insert(i + v_num, m_FrontPoints[i].idx()) = 1;
    OMT::Point temp_p = m_Mesh->point(m_FrontPoints[i]);
    B[0][i + v_num] = temp_p.data()[0];
    B[1][i + v_num] = temp_p.data()[1];
    B[2][i + v_num] = temp_p.data()[2];
  }
  for (int i = 0; i < m_BackPoints.size(); i++)
  {
    A.insert(i + v_num + m_FrontPoints.size(), m_BackPoints[i].idx()) = 1;
    OMT::Point temp_p = m_Mesh->point(m_BackPoints[i]);
    B[0][i + v_num + m_FrontPoints.size()] = temp_p.data()[0];
    B[1][i + v_num + m_FrontPoints.size()] = temp_p.data()[1];
    B[2][i + v_num + m_FrontPoints.size()] = temp_p.data()[2];
  }

  A.makeCompressed();

  SparseQR<SparseMatrix<double>, COLAMDOrdering<int>> linearSolver;
  linearSolver.compute(A);

  std::vector<Eigen::VectorXd> X;
  X.resize(3);
  X[0] = linearSolver.solve(B[0]);
  X[1] = linearSolver.solve(B[1]);
  X[2] = linearSolver.solve(B[2]);

  for (OMT::VIter v_it = m_Mesh->vertices_begin(); v_it != m_Mesh->vertices_end(); v_it++)
  {
    m_Mesh->point(v_it).data()[0] = X[0][v_it.handle().idx()];
    m_Mesh->point(v_it).data()[1] = X[1][v_it.handle().idx()];
    m_Mesh->point(v_it).data()[2] = X[2][v_it.handle().idx()];
  }
}

int Mesh::SearchVertex(Vector2 pData)
{
  for (int i = 0; i < m_Vertices.size(); i++)
    if (m_Vertices[i][0] == pData[0] && m_Vertices[i][1] == pData[1])
      return i;
  m_Vertices.push_back(pData);
  return -1;
}