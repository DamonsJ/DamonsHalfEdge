#ifndef _DHMESH_HEADER_
#define _DHMESH_HEADER_

#include "DHTypes.h"

/*!
 * \class DTMesh
 *
 * \brief simple mesh structure that store as half-edge structure
 *        edges store by std::map< std::pair<DTVertex *, DTVertex *>, DTHalfEdge* >
 *		  for easily find
 * \author Damons
 * \date ʮ���� 2018
 */
class DTMesh
{
public:
	using index_type = unsigned int;
	using data_type = float;

public:
	DTMesh() {}
	~DTMesh() { destroy(); }

public:
	// add vertex
	void addVertex(DTVertex* v) {
		m_vertexs.push_back(v);
	}
	// add face to mesh
	void addFace(DTFace* f) {
		m_faces.push_back(f);
	}

	void build();
	
	// get vertex by index
	DTVertex * getVertex(int ind) {
		return m_vertexs[ind];
	}
	// get number of vetex
	int getnumberVertex() { return m_vertexs.size(); }
	// get face by index
	DTFace * getFace(int ind) {
		return m_faces[ind];
	}
	// get number of faces
	int getnumberFaces() { return m_faces.size(); }

	void sortvertex() {
		auto cmp = [](const DTVertex *a, const DTVertex *b)->int {
			if (a->x < b->x)
				return -1;
			else if (a->x > b->x)
				return 1;
			else if (a->y < b->y)
				return -1;
			else if (a->y > b->y)
				return 1;
			//assert(0 && "2 or more points share the same exact coordinate");
			return 0; /* Should not be given! */
		};
		std::sort(m_vertexs.begin(), m_vertexs.end(), cmp);
		for (int i = 0; i < m_vertexs.size(); ++i)
		{
			m_vertexs[i]->id = i;
		}
	}
	//////////////////////////////////////////////////////////////////////////
public:
	//************************************  
	// @brief : find all the face that a vertex adjacent 
	// @author: SunHongLei
	// @date  : 2018/12/06  
	// @return: void
	// @param[in] : int v_ind: the vertex which should be searched
	// @param[out] : std::vector<DTFace *> : all the faces found
	//************************************ 
	void all_faces_adjacent_vertex(const int v_ind, std::vector<DTFace *> &leaving_face) const{
		std::vector<DTHalfEdge *> leaving_list;
		all_half_edges_leaving(v_ind, leaving_list);
		for (auto &he : leaving_list)
		{
			if (he->face) {
				leaving_face.push_back(he->face);
			}
		}
		std::vector<DTHalfEdge *>().swap(leaving_list);
	}
	std::vector<DTFace *> all_faces_adjacent_vertex(const int v_ind) const {

		std::vector<DTFace *> leaving_face;
		std::vector<DTHalfEdge *> leaving_list;
		all_half_edges_leaving(v_ind, leaving_list);
		for (auto &he : leaving_list)
		{
			if (he->face) {
				leaving_face.push_back(he->face);
			}
		}
		std::vector<DTHalfEdge *>().swap(leaving_list);

		return leaving_face;
	}

	//************************************  
	// @brief : find all the half-edge that leaving a vertex
	// @author: SunHongLei
	// @date  : 2018/12/06  
	// @return: void
	// @param[in] : int v_ind: the vertex which should be searched
	// @param[out] : std::vector<DTHalfEdge *> : all the half-edges found 
	//************************************ 
	void all_half_edges_leaving(int v_ind, std::vector<DTHalfEdge *> &leaving_list) const
	{
		DTVertex *v = m_vertexs[v_ind];
		DTHalfEdge *h = v->edge_out;
		leaving_list.push_back(h);
		DTHalfEdge *w = h->pair->next;
		while (w && w != h)
		{
			leaving_list.push_back(w);
			w = w->pair->next;
		}
	}
	//************************************  
	// @brief : find all the face's half-edge 
	// @author: SunHongLei
	// @date  : 2018/12/06  
	// @return: void
	// @param[in] : int f_ind: the face which should be searched
	// @param[out] : std::vector<DTHalfEdge *> : all the half-edges found  
	//************************************ 
	void all_half_edges_face(int f_ind, std::vector<DTHalfEdge *> &edges) {
		DTFace *face = m_faces[f_ind];
		DTHalfEdge *he = face->edge;
		DTHalfEdge *t = he;
		do {
			edges.push_back(t);
			t = t->next;
		} while (t != he);
	}
	//************************************  
	// @brief : find all the face's adjoin with given face
	// @author: SunHongLei
	// @date  : 2018/12/06  
	// @return: void
	// @param[in] : int f_ind: the face which should be searched
	// @param[out] : std::vector<DTFace *> : all the faces found  
	//************************************ 
	void all_faces_adjacent(int f_id, std::vector<DTFace *> &faces) {
		DTFace *face = m_faces[f_id];
		DTHalfEdge *he = face->edge;
		DTHalfEdge *t = he;
		do {
			DTHalfEdge *p = t->pair;
			if (p->face)
				faces.push_back(p->face);
			t = t->next;
		} while (t != he);
	}
	//************************************  
	// @brief : find all the vertex in the face
	// @author: SunHongLei
	// @date  : 2018/12/06  
	// @return: void
	// @param[in] : int f_ind: the face which should be searched
	// @param[out] : std::vector<DTVertex *> : all the vertex that found 
	//************************************ 
	void all_vertex_face(int f_id, std::vector<DTVertex *> &v) {
		DTFace *face = m_faces[f_id];
		DTHalfEdge *he = face->edge;
		DTHalfEdge *t = he;
		do {
			v.push_back(t->start_vert);
			t = t->next;
		} while (t != he);
	}
	
	std::vector<DTFace * > findboundaryface() {
		std::vector<DTFace * > faces;
		for (auto &hf : m_halfedges)
		{
			if (hf->face == nullptr) {
				faces.push_back(hf->pair->face);
			}
		}
		return faces;
	}

	std::vector<DTVertex * > findboundarypoints() {
		std::vector<DTVertex * > points;
		for (auto &hf : m_halfedges)
		{
			if (hf->face == nullptr) {
				points.push_back(hf->start_vert);
			}
		}
		return points;
	}

	void findboundarys() {
		std::set<DTFace* > faces;
		std::map<index_type, DTHalfEdge*> boundarys_points;
		std::vector<DTHalfEdge* > curhalfedges;
		for (auto &hf : m_halfedges)
		{
			if (hf->face == nullptr) {
				//points.push_back(hf->start_vert);
				faces.insert(hf->pair->face);
				boundarys_points[index_type(hf->start_vert->id)] = hf;
				curhalfedges.push_back(hf);
			}
		}

		/*std::ofstream fstrm("BOUND.off");
		fstrm << "OFF" << std::endl;
		fstrm << getnumberVertex() << " " << faces.size() << "  0" << std::endl;

		for (auto &ptr : m_vertexs)
		{
			fstrm << ptr->x << " " << ptr->y << " " << ptr->z << std::endl;
		}

		for (auto &ptr : faces)
		{
			fstrm << ptr->point_ids.size();
			for (auto &vt : ptr->point_ids)
			{
				fstrm << "  " << vt;
			}
			fstrm << std::endl;
		}
		fstrm.close();*/

		faces.clear();

		int sz = curhalfedges.size();
		std::vector< std::vector<DTVertex* > > all_boundarys;
		std::vector<DTVertex* > boundarys;
		DTHalfEdge* curedge = curhalfedges[0];
		boundarys.push_back(curedge->start_vert);
		while (boundarys_points.size())
		{	
			if (boundarys.size() == 1 || (boundarys.size() > 1 && boundarys[0] != boundarys.back()))
			{
				index_type id = curedge->pair->start_vert->id;
				if (boundarys_points.find(id) != boundarys_points.end()) {
					curedge = boundarys_points[id];
					boundarys.push_back(curedge->start_vert);
					boundarys_points.erase(id);
				}
			}
			else if (boundarys[0] == boundarys.back()) {
			// another boundary
				all_boundarys.push_back(boundarys);
				boundarys.clear();
				curedge = boundarys_points.begin()->second;
				boundarys.push_back(curedge->start_vert);
			}
		};

		if(boundarys.size())
			all_boundarys.push_back(boundarys);

	}

public:
	void printFaceInfo(DTFace *_face) {
		if (!_face) {
			std::cout << "null face" << std::endl;
			return;
		}
		DTHalfEdge *he = _face->edge;
		DTHalfEdge *t = he;
		do {
			std::cout << " point: " << t->start_vert->x << "  " << t->start_vert->y << "  " << t->start_vert->z << std::endl;
			t = t->next;
		} while (t != he);
	}

	void printInfo() {
		std::cout << "vertex info : " << std::endl;
		for (auto &ptr : m_vertexs)
		{
			std::cout <<"\t" << ptr->id << " " <<ptr->x << " " << ptr->y << " " << ptr->z << std::endl;
		}

		std::cout << "faces info : " << std::endl;
		for (auto &ptr :m_faces)
		{
			std::cout << "\t" << ptr->id << " " << ptr->point_ids[0] << " " << ptr->point_ids[1] << " " << ptr->point_ids[2] << std::endl;
		}

		std::cout << "halfedges info : " << std::endl;
		for (auto &ptr : m_halfedges)
		{
			std::cout << "\t" << ptr->start_vert->id << " "<<(ptr->pair == nullptr ? -1 : ptr->pair->start_vert->id);
			std::cout << " " << (ptr->face == nullptr ? -1 : ptr->face->id) << " " << (ptr->pair == nullptr ? -1 : ptr->pair->start_vert->id) << " " << std::endl;
		}

	}
protected:
		
		void addHalfEdge(DTHalfEdge* e, index_type v1, index_type v2) {
			m_halfedges.push_back(e);
			std::pair<index_type, index_type> p = std::make_pair(v1, v2);
			m_edgePairs_cache[p] = m_halfedges.size() - 1;
		}

		int findHalfEdge(index_type v1, index_type v2) {
			std::pair<index_type, index_type> p = std::make_pair(v1, v2);
			if (m_edgePairs_cache.find(p) != m_edgePairs_cache.end()) {
				index_type hf_index = m_edgePairs_cache[p];
				return hf_index;
			}
			return -1;
		}
public:
	//************************************  
	// @brief : out put this mesh to off 
	// @author: SunHongLei
	// @date  : 2018/12/06  
	// @return: void
	// @param : void  
	//************************************ 
	void write_as_off(std::string filename) {
		std::ofstream fstrm(filename);
		fstrm << "OFF" << std::endl;
		fstrm << getnumberVertex() << " " << getnumberFaces() << "  0" << std::endl;

		for (auto &ptr : m_vertexs)
		{
			fstrm << ptr->x << " " << ptr->y << " " << ptr->z << std::endl;
		}

		std::vector<DTVertex *> v;
		for (auto &ptr : m_faces)
		{
			all_vertex_face(ptr->id, v);
			fstrm << v.size();
			for (auto &vt : v)
			{
				fstrm << "  " << vt->id;
			}
			fstrm << std::endl;
			v.clear();
		}
		std::vector<DTVertex *>().swap(v);
		fstrm.close();
	}
	//************************************  
	// @brief : read off file 
	// @author: SunHongLei
	// @date  : 2018/12/06  
	// @return: void
	// @param : void  
	//************************************ 
	void read_off(std::string filename) {
		destroy();

		std::ifstream infile(filename);
		std::string sline;
		std::getline(infile, sline);
		std::getline(infile, sline);

		std::istringstream iss(sline);
		int vertexNum = 0, faceNum = 0;
		if (!(iss >> vertexNum >> faceNum))
			return;
		//read vertex and store in mesh
		int i = 0;
		double x = 0.0, y = 0.0, z = 0.0;
		while (i < vertexNum) {
			std::getline(infile, sline);
			if (sline.size() < 1)
				continue;
			++i;
			std::istringstream istmline(sline);
			if (istmline >> x >> y >> z)
			{
				DTVertex *v = new DTVertex(x, y, z);
				v->id = i - 1;
				addVertex(v);
			}
		}
		i = 0;
		while (i < faceNum)
		{
			std::getline(infile, sline);
			if (sline.size() < 1)
				continue;

			++i;
			int id0 = 0, id1 = 0, trinum = 0;
			std::istringstream istmline(sline);
			istmline >> trinum;
			//create new face
			DTFace *face = new DTFace;
			addFace(face);
			face->id = i - 1;
			//vertex index
			std::vector<int > indexs;
			indexs.resize(trinum + 1);
			for (int k = 0; k < trinum; ++k)
			{
				istmline >> id0;
				indexs[k] = id0;
			}
			indexs[trinum] = indexs[0];

			std::vector<DTHalfEdge * > curedges;
			curedges.resize(trinum + 1);
			for (int k = 0; k < trinum; ++k)
			{
				int f = indexs[k];
				int s = indexs[k + 1];

				DTVertex *fv = getVertex(f);
				DTVertex *sv = getVertex(s);
				//whether this half-edge exist
				int hf_index_f = findHalfEdge(f, s);
				DTHalfEdge *es = hf_index_f > -1 ? m_halfedges[hf_index_f] : nullptr;
				bool isesNew = hf_index_f != -1 ? false : true;
				int hf_index_s = findHalfEdge(s, f);
				DTHalfEdge *ep = hf_index_s > -1 ? m_halfedges[hf_index_s] : nullptr;//es's pair edge
				bool isepNew = hf_index_s != -1 ? false : true;
				// create new if half-edge does not exist
				if (isesNew) {
					es = new DTHalfEdge;
					addHalfEdge(es, hf_index_f, hf_index_s);
				}
				if (isepNew) {
					ep = new DTHalfEdge;
					addHalfEdge(ep, hf_index_s, hf_index_f);
				}
				//set vertex leaving edge
				fv->edge_out = es;
				sv->edge_out = ep;
				//set face edge
				face->edge = es;
				curedges[k] = es;
				if (isesNew && isepNew) {
					//set half-edge properties
					es->face = face;
					es->pair = ep;
					es->start_vert = fv;

					ep->pair = es;
					ep->start_vert = sv;
				}
				else if (isesNew && !isepNew) {
					//this should not happen
					std::cout << "should never happen : isesNew && !isepNew" << std::endl;
					es->face = face;
					es->pair = ep;
					es->start_vert = fv;

					ep->pair = es;
					//assert(ep->start_vert == sv);
				}
				else if (!isesNew && isepNew) {
					//this should not happen
					std::cout << "should never happen : !isesNew && isepNew" << std::endl;
					es->face = face;
					es->pair = ep;
					es->start_vert = fv;

					ep->pair = es;
					ep->start_vert = sv;
				}
				else {
					es->face = face;
				}
			}
			curedges[trinum] = curedges[0];
			// set this half-edge's next and prev edge
			for (int k = 0; k < trinum; ++k)
			{
				curedges[k]->next = curedges[k + 1];
				curedges[trinum - k]->prev = curedges[trinum - k - 1];
			}
			std::vector<int >().swap(indexs);
			std::vector<DTHalfEdge * >().swap(curedges);
		}
	}
protected:
	// destroy memory
	void destroy() {
		for (auto &ptr : m_vertexs)
		{
			delete ptr;
			ptr = nullptr;
		}
		std::vector<DTVertex *>().swap(m_vertexs);

		for (auto &ptr : m_faces)
		{
			delete ptr;
			ptr = nullptr;
		}
		std::vector<DTFace *>().swap(m_faces);

		for (auto &ptr : m_halfedges)
		{
			delete ptr;
			ptr = nullptr;
		}
		std::vector<DTHalfEdge *>().swap(m_halfedges);

		//for (auto &ptr : m_edgePairs_cache)
		//{
		//	delete ptr.second;
		//	ptr.second = nullptr;
		//}
		//m_edgePairs_cache.clear();
	}
protected:
	// all the vertex
	std::vector<DTVertex *> m_vertexs;
	//all the faces
	std::vector<DTFace *> m_faces;
	//all the edges 
	std::vector<DTHalfEdge *> m_halfedges;
	using halfedge_pairs = std::map< std::pair<index_type, index_type>, index_type >;
	halfedge_pairs m_edgePairs_cache;
};
#endif// 2019/10/29