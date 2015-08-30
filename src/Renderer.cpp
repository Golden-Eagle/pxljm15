

#include <gecom/Initial3D.hpp>
#include <gecom/Log.hpp>

#include "SimpleShader.hpp"

#include "Renderer.hpp"

using namespace std;
using namespace initial3d;
using namespace gecom;

namespace {

	static const std::string shader_fullscreen_source = R"delim(
// vertex shader
#ifdef _VERTEX_

flat out int instanceID;

void main() {
	instanceID = gl_InstanceID;
}

#endif

// geometry shader
#ifdef _GEOMETRY_

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

flat in int instanceID[];

out vec2 fullscreen_tex_coord;
flat out int fullscreen_layer;

void main() {
	// output a single triangle that covers the whole screen
	// if instanced, set layer to instance id
	
	gl_Position = vec4(3.0, 1.0, 0.0, 1.0);
	gl_Layer = instanceID[0];
	fullscreen_layer = instanceID[0];
	fullscreen_tex_coord = vec2(2.0, 1.0);
	EmitVertex();
	
	gl_Position = vec4(-1.0, 1.0, 0.0, 1.0);
	gl_Layer = instanceID[0];
	fullscreen_layer = instanceID[0];
	fullscreen_tex_coord = vec2(0.0, 1.0);
	EmitVertex();
	
	gl_Position = vec4(-1.0, -3.0, 0.0, 1.0);
	gl_Layer = instanceID[0];
	fullscreen_layer = instanceID[0];
	fullscreen_tex_coord = vec2(0.0, -1.0);
	EmitVertex();
	
	EndPrimitive();
	
}

#endif

// fragment shader
#ifdef _FRAGMENT_

in vec2 fullscreen_tex_coord;
flat in int fullscreen_layer;

// main() should be implemented by includer

#endif

)delim";

	static const std::string shader_deferred0_source = shader_fullscreen_source + R"delim(
const float pi = 3.14159265897925;

uniform sampler2D sampler_depth;
uniform sampler2D sampler_color;
uniform sampler2D sampler_normal;

uniform mat4 inv_proj_matrix;
uniform float zfar;

uniform int num_lights;

#define MAX_LIGHTS 128

// all positions/directions in view space
struct Light {
	vec4 pos;
	vec4 dir;
	vec3 rgb;
	float spot_cos_cutoff;
	vec3 k;
	float spot_exp;
};

layout(std140) uniform LightBlock {
	Light lights[MAX_LIGHTS];
};

#ifdef _FRAGMENT_

vec3 fragpos_v() {
	float depth = texture(sampler_depth, fullscreen_tex_coord).r;
	const float C = 0.01;
	float FC = 1.0 / log(zfar * C + 1.0);
	float z = -(exp(depth / FC) - 1.0) / C;
	vec4 temp = inv_proj_matrix * vec4(fullscreen_tex_coord * 2.0 - 1.0, 0.0, 1.0);
	return temp.xyz * (z / temp.z);
}

vec3 fragnorm_v() {
	vec2 thetaphi = texture(sampler_normal, fullscreen_tex_coord).xy;
	vec3 n;
	n.x = cos(thetaphi.x) * sin(thetaphi.y);
	n.y = sin(thetaphi.x) * sin(thetaphi.y);
	n.z = cos(thetaphi.y);
	return normalize(n);
}

out vec4 frag_color;

vec3 orenNayarDiffuse(
	vec3 lightDirection,
	vec3 viewDirection,
	vec3 surfaceNormal,
	float roughness,
	vec3 albedo
) {
	float LdotV = dot(lightDirection, viewDirection);
	float NdotL = dot(lightDirection, surfaceNormal);
	float NdotV = dot(surfaceNormal, viewDirection);

	float s = LdotV - NdotL * NdotV;
	float t = mix(1.0, max(NdotL, NdotV), step(0.0, s));

	float sigma2 = roughness * roughness;
	vec3 A = vec3(1.0 + sigma2 * (albedo / (sigma2 + 0.13) + 0.5 / (sigma2 + 0.33)));
	vec3 B = vec3(0.45 * sigma2 / (sigma2 + 0.09));

	return albedo * (A + B * s / t) / pi;
}

vec3 cookTorranceSpecular(
	vec3 lightDirection,
	vec3 eyeDir,
	vec3 normal,
	float roughnessValue,
	vec3 fresnel0,
	float specratio
) {
	// set important material values
	//float roughnessValue = 0.3; // 0 : smooth, 1: rough
	//float F0 = 0.8; // fresnel reflectance at normal incidence
	//vec3 k = 0.2; // fraction of diffuse reflection (specular reflection = 1 - k)
	
	// interpolating normals will change the length of the normal, so renormalize the normal.
	//vec3 normal = normalize(varNormal);
	
	// do the lighting calculation for each fragment.
	float NdotL = max(dot(normal, lightDirection), 0.0);
	
	vec3 specular = vec3(0.0);
	if(NdotL > 0.0) {
		//vec3 eyeDir = normalize(varEyeDir);

		// calculate intermediary values
		vec3 halfVector = normalize(lightDirection + eyeDir);
		float NdotH = max(dot(normal, halfVector), 0.0); 
		float NdotV = max(dot(normal, eyeDir), 0.0); // note: this could also be NdotL, which is the same value
		float VdotH = max(dot(eyeDir, halfVector), 0.0);
		float mSquared = roughnessValue * roughnessValue;
		
		// geometric attenuation
		float NH2 = 2.0 * NdotH;
		float g1 = (NH2 * NdotV) / VdotH;
		float g2 = (NH2 * NdotL) / VdotH;
		float geoAtt = min(1.0, min(g1, g2));
	 
		// roughness (or: microfacet distribution function)
		// beckmann distribution function
		float r1 = 1.0 / ( 4.0 * mSquared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
		float roughness = r1 * exp(r2);
		
		// fresnel
		// Schlick approximation
		vec3 fresnel = vec3(pow(1.0 - VdotH, 5.0));
		fresnel *= (1.0 - fresnel0);
		fresnel += fresnel0;
		
		specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.14);
	}
	
	// TODO forward kd to oren-nayar?
	return vec3((1.0 - specratio)) + specular * specratio;
}

void main() {
	vec3 pos_v = fragpos_v();
	vec3 norm_v = fragnorm_v();
	vec4 temp;
	temp = texture(sampler_color, fullscreen_tex_coord);
	vec3 color = temp.rgb;
	float metal = temp.a;
	temp = texture(sampler_normal, fullscreen_tex_coord);
	float roughness = temp.z;
	float specular = temp.w;

	// TODO metals...

	vec3 kd = color;

	vec3 lr = vec3(0.0);

	for (int i = 0; i < num_lights; i++) {
		Light light = lights[i];

		// incident irradiance
		vec3 e0 = vec3(0.0);
		
		if (light.pos[3] < 0.001) {
			// directional
			
		} else if (light.spot_cos_cutoff < 0.001) {
			// point light
			float d = distance(pos_v, light.pos.xyz);
			e0 = light.rgb / vec3((4.0 * pi * (light.k[0] + light.k[1] * d + light.k[2] * d * d)));

		} else {
			// spotlight
			
		}

		// light direction from surface
		vec3 surflightdir = normalize(light.pos.xyz - pos_v);
		
		// cos falloff
		float ndotl = dot(norm_v, surflightdir);

		lr += e0 * ndotl * orenNayarDiffuse(surflightdir, -normalize(pos_v), norm_v, roughness, kd);
		//lr += e0 * ndotl; // * cookTorranceSpecular(surflightdir, -normalize(pos_v), norm_v, roughness, vec3(0.8), kd);
		

	}
	
	frag_color = 1.0 - exp(-20.0 * vec4(lr, 1.0));
}

#endif

)delim";

	struct light_data {
		float pos[4];
		float dir[4];
		float rgb[3];
		float spot_cos_cutoff = 0.f;
		float k[3];
		float spot_exp = 0.f;
	};

}

namespace pxljm {

	void Renderer::initFBO(size2i sz) {
		if (sz == m_fbsize) return;
		
		glActiveTexture(GL_TEXTURE0);

		if (!m_fbo_scene) {
			// make fbo and textures
			glGenFramebuffers(1, &m_fbo_scene);
			glGenTextures(1, &m_tex_scene_depth);
			glGenTextures(1, &m_tex_scene_color);
			glGenTextures(1, &m_tex_scene_normal);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo_scene);

			// depth texture
			glBindTexture(GL_TEXTURE_2D, m_tex_scene_depth);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, sz.w, sz.h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_tex_scene_depth, 0);

			// color + metallicity texture
			glBindTexture(GL_TEXTURE_2D, m_tex_scene_color);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sz.w, sz.h, 0, GL_RGBA, GL_FLOAT, nullptr);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex_scene_color, 0);

			// normal + roughness + specularity texture
			glBindTexture(GL_TEXTURE_2D, m_tex_scene_normal);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, sz.w, sz.h, 0, GL_RGBA, GL_FLOAT, nullptr);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_tex_scene_normal, 0);

			GLenum bufs_scene[] { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(2, bufs_scene);
		}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo_scene);

		glBindTexture(GL_TEXTURE_2D, m_tex_scene_depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, sz.w, sz.h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glBindTexture(GL_TEXTURE_2D, m_tex_scene_color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sz.w, sz.h, 0, GL_RGBA, GL_FLOAT, nullptr);

		glBindTexture(GL_TEXTURE_2D, m_tex_scene_normal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, sz.w, sz.h, 0, GL_RGBA, GL_FLOAT, nullptr);

		m_fbsize = sz;
	}

	void Renderer::renderScene(Scene &s) {

		
		glClearColor(0.f, 0.f, 0.f, 1.f); // default background color

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		auto size = m_win->framebufferSize();

		if (size == size2i(0, 0)) return;
		
		initFBO(size);

		s.cameraSystem().update(size.w, size.h);
		glViewport(0, 0, size.w, size.h);

		// draw material properties to scene buffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo_scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double zfar = s.cameraSystem().getPrimaryCamera()->getZfar();
		i3d::mat4d view = s.cameraSystem().getPrimaryCamera()->getViewMatrix();
		i3d::mat4d proj = s.cameraSystem().getPrimaryCamera()->getProjectionMatrix();
		std::priority_queue<DrawCall *> drawList = s.drawableSystem().getDrawQueue(view);

		for (; !drawList.empty(); drawList.pop()) {
			auto d = drawList.top();
			// Bind shader program
			// Bind material properties
			// Bind Geometry
			// Then render

			material_ptr m = d->material();
			m->shader->bind();
			m->bind(proj, zfar);
			d->draw();
		}

		// collect light data
		static_assert(sizeof(light_data) % 16 == 0, "light_data sizeof must be multiple of 16");
		
		std::vector<light_data> lights;

		for (PointLight *l : s.lightSystem().getPointLights()) {
			light_data ld;
			i3d::mat4d mv = view * l->entity()->root()->matrix();
			vec3d pos = mv * vec4d(vec3d(), 1.0);
			vec3d dir = (mv * vec4d(vec3d::k(-1), 0.0)).xyz<double>();
			ld.pos[0] = pos.x();
			ld.pos[1] = pos.y();
			ld.pos[2] = pos.z();
			ld.pos[3] = 1.f;
			ld.dir[0] = dir.x();
			ld.dir[1] = dir.y();
			ld.dir[2] = dir.z();
			ld.dir[3] = 0.f;
			vec3f flux = l->flux();
			ld.rgb[0] = flux.x();
			ld.rgb[1] = flux.y();
			ld.rgb[2] = flux.z();
			ld.spot_cos_cutoff = 0.f;
			ld.k[0] = 1.f;
			ld.k[1] = 2.f / l->radius();
			ld.k[2] = 1.f / i3d::math::sq(l->radius());
			ld.spot_exp = 0.f;
			lights.push_back(ld);
		}

		// upload light data to buffer
		if (!lights.empty()) {
			if (!m_light_ubo) glGenBuffers(1, &m_light_ubo);
			glBindBuffer(GL_ARRAY_BUFFER, m_light_ubo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(light_data) * lights.size(), &lights.front(), GL_STREAM_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_light_ubo);
		}

		static GLuint prog_deferred0 = 0;
		if (!prog_deferred0) {
			prog_deferred0 = gecom::makeShaderProgram("330 core", { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER }, shader_deferred0_source);
		}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(prog_deferred0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_tex_scene_depth);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_tex_scene_color);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_tex_scene_normal);

		glUniform1i(glGetUniformLocation(prog_deferred0, "sampler_depth"), 0);
		glUniform1i(glGetUniformLocation(prog_deferred0, "sampler_color"), 1);
		glUniform1i(glGetUniformLocation(prog_deferred0, "sampler_normal"), 2);

		glUniformMatrix4fv(glGetUniformLocation(prog_deferred0, "inv_proj_matrix"), 1, true, mat4f(!proj));
		glUniform1f(glGetUniformLocation(prog_deferred0, "zfar"), zfar);
		glUniform1i(glGetUniformLocation(prog_deferred0, "num_lights"), lights.size());

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_light_ubo);
		glUniformBlockBinding(prog_deferred0, glGetUniformBlockIndex(prog_deferred0, "LightBlock"), 0);


		draw_dummy();



		glUseProgram(0);

		s.physicsSystem().debugDraw(s);
		
	}


}
