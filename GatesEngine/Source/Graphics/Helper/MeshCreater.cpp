#include "..\..\..\Header\Graphics\Helper\MeshCreater.h"
#include <fstream>
#include <sstream>
#pragma comment(lib,"winmm.lib")

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

void GatesEngine::MeshCreater::CreateLineCube(Math::Vector3 size, const Math::Vector4& color, MeshData<VertexInfo::Vertex_Color>& meshData)
{
	using namespace GatesEngine::Math;
	std::vector<VertexInfo::Vertex_Color>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	Vector4 white = color;
	//前
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),white });
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f), white });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f), white });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),white });
	//奥
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f), white });
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),white });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),white });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f), white });

	indices->push_back(0);
	indices->push_back(1);
	indices->push_back(1);
	indices->push_back(2);
	indices->push_back(2);
	indices->push_back(3);

	indices->push_back(1);
	indices->push_back(4);
	indices->push_back(4);
	indices->push_back(7);
	indices->push_back(7);
	indices->push_back(2);

	indices->push_back(4);
	indices->push_back(5);
	indices->push_back(5);
	indices->push_back(6);
	indices->push_back(6);
	indices->push_back(7);

	indices->push_back(5);
	indices->push_back(0);
	indices->push_back(0);
	indices->push_back(3);
	indices->push_back(3);
	indices->push_back(6);
}

void GatesEngine::MeshCreater::CreateLineCircle(Math::Vector3 size, float vertexCount, const Math::Vector4& color, MeshData<VertexInfo::Vertex_Color>& meshData)
{
	using namespace GatesEngine::Math;
	std::vector<VertexInfo::Vertex_Color>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	for (int i = 0; i < vertexCount; ++i)
	{
		Math::Vector3 pos;
		pos.x = size.x * sinf((2 * GatesEngine::Math::PI / vertexCount) * i)/2;
		pos.y = 0;
		pos.z = size.x * cosf((2 * GatesEngine::Math::PI / vertexCount) * i)/2;
		//transform->position.x = r * sinf((2 * GatesEngine::Math::PI / 360) * time*10 * delay / 10);
		//transform->position.z = r * cosf((2 * GatesEngine::Math::PI / 360) * time*10 * delay / 10);
		vertices->push_back({ pos,color });

		indices->push_back(i);
		int index = (i >= vertexCount - 1) ? 0 : i + 1;
		indices->push_back(index);
	}


}

void GatesEngine::MeshCreater::CreateCube(Math::Vector3 size, MeshData<VertexInfo::Vertex_UV_Normal>& meshData)
{
	using namespace GatesEngine::Math;
	std::vector<VertexInfo::Vertex_UV_Normal>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	//前
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(0,0),Vector3(0,0,-1) });
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(0,0,-1) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),Vector2(1,1),Vector3(0,0,-1) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(0,0,-1) });
	//右
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(0,0),Vector3(1,0,0) });
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(1,0,0) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),Vector2(1,1),Vector3(1,0,0) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(1,0,0) });
	//左
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(0,0),Vector3(-1,0,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(-1,0,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f,-size.z / 2.0f),Vector2(1,1),Vector3(-1,0,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(-1,0,0) });
	//奥
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(0,0),Vector3(0,0,1) });
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(0,0,1) });
	vertices->push_back({ Vector3(-size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),Vector2(1,1),Vector3(0,0,1) });
	vertices->push_back({ Vector3(size.x / 2.0f,-size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(0,0,1) });
	//上
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(0,0),Vector3(0,1,0) });
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f),Vector2(1,0),Vector3(0,1,0) });
	vertices->push_back({ Vector3(size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(1,1),Vector3(0,1,0) });
	vertices->push_back({ Vector3(-size.x / 2.0f, size.y / 2.0f,-size.z / 2.0f),Vector2(1,0),Vector3(0,1,0) });
	//下
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
	//using namespace GatesEngine::Math;
	//std::vector<VertexInfo::Vertex_UV_Normal>* vertices = meshData.GetVertices();
	//std::vector<unsigned short>* indices = meshData.GetIndices();

	//vertices->resize((uint64_t)vTess * ((uint64_t)hTess + 1));
	//for (int v = 0; v <= hTess; v++)
	//{
	//	for (int u = 0; u < vTess; u++)
	//	{
	//		float theta = Math::ConvertToRadian(180.0f * v / hTess);
	//		float phi = Math::ConvertToRadian(360.0f * u / vTess);
	//		float x = sinf(theta) * cosf(phi);
	//		float c = cosf(theta);
	//		float y = cosf(theta);
	//		float z = sinf(theta) * sinf(phi);
	//		(*vertices)[(uint64_t)vTess * v + u].point = { x, y, z, };
	//	}
	//}


	////int maxIndex = vTess * hTess * 6;
	////for (int i = 0; i < maxIndex / 6; ++i)
	////{
	////	indices->push_back(i);
	////	indices->push_back(i + 1);
	////	indices->push_back(i + 12);
	////	indices->push_back(i + 12);
	////	indices->push_back(i + 11);
	////	indices->push_back(i);
	////}

	//int i = 0;
	//indices->resize((uint64_t)2 * hTess * ((uint64_t)vTess + 1));

	//for (int v = 0; v < hTess; v++) {
	//	for (int u = 0; u <= vTess; u++) {
	//		if (u == vTess) {
	//			(*indices)[i] = v * vTess;
	//			++i;
	//			(*indices)[i] = (v + 1) * vTess;
	//			++i;
	//		}
	//		else {
	//			(*indices)[i] = (v * vTess) + u;
	//			++i;
	//			(*indices)[i] = (*indices)[(uint64_t)i - 1] + vTess;
	//			++i;
	//		}
	//	}
	//}
	using namespace GatesEngine::Math;
	std::vector<VertexInfo::Vertex_UV_Normal>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	vertices->resize((uint64_t)vTess * ((uint64_t)hTess + 1));
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
			(*vertices)[(uint64_t)vTess * v + u].point = { x/2, y/2, z/2 };
		}
	}

	//縦列
	for (int i = 0; i < hTess; ++i)
	{
		if (i >= hTess)continue;
		//横列
		for (int j = 0; j < vTess; ++j)
		{
			if (j == vTess - 1)
			{
				int offset = i * vTess;
				int jPlusOffset = j + offset;

				indices->push_back(jPlusOffset);

				int jPlusMinusVTessMinusOne = jPlusOffset - (vTess - 1);
				indices->push_back(jPlusMinusVTessMinusOne);

				int jPlusOne = jPlusOffset + 1;
				indices->push_back(jPlusOne);

				Math::Vector3 normal, ba, cb;
				ba = (*vertices)[jPlusMinusVTessMinusOne].point - (*vertices)[jPlusOffset].point;
				cb = (*vertices)[jPlusOne].point - (*vertices)[jPlusMinusVTessMinusOne].point;
				normal = Math::Vector3::Cross(ba, cb);
				(*vertices)[jPlusOffset].normal = normal;
				(*vertices)[jPlusMinusVTessMinusOne].normal = normal;
				(*vertices)[jPlusOne].normal = normal;

				indices->push_back(jPlusOne);

				int jPlusVTessMinusOne = jPlusOffset + vTess;
				indices->push_back(jPlusVTessMinusOne);

				indices->push_back(jPlusOffset);

				ba = (*vertices)[jPlusVTessMinusOne].point - (*vertices)[jPlusOne].point;
				cb = (*vertices)[jPlusOffset].point - (*vertices)[jPlusVTessMinusOne].point;
				normal = Math::Vector3::Cross(ba, cb);
				normal = normal.Normalize();
				(*vertices)[jPlusOne].normal = normal;
				(*vertices)[jPlusVTessMinusOne].normal = normal;
				(*vertices)[jPlusOffset].normal = normal;

				continue;
			}

			int offset = i * vTess;
			int jPlusOffset = j + offset;

			indices->push_back(jPlusOffset);

			int jPlusOne = jPlusOffset + 1;
			indices->push_back(jPlusOne);

			int jPlusVTess = jPlusOffset + vTess + 1;
			indices->push_back(jPlusVTess);

			Math::Vector3 normal, ba, cb;
			ba = (*vertices)[jPlusOne].point - (*vertices)[jPlusOffset].point;
			cb = (*vertices)[jPlusVTess].point - (*vertices)[jPlusOffset].point;
			normal = Math::Vector3::Cross(ba, cb);
			(*vertices)[jPlusOffset].normal = normal;
			(*vertices)[jPlusOne].normal = normal;
			(*vertices)[jPlusVTess].normal = normal;


			indices->push_back(jPlusVTess);

			int jPlusVTessMinusOne = jPlusVTess - 1;
			indices->push_back(jPlusVTessMinusOne);

			indices->push_back(jPlusOffset);

			ba = (*vertices)[jPlusVTessMinusOne].point - (*vertices)[jPlusVTess].point;
			cb = (*vertices)[jPlusOffset].point - (*vertices)[jPlusVTessMinusOne].point;
			normal = Math::Vector3::Cross(ba, cb);
			normal = normal.Normalize();

			(*vertices)[jPlusVTess].normal = normal;
			(*vertices)[jPlusVTessMinusOne].normal = normal;
			(*vertices)[jPlusOffset].normal = normal;
		}
	}
}

void GatesEngine::MeshCreater::LoadModelData(const std::string& filename, MeshData<VertexInfo::Vertex_UV_Normal>& meshData)
{
	std::vector<VertexInfo::Vertex_UV_Normal>* vertices = meshData.GetVertices();
	std::vector<unsigned short>* indices = meshData.GetIndices();

	std::string  filepath = "Resources/Model/" + filename + ".obj";
	std::vector<Math::Vector3> positions;
	std::vector<Math::Vector2> uvies;
	std::vector<Math::Vector3> normals;
	int startIndex = 0;

	std::ifstream file;
	file.open(filepath);
	if (file.fail())return;
	std::string line;
	while (std::getline(file, line))    //ファイルから1行づつ読み込む
	{
		std::istringstream line_stream(line);    //getlineで読み込めるようにstringからstreamに変換する
		std::string key;
		std::getline(line_stream, key, ' ');    //line_streamから空白文字が出力されるまでkeyに入力する
		//if (key == "mtllib")
		//{
		//	std::string materialFilename;
		//	line_stream >> materialFilename;
		//	LoadMaterialData(directoryPath, materialFilename, outputMeshData.materialData);
		//}
		if (key == "v")    //頂点座標の読み込み
		{
			Math::Vector3 pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
			//positions.push_back(pos);    //単なるfloatなどならpush_backでもOK
			positions.emplace_back(pos);    //クラスや構造体の場合emplace_backのほうが早い
		}
		if (key == "vt")    //テクスチャ座標の読み込み
		{
			Math::Vector2 uv;
			line_stream >> uv.x;
			line_stream >> uv.y;
			uvies.emplace_back(uv);
		}
		if (key == "vn")    //法線情報の読み込み
		{
			Math::Vector3 normal;
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			normals.emplace_back(normal);
		}
		if (key == "f")    //v,vt,vnの組み合わせを生成
		{
			std::string index_word;
			std::string index;
			int number;
			std::vector<int> localIndices;
			int vCount = 0;
			while (std::getline(line_stream, index_word, ' '))
			{
				std::istringstream indexWord_stream(index_word);
				while (std::getline(indexWord_stream, index, '/'))
				{
					std::istringstream index_stream(index);
					index_stream >> number;
					localIndices.push_back(number - 1);
				}
				++vCount;
			}

			//頂点情報を組み合わせる
			for (int i = 0; i < (int)localIndices.size(); i += 3)
			{
				int v, vt, vn;
				v = localIndices[i];
				vt = localIndices[(uint64_t)i + 1];
				vn = localIndices[(uint64_t)i + 2];
				vertices->emplace_back(positions[v], uvies[vt], normals[vn]);
			}
			//面情報からインデックスを作る
			//4頂点と3頂点で分岐
			//4頂点なら6個追加
			if (vCount == 4)
			{
				indices->push_back(startIndex);
				indices->push_back(startIndex + 1);
				indices->push_back(startIndex + 2);
				indices->push_back(startIndex + 2);
				indices->push_back(startIndex + 3);
				indices->push_back(startIndex);
				startIndex += vCount;
			}
			//3頂点なら3個追加
			if (vCount == 3)
			{
				indices->push_back(startIndex);
				indices->push_back(startIndex + 1);
				indices->push_back(startIndex + 2);
				startIndex += vCount;
			}
		}
	}
	file.close();
}
