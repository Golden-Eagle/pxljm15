
// a   per-vertex attribute from application
// u   uniform variable from application
// v   from vertex shader
// tc  from tessellation control shader
// te  from tessellation evaluation shader
// g   from geometry shader
// f   from fragment shader

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform float zfar;

uniform material {
	vec3 uColor;
	float uMetalicity;
	float uRoughness;
	float uSpecular;
} value;

uniform sampler2D​ uColorMap;
uniform sampler2D​ uNormalMap;


//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------


#ifdef _VERTEX_

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBiTangent;
layout(location = 4) in vec2 aUV;
// layout(location = 5) in vec2 aUV1;
// layout(location = 6) in vec2 aUV2;
// layout(location = 7) in vec2 aUV3;


out VertexData {
	vec4 pos;
	vec4 normal;
	vec4 tangent;
	vec4 bitangent;
	vec2 uv;
	// vec2 uv1;
	// vec2 uv2;
	// vec2 uv3;
} v_out;


void main() {
	v_out.pos = uModelViewMatrix * vec4(aPosition, 0.0);
	v_out.normal = uModelViewMatrix * vec4(aNormal, 0.0);
	v_out.tangent = uModelViewMatrix * vec4(aTangent, 0.0);
	v_out.bitangent = uModelViewMatrix * vec4(aBiTangent, 0.0);
	v_out.uv  = aUV;
	// v_out.uv1 = aUV1;
	// v_out.uv2 = aUV2;
	// v_out.uv3 = aUV3;

	gl_Position = uProjectionMatrix * p;
}

#endif


//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------


#ifdef _FRAGMENT_

// Viewspace data
in VertexData {
	vec4 pos;
	vec4 normal;
	vec4 tangent;
	vec4 bitangent;
	vec2 uv;
	// vec2 uv1;
	// vec2 uv2;
	// vec2 uv3;
} f_in;


// (rgb) color, (a) metalicity or (rgb) color, (a) opacity
// (rg) normal, (b) roughness, (a) specularity
//
layout(location = 0) out vec4 fColor;
layout(location = 1) out vec4 fNormalMaterials;


subroutine void drawmode();
subroutine vec3 colorFetch();
subroutine vec3 normalFetch();

subroutine uniform drawmode drawFragment;
subroutine uniform colorFetch getColor;
subroutine uniform normalFetch getNormal;


//
// Helper functions
//
void writeDepth(float depth) {
	// this has to match with depth buffer settings from shadow shaders
	const float C = 0.01;
	float FC = 1.0 / log(zfar * C + 1.0);
	gl_FragDepth = log(depth * C + 1.0) * FC;
}

// Drawing subroutines
//
subroutine(drawmode) void depth_only() {
	vec4 p = uProjectionMatrix * f_in.pos();
	writeDepth(p.z/p.w);
}
subroutine(drawmode) void material() {
	vec4 p = uProjectionMatrix * f_in.pos();
	writeDepth(p.z/p.w);

	vec4 n = uProjectionMatrix * getNormal();
	n = faceforward(n, vec4(0.0, 0.0, 1.0, 0.0), n);
	fColor = vec4(getColor(), value.uMetalicity);
	getNormal = vec4(n.xy, value.uRoughness, value.uSpecular);
}

// Color
//
subroutine(colorFetch) vec3 colorFromTexture() { return texture(uColorMap, f_in.uv).rgb; }
subroutine(colorFetch) vec3 colorFromValue() { return value.uColor; }

// Normal
//
subroutine(colorFetch) vec4 normalTexture() {
	mat3 tbn = transpose(tangent.xyz, bitangent.xyz, f_in.normal.xyz);
	return vec4(tbn * normalise(texture(uNormalMap, f_in.uv).rgb * 2.0 - 1.0), 0.0);
}
subroutine(colorFetch) vec4 normalAttrib() { return f_in.normal; }


//
// Main
//

void main() {
	drawFragment();
}


#endif