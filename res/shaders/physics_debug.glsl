/*
 *
 * Default shader program for drawing lines for the Bullet physics library
 *
 */

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

#ifdef _VERTEX_

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

out VertexData {
	vec3 pos;
	vec3 col;
} v_out;

void main() {
	vec4 p = (uModelViewMatrix * vec4(aPosition, 1.0)) + vec4(0, 0, 0.001, 0); // hack
	gl_Position = uProjectionMatrix * p;
	v_out.pos = p.xyz;
	v_out.col = aColor;
}

#endif


#ifdef _FRAGMENT_

in VertexData {
	vec3 pos;
	vec3 col;
} v_in;

out vec3 color;

void main() {
	color = v_in.col;
}

#endif