#include "DHMesh.h"

int main() {

	std::string filename = "test.off";
	std::ifstream infile(filename);
	std::string sline;
	std::getline(infile, sline);
	std::getline(infile, sline);

	DTMesh *mesh = new DTMesh;

	std::istringstream iss(sline);
	int vertexNum = 0, faceNum = 0;
	if (!(iss >> vertexNum >> faceNum))
		return 0;
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
			mesh->addVertex(v);
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
		mesh->addFace(face);
		face->id = i - 1;

		face->point_ids.resize(trinum);
		for (int k = 0; k < trinum; ++k)
		{
			istmline >> id0;
			face->point_ids[k] = id0;
		}
	}

	mesh->build();
	//mesh->printInfo();
	//std::vector<DTFace * > faces = mesh->findboundaryface();
	mesh->findboundarys();
	if (mesh) {
		delete mesh;
	}

	return 0;
}