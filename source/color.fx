//=============================================================================
// color.fx by Frank Luna (C) 2008 All Rights Reserved.
//
// Transforms and colors geometry.
//=============================================================================

#include "lighthelper.fx"

#define NUM_POINT_LIGHTS 7

cbuffer cbPerFrame
{
	Light gLights[NUM_POINT_LIGHTS + 1];
	float3 gEyePosW;
};

bool gSpecularEnabled;

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWVP; 
	float4x4 gTexMtx;
};

Texture2D gDiffuseMap;
Texture2D gSpecMap;
float blood;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;
};

struct VS_IN
{
	float3 posL    : POSITION;
	float3 normalL : NORMAL;
	float2 texC    : TEXCOORD;
};

struct VS_OUT
{
	float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
    float3 normalW : NORMAL;
    float2 texC    : TEXCOORD;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	// Transform to world space space.
	vOut.posW    = mul(float4(vIn.posL, 1.0f), gWorld);
	vOut.normalW = mul(float4(vIn.normalL, 0.0f), gWorld);
		
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);
	
	// Output vertex attributes for interpolation across triangle.
	vOut.texC  = mul(float4(vIn.texC, 0.0f, 1.0f), gTexMtx);
	
	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	// Get materials from texture maps.
	float alpha    = gDiffuseMap.Sample( gTriLinearSam, pIn.texC ).a;
	
	// Discard pixel if texture alpha < 0.25.  Note that we do this
	// test as soon as possible so that we can potentially exit the shader 
	// early, thereby skipping the rest of the shader code.
	//clip(alpha - 0.25f);
	
	float4 diffuse = gDiffuseMap.Sample( gTriLinearSam, pIn.texC );
	float4 spec    = gSpecMap.Sample( gTriLinearSam, pIn.texC );

	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Interpolating normal can make it not be of unit length so normalize it.
	pIn.normalW = normalize(pIn.normalW); 

	SurfaceInfo v = {pIn.posW, pIn.normalW, diffuse, spec};

	float3 litColor =	ParallelLight(v, gLights[0], gEyePosW);

	[loop]
	for (uint i = 1; i < NUM_POINT_LIGHTS+1; i++) 
		litColor += PointLight(v, gLights[i], gEyePosW);
	
	[branch]
	if (blood > 0) alpha += blood;
	

	return float4(litColor, alpha);
}

technique10 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
