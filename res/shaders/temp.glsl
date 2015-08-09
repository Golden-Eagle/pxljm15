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
layout(location = 2) in vec3 aUV;

out VertexData {
	vec4 pos;
	vec4 normal;
	vec2 uv;
} v_out;

void main() {
	vec4 p = (uModelViewMatrix * vec4(aPosition, 1.0));
	gl_Position = uProjectionMatrix * p;
	v_out.pos = p;
	v_out.normal = (uModelViewMatrix * vec4(aNormal, 0.0));
}

#endif


#ifdef _FRAGMENT_

in VertexData {
	vec4 pos;
	vec4 normal;
	vec2 uv;
} v_in;

out vec3 color;

void main() {
	vec3 grey = vec3(0.8, 0.8, 0.8);
    color = abs((uProjectionMatrix * v_in.normal).z) * grey;
}

#endif