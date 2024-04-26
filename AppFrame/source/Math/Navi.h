#include"../AppFrame/source/Math/NavigationMesh.h"

class Navi {
public:

	Navi();
	~Navi();

	void CreateNavigationMesh();

	bool FindPath(const Vector3& startPos, const Vector3& goalPos, std::vector<Vector3>& path, float radius = 0.f);

	int GetHandle()const { return _handle; }

	bool LoadModel(const char* fileName);

	std::vector<NavigationMesh>& GetNavMeshes() { return navMeshes; }

private:

	void GetPolygonData(int handle, std::vector<Polygon3D>& polygons);

	std::vector<NavigationMesh> navMeshes;

	int _handle;
};