#include "student_code.h"
#include "mutablePriorityQueue.h"

using namespace std;

namespace CGL
{

  /**
   * Evaluates one step of the de Casteljau's algorithm using the given points and
   * the scalar parameter t (class member).
   *
   * @param points A vector of points in 2D
   * @return A vector containing intermediate points or the final interpolated vector
   */
  std::vector<Vector2D> BezierCurve::evaluateStep(std::vector<Vector2D> const &points)
  { 
    // TODO Part 1.
    if (points.size() == 1) { return points; }
    
    std::vector<Vector2D> r;
    for (int i = 0; i < points.size()-1; i++) {
        r.push_back((1 - t) * points[i] + t * points[i + 1]);
    }
    
    //return evaluateStep(r);
    return r;
  }

  /**
   * Evaluates one step of the de Casteljau's algorithm using the given points and
   * the scalar parameter t (function parameter).
   *
   * @param points    A vector of points in 3D
   * @param t         Scalar interpolation parameter
   * @return A vector containing intermediate points or the final interpolated vector
   */
  std::vector<Vector3D> BezierPatch::evaluateStep(std::vector<Vector3D> const &points, double t) const
  {
    // TODO Part 2.
    if (points.size() == 1) { return points; }
    
    std::vector<Vector3D> r;
    for (int i = 0; i < points.size()-1; i++) {
        r.push_back((1 - t) * points[i] + t * points[i + 1]);
    }
    
    return r;
  }

  /**
   * Fully evaluates de Casteljau's algorithm for a vector of points at scalar parameter t
   *
   * @param points    A vector of points in 3D
   * @param t         Scalar interpolation parameter
   * @return Final interpolated vector
   */
  Vector3D BezierPatch::evaluate1D(std::vector<Vector3D> const &points, double t) const
  {
    // TODO Part 2.
    if (points.size() == 1) { return points[0]; }

    return evaluate1D(evaluateStep(points, t), t);
  }

  /**
   * Evaluates the Bezier patch at parameter (u, v)
   *
   * @param u         Scalar interpolation parameter
   * @param v         Scalar interpolation parameter (along the other axis)
   * @return Final interpolated vector
   */
  Vector3D BezierPatch::evaluate(double u, double v) const 
  {  
    // TODO Part 2.
    std::vector<Vector3D> r;
    for (int i = 0; i < controlPoints.size(); i++) {
        r.push_back(evaluate1D(controlPoints[i], u));
    }
    
    return evaluate1D(r, v);
  }

  Vector3D Vertex::normal( void ) const
  {
    // TODO Part 3.
    // Returns an approximate unit normal at this vertex, computed by
    // taking the area-weighted average of the normals of neighboring
    // triangles, then normalizing.
      Vector3D N(0,0,0);
      HalfedgeCIter h = halfedge();
      
      do {
          Vector3D pi = h->next()->vertex()->position;
          Vector3D pj = h->next()->next()->vertex()->position;
          Vector3D a = cross(pi-position, pj-position);
          N += (a.norm()/2) * h->face()->normal();
          h = h->twin()->next();
      } while (!h->face()->isBoundary() and h != halfedge());
      
      return N.unit();
  }

  EdgeIter HalfedgeMesh::flipEdge( EdgeIter e0 )
  {
    // TODO Part 4.
    // This method should flip the given edge and return an iterator to the flipped edge.
      if (e0->isBoundary()) { return EdgeIter(); }
      
      else {
          //before
          HalfedgeIter h0 = e0->halfedge();
          HalfedgeIter h1 = h0->next();
          HalfedgeIter h2 = h1->next();
          HalfedgeIter h3 = h0->twin();
          HalfedgeIter h4 = h3->next();
          HalfedgeIter h5 = h4->next();
          HalfedgeIter h6 = h1->twin();
          HalfedgeIter h7 = h2->twin();
          HalfedgeIter h8 = h4->twin();
          HalfedgeIter h9 = h5->twin();
          VertexIter vold1 = h0->vertex();
          VertexIter vold2 = h3->vertex();
          VertexIter vnew1 = h2->vertex();
          VertexIter vnew2 = h5->vertex();
          EdgeIter e1 = h1->edge();
          EdgeIter e2 = h2->edge();
          EdgeIter e3 = h4->edge();
          EdgeIter e4 = h5->edge();
          FaceIter f0 = h0->face();
          FaceIter f1 = h3->face();
          
          //after
          h0->setNeighbors(h1,h3,vnew2,e0,f0);
          h1->setNeighbors(h2,h7,vnew1,e2,f0);
          h2->setNeighbors(h0,h8,vold1,e3,f0);
          h3->setNeighbors(h4,h0,vnew1,e0,f1);
          h4->setNeighbors(h5,h9,vnew2,e4,f1);
          h5->setNeighbors(h3,h6,vold2,e1,f1);
          h6->setNeighbors(h6->next(),h5,vnew1,e1,h6->face());
          h7->setNeighbors(h7->next(),h1,vold1,e2,h7->face());
          h8->setNeighbors(h8->next(),h2,vnew2,e3,h8->face());
          h9->setNeighbors(h9->next(),h4,vold2,e4,h9->face());
          vold1->halfedge() = h2;
          vold2->halfedge() = h5;
          vnew1->halfedge() = h3;
          vnew2->halfedge() = h0;
          e0->halfedge() = h0;
          e1->halfedge() = h5;
          e2->halfedge() = h1;
          e3->halfedge() = h2;
          e4->halfedge() = h4;
          f0->halfedge() = h0;
          f1->halfedge() = h3;
      }
    
    return e0;
  }

  VertexIter HalfedgeMesh::splitEdge( EdgeIter e0 )
  {
    // TODO Part 5.
    // This method should split the given edge and return an iterator to the newly inserted vertex.
    // The halfedge of this vertex should point along the edge that was split, rather than the new edges.
      VertexIter v = newVertex();
      
      if (e0->isBoundary()) {return v;}
      
      else {
          //before
          HalfedgeIter h0 = e0->halfedge();
          HalfedgeIter h1 = h0->next();
          HalfedgeIter h2 = h1->next();
          HalfedgeIter h3 = h0->twin();
          HalfedgeIter h4 = h3->next();
          HalfedgeIter h5 = h4->next();
          HalfedgeIter h6 = h1->twin();
          HalfedgeIter h7 = h2->twin();
          HalfedgeIter h8 = h4->twin();
          HalfedgeIter h9 = h5->twin();
          HalfedgeIter h10 = newHalfedge();
          HalfedgeIter h11 = newHalfedge();
          HalfedgeIter h12 = newHalfedge();
          HalfedgeIter h13 = newHalfedge();
          HalfedgeIter h14 = newHalfedge();
          HalfedgeIter h15 = newHalfedge();
          VertexIter vold1 = h0->vertex();
          VertexIter vold2 = h3->vertex();
          VertexIter vnew1 = h2->vertex();
          VertexIter vnew2 = h5->vertex();
          EdgeIter e1 = h1->edge();
          EdgeIter e2 = h2->edge();
          EdgeIter e3 = h4->edge();
          EdgeIter e4 = h5->edge();
          EdgeIter e5 = newEdge();
          EdgeIter e6 = newEdge();
          EdgeIter e7 = newEdge();
          FaceIter f0 = h0->face();
          FaceIter f1 = h3->face();
          FaceIter f2 = newFace();
          FaceIter f3 = newFace();
          
          //after
          v->position = (vold1->position + vold2->position) / 2;
          h0->setNeighbors(h1,h3,v,e0,f0);
          h1->setNeighbors(h10,h11,vold2,e1,f0);
          h2->setNeighbors(h15,h7,vnew1,e2,f2);
          h3->setNeighbors(h12,h0,vold2,e0,f1);
          h4->setNeighbors(h13,h8,vold1,e3,f3);
          h5->setNeighbors(h3,h9,vnew2,e4,f1);
          h6->setNeighbors(h6->next(),h1,vnew1,e1,h6->face());
          h7->setNeighbors(h7->next(),h2,vold1,e2,h7->face());
          h8->setNeighbors(h8->next(),h4,vnew2,e3,h8->face());
          h9->setNeighbors(h9->next(),h5,vold2,e4,h9->face());
          h10->setNeighbors(h0,h11,vnew1,e5,f0);
          h11->setNeighbors(h2,h10,v,e5,f2);
          h12->setNeighbors(h5,h13,v,e6,f1);
          h13->setNeighbors(h14,h12,vnew2,e6,f3);
          h14->setNeighbors(h4,h15,v,e7,f3);
          h15->setNeighbors(h11,h14,vold1,e7,f2);
          v->halfedge() = h0;
          vold1->halfedge() = h15;
          vold2->halfedge() = h3;
          vnew1->halfedge() = h10;
          vnew2->halfedge() = h13;
          e0->halfedge() = h0;
          e1->halfedge() = h1;
          e2->halfedge() = h2;
          e3->halfedge() = h4;
          e4->halfedge() = h5;
          e5->halfedge() = h10;
          e6->halfedge() = h13;
          e7->halfedge() = h14;
          f0->halfedge() = h10;
          f1->halfedge() = h12;
          f2->halfedge() = h11;
          f3->halfedge() = h13;
          v->isNew = true;
          e5->isNew = true;
          e6->isNew = true;
          e7->isNew = true;
      }
    return v;
  }



  void MeshResampler::upsample( HalfedgeMesh& mesh )
  {
      // TODO Part 6.
      // This routine should increase the number of triangles in the mesh using Loop subdivision.
      // One possible solution is to break up the method as listed below.
      
      // 1. Compute new positions for all the vertices in the input mesh, using the Loop subdivision rule, and store them in Vertex::newPosition. At this point, we also want to mark each vertex as being a vertex of the original mesh.
      for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
          Vector3D original_neighbor_position_sum(0,0,0);
          HalfedgeIter h = v->halfedge();
          do {
              Vector3D p = h->twin()->vertex()->position;
              original_neighbor_position_sum += p;
              h = h->twin()->next();
          } while (h != v->halfedge());
          
          float n = v->degree();
          float u = (n == 3) ? (3/16) : (3 / (8*n));
          v->isNew = false;
          v->newPosition = (1 - n * u) * v->position + u * original_neighbor_position_sum;
      }
      
      // 2. Compute the updated vertex positions associated with edges, and store it in Edge::newPosition.
      for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
          VertexIter v1 = e->halfedge()->vertex();
          VertexIter v2 = e->halfedge()->twin()->vertex();
          VertexIter v3 = e->halfedge()->next()->next()->vertex();
          VertexIter v4 = e->halfedge()->twin()->next()->next()->vertex();
          e->newPosition = (3/8) * (v1->position + v2->position) + (1/8) * (v3->position + v4->position);
          e->isNew = false;
      }
      
      // 3. Split every edge in the mesh, in any order. For future reference, we're also going to store some information about which subdivide edges come from splitting an edge in the original mesh, and which edges are new, by setting the flat Edge::isNew. Note that in this loop, we only want to iterate over edges of the original mesh---otherwise, we'll end up splitting edges that we just split (and the loop will never end!)
      for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
          HalfedgeIter h = e->halfedge();
          if(!h->vertex()->isNew || !h->twin()->vertex()->isNew) {
              VertexIter v = mesh.splitEdge(e);
              v->newPosition = e->newPosition;
          }
      }

      // 4. Flip any new edge that connects an old and new vertex.
      for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++) {
          HalfedgeIter h = e->halfedge();
          VertexIter v1 = h->vertex();
          VertexIter v2 = h->twin()->vertex();
          
          if (e->isNew == true) {
              if ((v1->isNew == true) && (v2->isNew == false)) {
                  mesh.flipEdge(e);
              } else if ((v1->isNew == false) && (v2->isNew == true)) {
                  mesh.flipEdge(e);
              }
          }
      }
      
      // 5. Copy the new vertex positions into final Vertex::position.
      for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++) {
          v->position = v->newPosition;
      }
  }
}
