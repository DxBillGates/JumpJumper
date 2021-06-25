#include "DefaultSpriteShader.hlsli"

DefaultSpriteVSOutput main(DefaultSpriteVSInput input)
{
	DefaultSpriteVSOutput output;
	output.svpos = mul(projMatrix, mul(modelMatrix,input.pos));
	output.uv = input.uv;
	return output;
}