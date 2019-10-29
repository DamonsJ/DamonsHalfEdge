#include "DHMesh.h"

void DTMesh::build() {
	for (auto &face : m_faces)
	{
		int trinum = face->point_ids.size();
		std::vector<unsigned int > indexs = face->point_ids;
		indexs.push_back(indexs[0]);

		std::vector<DTHalfEdge * > curedges;
		curedges.resize(trinum + 1);

		for (int k = 0; k < trinum; ++k)
		{
			unsigned int f = indexs[k];
			unsigned int s = indexs[k + 1];

			DTVertex *fv = getVertex(f);
			DTVertex *sv = getVertex(s);
			//whether this half-edge exist
			int hf_index_f = findHalfEdge(f, s);
			DTHalfEdge *es = hf_index_f > -1 ? m_halfedges[hf_index_f]:nullptr;
			bool isesNew = hf_index_f != -1 ? false : true;
			int hf_index_s = findHalfEdge(s, f);
			DTHalfEdge *ep = hf_index_s > -1 ? m_halfedges[hf_index_s] : nullptr;//es's pair edge
			bool isepNew = hf_index_s != -1 ? false : true;
			// create new if half-edge does not exist
			if (isesNew) {
				es = new DTHalfEdge;
				addHalfEdge(es, f, s);
			}
			if (isepNew) {
				ep = new DTHalfEdge;
				addHalfEdge(ep, s, f);
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
				assert(0);
				//this should not happen
				std::cout << "should never happen : isesNew && !isepNew" << std::endl;
				es->face = face;
				es->pair = ep;
				es->start_vert = fv;

				ep->pair = es;
				//assert(ep->start_vert == sv);
			}
			else if (!isesNew && isepNew) {
				assert(0);
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
		std::vector<unsigned int >().swap(indexs);
		std::vector<DTHalfEdge * >().swap(curedges);
	}
}