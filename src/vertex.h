#pragma once
#ifndef VERTEX
#define VERTEX
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class vertex {

public:

	vertex() {

		position = vec3(0);
		normal = vec3(0);
		uv = vec2(0);

	}

	vertex(vec3 p, vec3 n, vec2 UV) {

		position = p;
		normal = n;
		uv = UV;

	}

	vec3 Position() { return position; }
	vec3 Normal() { return normal; }
	vec2 UVs() { return uv; }

private:

	vec3 position;
	vec3 normal;
	vec2 uv;

};

#endif // !VERTEX

