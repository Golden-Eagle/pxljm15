/*
 *
 * Default shader program for writing to scene buffer using GL_TRIANGLES
 *
 */

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

#ifdef _VERTEX_

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out VertexData {
	vec3 pos;
	vec3 normal;
	vec2 uv;
} v_out;

void main() {
	vec4 p = (uModelViewMatrix * vec4(aPosition, 1.0));
	gl_Position = uProjectionMatrix * p;
	v_out.pos = p.xyz;
	v_out.normal = normalize((uModelViewMatrix * vec4(aNormal, 0.0)).xyz);
}

#endif


#ifdef _FRAGMENT_

in VertexData {
	vec3 pos;
	vec3 normal;
	vec2 uv;
} v_in;

out vec3 color;

void main() {
	vec3 eye = normalize(-v_in.pos);
	color = abs(dot(normalize(v_in.normal), eye)) * vec3(0.5) + vec3(0.3);
}

#endif