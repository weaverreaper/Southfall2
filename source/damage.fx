//=============================================================================
// tree.fx by Frank Luna (C) 2008 All Rights Reserved.
//
// Uses the geometry shader to expand points into y-axis aligned billboards.
//=============================================================================


#include "lighthelper.fx"
#include "random.fx" 
//#include "blending.fx"

cbuffer cbPerFrame
{
	float dt;
	float3 gEyePosW;
	float3 gCenter;
	float4x4 gViewProj;
};
 
// Nonnumeric values cannot be added to a cbuffer.
Texture2DArray gDiffuseMapArray;


SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_IN
{
	float seed	   : PSIZE;
	float value	   : BLENDWEIGHT;
	float2 sizeW   : SIZE;
};

struct VS_OUT
{
	float value	   : BLENDWEIGHT;
	float3 centerW : POSITION;
	float2 sizeW   : SIZE;
};

struct GS_OUT
{
	float value	   : BLENDWEIGHT;
	float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
    float3 normalW : NORMAL;
    float2 texC    : TEXCOORD;
    uint primID    : SV_PrimitiveID;
};

 
VS_OUT VS(VS_IN vIn)
{

	VS_OUT vOut;
	
	// Just pass data into geometry shader stage.
	//vOut.centerW  = vIn.centerW;
	
	rng_state = hash(vIn.seed);

	float3 look = gEyePosW - gCenter;
	look = normalize(look);
	float3 up = float3(0.0f, 1.0f, 0.0f);

	float3 right = cross(up, look);
	right = normalize(right);
	up = cross(look, right);
	up = normalize(up);

	float x = randfloat(-1.0f,1.0f);
	float y = randfloat(-1.0f,1.0f);
	while(x*x+y*y > 1)
	{
		x = randfloat(-1.0f,1.0f);
		y = randfloat(-1.0f,1.0f);
	}

	float sc = 20;
	float MOB_HEIGHT = 25;
	float MOB_WIDTH = 100;
	vOut.centerW = dt*sc*(up*(y*.125+.5)+right*x*.75) + MOB_HEIGHT*up + MOB_WIDTH*look+randfloat(0,1.0f);  //do things

	vOut.centerW += gCenter;
	vOut.sizeW    = vIn.sizeW;
	vOut.value = vIn.value;

	return vOut;
}

[maxvertexcount(4)]
void GS(point VS_OUT gIn[1], 
        uint primID : SV_PrimitiveID, 
        inout TriangleStream<GS_OUT> triStream)
{	
	//
	// Compute 4 triangle strip vertices (quad) in local space.
	// The quad faces down the +z axis in local space.
	//
	float halfWidth  = 0.5f*gIn[0].sizeW.x;
	float halfHeight = 0.5f*gIn[0].sizeW.y;
	
	float4 v[4];
	v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
	v[1] = float4(+halfWidth, -halfHeight, 0.0f, 1.0f);
	v[2] = float4(-halfWidth, +halfHeight, 0.0f, 1.0f);
	v[3] = float4(+halfWidth, +halfHeight, 0.0f, 1.0f);
	//
	// Compute texture coordinates to stretch texture over quad.
	//
	float2 texC[4];
	texC[0] = float2(1.0f, 1.0f);
	texC[1] = float2(0.0f, 1.0f);
	texC[2] = float2(1.0f, 0.0f);
	texC[3] = float2(0.0f, 0.0f);
	//
	// Compute world matrix so that billboard is aligned with
	// the y-axis and faces the camera.
	//
	float3 look = gEyePosW - gCenter;
	look = normalize(look);
	float3 up = float3(0.0f, 1.0f, 0.0f);

	//look.y = 0.0f; // y-axis aligned, so project to xz-plane
	
	float3 right = cross(up, look);
	right = normalize(right);
	up = cross(look, right);
	up = normalize(up);

	/*
	float4x4 Rot;
	Rot[0] = float4(0,-1,0,0.0f);
	Rot[1] = float4(1,0,0,0.0f);
	Rot[2] = float4(0,0,1,0.0f);
	Rot[3] = float4(0,0,0,1.0f);
	*/
	float4x4 W;
	W[0] = float4(right,          0.0f);
	W[1] = float4(up,             0.0f);
	W[2] = float4(look,           0.0f);
	W[3] = float4(gIn[0].centerW, 1.0f);

	float4x4 WVP = mul(W,gViewProj);
	//WVP = mul(Rot,WVP);

	//
	// Transform quad vertices to world space and output 
	// them as a triangle strip.
	//
	GS_OUT gOut;

	[unroll]
	for(int i = 0; i < 4; ++i)
	{
		

		gOut.posH     = mul(v[i], WVP);
		gOut.posW     = mul(v[i], W);
		gOut.normalW  = look;
		gOut.texC     = texC[i];
		gOut.primID   = primID;
		gOut.value	  = gIn[0].value;
		triStream.Append(gOut);
	}
}

float4 PS(GS_OUT pIn) : SV_Target
{
	// Get materials from texture maps.
	
	//float3 uvw = float3(pIn.texC, pIn.primID%4);
	float3 uvw = float3(pIn.texC, pIn.value);
	float4 diffuse = gDiffuseMapArray.Sample( gTriLinearSam, uvw );
 
	// Discard pixel if texture alpha < 0.25.  Note that we do this
	// test as soon as possible so that we can potentially exit the shader 
	// early, thereby skipping the rest of the shader code.
	clip(diffuse.a - 0.0025f);
	
	// Don't light tree billboards, just use texture color.
    
    return diffuse;
}
 
technique10 DamageBillboardTech
{
    pass P0
    {
        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader(    ps_4_0, PS() ) );

		//SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }
}
