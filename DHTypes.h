#ifndef _DHTYPES_HEADER_
#define _DHTYPES_HEADER_

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <assert.h>

class DTHalfEdge;
class DTFace;

/*!
 * \class DTVertex
 *
 * \brief simple vertex class (you can rewrite as template class )
 *
 * \author Damons
 * \date 十二月 2018
 */
class DTVertex {
public:
	//可以使用模板类
	using DataType = double;
public:
	int id;
	DataType x;
	DataType y;
	DataType z;
	DTHalfEdge * edge_out;// 从此点开始的所有的半边中的任意一个(从任意一个便可以遍历所有的边)

public:
	DTVertex() :x(0.0), y(0.0), z(0.0) {}
	DTVertex(DataType _x, DataType _y, DataType _z = 0.0) :x(_x), y(_y), z(_z) {}
	~DTVertex() { edge_out = nullptr; }

};
/*!
 * \class half-edge
 *
 * \brief half-edge struct
 *
 * \author Damons
 * \date 十二月 2018
 */
class DTHalfEdge
{
public:
	DTVertex	*start_vert; // 半边的开始的顶点
	DTHalfEdge	*pair;		 // 半边的对边（oppositely oriented adjacent half-edge ） 
	DTFace		*face;		 // 位于半边左边的面（face the half-edge borders)
	DTHalfEdge  *next;		 // 半边的下一条边（next half-edge around the face）
	DTHalfEdge  *prev;		 // 半边的上一条边（prev half-edge around the face）
public:
	DTHalfEdge() :start_vert(nullptr), pair(nullptr), face(nullptr), next(nullptr) {}
	~DTHalfEdge() { start_vert = nullptr; pair = nullptr; face = nullptr; next = nullptr; }
};

/*!
 * \class DTFace
 *
 * \brief mesh triangle face
 *
 * \author Damons
 * \date 十二月 2018
 */
class DTFace {
public:
	DTHalfEdge * edge;// one of the half-edges bordering the face
	unsigned int number_vertex;
	int id;

	std::vector<unsigned int > point_ids;
public:
	DTFace() :edge(nullptr) {
		number_vertex = 0; 
	}
	~DTFace() {
		edge = nullptr; 
		std::vector<unsigned int >().swap(point_ids);
	}
};

#endif// 2019/10/29