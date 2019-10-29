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
 * \date ʮ���� 2018
 */
class DTVertex {
public:
	//����ʹ��ģ����
	using DataType = double;
public:
	int id;
	DataType x;
	DataType y;
	DataType z;
	DTHalfEdge * edge_out;// �Ӵ˵㿪ʼ�����еİ���е�����һ��(������һ������Ա������еı�)

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
 * \date ʮ���� 2018
 */
class DTHalfEdge
{
public:
	DTVertex	*start_vert; // ��ߵĿ�ʼ�Ķ���
	DTHalfEdge	*pair;		 // ��ߵĶԱߣ�oppositely oriented adjacent half-edge �� 
	DTFace		*face;		 // λ�ڰ����ߵ��棨face the half-edge borders)
	DTHalfEdge  *next;		 // ��ߵ���һ���ߣ�next half-edge around the face��
	DTHalfEdge  *prev;		 // ��ߵ���һ���ߣ�prev half-edge around the face��
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
 * \date ʮ���� 2018
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