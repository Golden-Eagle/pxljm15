
// a   per-vertex attribute from application
// u   uniform variable from application
// v   from vertex shader
// tc  from tessellation control shader
// te  from tessellation evaluation shader
// g   from geometry shader
// f   from fragment shader

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform float uZFar;


uniform vec3 uDiffuse;
uniform float uMetalicity;
uniform float uRoughness;
uniform float uSpecular;

uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;


//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------


#ifdef _VERTEX_

// @josh - don't we not need to pass bitangents to the shader? we can calculate them from tangent and normal, right?

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aUV;


out VertexData {
	vec3 pos;
	vec3 normal;
	vec3 tangent;
	vec2 uv;
} v_out;


void main() {
	v_out.pos = (uModelViewMatrix * vec4(aPosition, 1.0)).xyz;
	v_out.normal = (uModelViewMatrix * vec4(aNormal, 0.0)).xyz;
	v_out.tangent = (uModelViewMatrix * vec4(aTangent, 0.0)).xyz;
	v_out.uv  = aUV;

	gl_Position = uProjectionMatrix * vec4(v_out.pos, 1.0);
}

#endif


//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------


#ifdef _FRAGMENT_

// Viewspace data
in VertexData {
	vec3 pos;
	vec3 normal;
	vec3 tangent;
	vec2 uv;
} f_in;

#ifndef _DEPTH_ONLY_

// (rgb) diffuse, (a) metalicity or (rgb) diffuse, (a) opacity
// (rg) normal, (b) roughness, (a) specularity
//
layout(location = 0) out vec4 fDiffuse;
layout(location = 1) out vec4 fNormalMaterials;

#endif


subroutine vec3 diffuseFetch();
subroutine vec3 normalFetch();

subroutine uniform diffuseFetch getDiffuse;
subroutine uniform normalFetch getNormal;

// Diffuse
//
subroutine(diffuseFetch) vec3 diffuseFromTexture() { return texture(uDiffuseMap, f_in.uv).rgb; }
subroutine(diffuseFetch) vec3 diffuseFromValue() { return uDiffuse; }

// Normal
//
subroutine(normalFetch) vec3 normalFromTexture() {
	vec3 bitangent = normalize(cross(f_in.normal, f_in.tangent));
	vec3 tangent = normalize(cross(bitangent, f_in.normal));
	mat3 tbn = transpose(mat3(tangent, bitangent, f_in.normal));
	return vec3(tbn * normalize(texture(uNormalMap, f_in.uv).rgb * 2.0 - 1.0));
}
subroutine(normalFetch) vec3 normalFromValue() { return normalize(f_in.normal); }


void writeDepth() {
	// this has to match with depth buffer settings from shadow shaders
	const float C = 0.01;
	float FC = 1.0 / log(uZFar * C + 1.0);
	gl_FragDepth = log(-f_in.pos.z * C + 1.0) * FC;
}


//
// Main
//
void main() {
	writeDepth();

	#ifndef _DEPTH_ONLY_

	
	vec3 n = normalize(f_in.normal);
	float theta = atan(n.y / n.x);
	float phi = atan(sqrt(n.x * n.x + n.y * n.y) / n.z);
	
	//vec3 a = normalize(f_in.pos);
	//vec3 b = normalize(cross(vec3(1.0, 0.0, 0.0), a));
	//vec3 c = normalize(cross(a, b));
	
	//vec3 n = mat3(c, b, a) * normalize(f_in.normal);
	
	//vec3 n = normalize((uProjectionMatrix * vec4(getNormal(), 0.0)).xyz);
	//n = faceforward(n, vec4(0.0, 0.0, 1.0, 0.0), n);
	
	fDiffuse = vec4(getDiffuse(), uMetalicity);
	fNormalMaterials = vec4(theta, phi, uRoughness, uSpecular);

	#endif
}


#endif





























