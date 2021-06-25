#include "..\..\..\Header\Graphics\Helper\MeshCreater.h"

void GatesEngine::MeshCreater::CreatePlane(Math::Vector2 size, Math::Vector2 uvMax, MeshData<VertexInfo::Vertex_UV_Normal>& meshData)
{
	std::vector<VertexInfo::Vertex_UV_Normal>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	using namespace Math;
	vertices->push_back({ Vector3(-size.x / 2.0f,0, size.y / 2.0f),Vector2(0,0),Vector3(0,1,0) });
	vertices->push_back({ Vector3(size.x / 2.0f,0, size.y / 2.0f),Vector2(uvMax.x,0),Vector3(0,1,0) });
	vertices->push_back({ Vector3(size.x / 2.0f,0,-size.y / 2.0f),Vector2(uvMax.x,uvMax.y),Vector3(0,1,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f,0,-size.y / 2.0f),Vector2(0,uvMax.y),Vector3(0,1,0) });

	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);
	indices->push_back(2);
	indices->push_back(3);
	indices->push_back(0);
}

void GatesEngine::MeshCreater::CreateQuad(Math::Vector2 size, Math::Vector2 uvMax, MeshData<VertexInfo::Vertex_UV_Normal>& meshData)
{
	std::vector<VertexInfo::Vertex_UV_Normal>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	using namespace Math;
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f,0),Vector2(0,0),Vector3(0,0,-1) });
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f,0),Vector2(uvMax.x,0),Vector3(0,0,-1) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f,0),Vector2(uvMax.x,uvMax.y),Vector3(0,0,0 - 1) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f,0),Vector2(0,uvMax.y),Vector3(0,0,-1) });

	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);
	indices->push_back(2);
	indices->push_back(3);
	indices->push_back(0);
}

void GatesEngine::MeshCreater::Create2DQuad(Math::Vector2 size, Math::Vector2 uvMax, MeshData<VertexInfo::Vertex_UV_Normal>& meshData)
{
	std::vector<VertexInfo::Vertex_UV_Normal>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	using namespace Math;
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f,0),Vector2(0,0),Vector3(0,0,-1) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f,0),Vector2(uvMax.x,0),Vector3(0,0,-1) });
	vertices->push_back({ Vector3(size.x / 2.0f,size.y / 2.0f,0),Vector2(uvMax.x,uvMax.y),Vector3(0,0,0 - 1) });
	vertices->push_back({ Vector3(-size.x / 2.0f,size.y / 2.0f,0),Vector2(0,uvMax.y),Vector3(0,0,-1) });

	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);
	indices->push_back(2);
	indices->push_back(3);
	indices->push_back(0);
}

void GatesEngine::MeshCreater::CreateGrid(Math::Vector2 size, float spaceInterval, MeshData<VertexInfo::Vertex_Color>& meshData)
{
	int width = (int)(size.x / spaceInterval);
	int depth = (int)(size.y / spaceInterval);

	std::vector<VertexInfo::Vertex_Color>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	Math::Vector2 offset = Math::Vector2(-size.x / 2, -size.y / 2);
	Math::Vector2 maxOffset = size / 2;

	for (int w = 0; w <= width; ++w)
	{
		float x = w * spaceInterval;
		GatesEngine::Math::Vector4 color = { 1,1,1,0.8f };
		if (offset.x + x == 0)color = { 0,0,1,1 };
		vertices->push_back({ Math::Vector3(offset.x + x,0,offset.y),color });
		vertices->push_back({ Math::Vector3(offset.x + x,0,maxOffset.y),color });
	}

	for (int d = 0; d <= depth; ++d)
	{
		float z = d * spaceInterval;
		GatesEngine::Math::Vector4 color = { 1,1,1,0.8f };
		if (offset.y + z == 0)color = { 1,0,0,1 };
		vertices->push_back({ Math::Vector3(offset.x,0   ,offset.y + z),color });
		vertices->push_back({ Math::Vector3(maxOffset.x,0,offset.y + z),color });
	}

	for (int i = 0; i < (int)vertices->size(); i += 2)
	{
		indices->push_back(i);
		indices->push_back(i + 1);
	}
}

void GatesEngine::MeshCreater::CreateCube(Math::Vector3 size, MeshData<VertexInfo::Vertex_UV_Normal>& meshData)
{
	using namespace GatesEngine::Math;
	std::vector<VertexInfo::Vertex_UV_Normal>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	//�O
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(0,0),Vector3(0,0,-1) });
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(0,0,-1) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),Vector2(1,1),Vector3(0,0,-1) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(0,0,-1) });
	//�E
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(0,0),Vector3(1,0,0) });
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(1,0,0) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),Vector2(1,1),Vector3(1,0,0) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(1,0,0) });
	//��
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(0,0),Vector3(-1,0,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(-1,0,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),Vector2(1,1),Vector3(-1,0,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(-1,0,0) });
	//��
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(0,0),Vector3(0,0,1) });
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(0,0,1) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),Vector2(1,1),Vector3(0,0,1) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(0,0,1) });
	//��
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(0,0),Vector3(0,1,0) });
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(0,1,0) });
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(1,1),Vector3(0,1,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(0,1,0) });
	//��
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),Vector2(0,0),Vector3(0,-1,0) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(0,-1,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),Vector2(1,1),Vector3(0,-1,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(0,-1,0) });

	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(2);
	indices->push_back(2);
	indices->push_back(3);
	indices->push_back(0);

	indices->push_back(4);
	indices->push_back(5);
	indices->push_back(6);
	indices->push_back(6);
	indices->push_back(7);
	indices->push_back(4);

	indices->push_back(8);
	indices->push_back(9);
	indices->push_back(10);
	indices->push_back(10);
	indices->push_back(11);
	indices->push_back(8);

	indices->push_back(12);
	indices->push_back(13);
	indices->push_back(14);
	indices->push_back(14);
	indices->push_back(15);
	indices->push_back(12);

	indices->push_back(16);
	indices->push_back(17);
	indices->push_back(18);
	indices->push_back(18);
	indices->push_back(19);
	indices->push_back(16);

	indices->push_back(20);
	indices->push_back(21);
	indices->push_back(22);
	indices->push_back(22);
	indices->push_back(23);
	indices->push_back(20);
}

void GatesEngine::MeshCreater::CreateSphere(Math::Vector3 size, int vTess, int hTess, MeshData<VertexInfo::Vertex_UV_Normal>& meshData)
{
	using namespace GatesEngine::Math;
	std::vector<VertexInfo::Vertex_UV_Normal>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	vertices->resize(vTess * (hTess + 1));
	for (int v = 0; v <= hTess; v++) 
	{
		for (int u = 0; u < vTess; u++) 
		{
			float theta = Math::ConvertToRadian(180.0f * v / hTess);
			float phi = Math::ConvertToRadian(360.0f * u / vTess);
			float x = sinf(theta) * cosf(phi);
			float c = cosf(theta);
			float y = cosf(theta);
			float z = sinf(theta) * sinf(phi);
			(*vertices)[vTess * v + u].point = { x, y, z, };
		}
	}


	//int maxIndex = vTess * hTess * 6;
	//for (int i = 0; i < maxIndex / 6; ++i)
	//{
	//	indices->push_back(i);
	//	indices->push_back(i + 1);
	//	indices->push_back(i + 12);
	//	indices->push_back(i + 12);
	//	indices->push_back(i + 11);
	//	indices->push_back(i);
	//}

	int i = 0;
	indices->resize(2 * hTess * (vTess + 1));

	for (int v = 0; v < hTess; v++) {
		for (int u = 0; u <= vTess; u++) {
			if (u == vTess) {
				(*indices)[i] = v * vTess;
				++i;
				(*indices)[i] = (v + 1) * vTess;
				++i;
			}
			else {
				(*indices)[i] = (v * vTess) + u;
				++i;
				(*indices)[i] = (*indices)[i - 1] + vTess;
				++i;
			}
		}
	}
}
