#include "..\..\Header\Graphics\Mesh.h"
#include "..\..\Header\Graphics\COMRelease.h"

GatesEngine::Mesh::Mesh()
	: vBuffer(nullptr)
	, iBuffer(nullptr)
	, vbView({})
	, ibView({})
	, indicesCount(0)
	, pGraphicsDevice(nullptr)
	, cmdList(nullptr)
{
}

GatesEngine::Mesh::~Mesh()
{
	COM_RELEASE(vBuffer);
	COM_RELEASE(iBuffer);
}

void GatesEngine::Mesh::Draw()
{
	cmdList->IASetIndexBuffer(&ibView);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->DrawIndexedInstanced(indicesCount, 1, 0, 0, 0);
}
