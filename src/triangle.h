#pragma once
#ifndef TRIANGLE
#define TRIANGLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vertex.h"

using namespace glm;

//Store indices of attributes
class triangle {

public:

	triangle() {

		p[0] = p[1] = p[2] = n[0] = n[1] = n[2] = t[0] = t[1] = t[2] = - 1;

	}

	triangle(int P1, int P2, int P3, int N1, int N2, int N3, int uv1, int uv2, int uv3) {

		p[0] = P1;
		p[1] = P2;
		p[2] = P3;
		n[0] = N1;
		n[1] = N2;
		n[2] = N3;
		t[0] = uv1;
		t[1] = uv2;
		t[2] = uv3;

	}

	int A() { return p[0]; }
	int B() { return p[1]; }
	int C() { return p[2]; }
	void swapAB() { 

		std::swap(p[0], p[1]); 
		std::swap(n[0], n[1]); 
		std::swap(t[0], t[1]); 
	
	}
	void swapBC() {

		std::swap(p[1], p[2]);
		std::swap(n[1], n[2]);
		std::swap(t[1], t[2]);

	}

private:

	int p[3];
	int n[3];
	int t[3];

};


#endif // !TRIANGLE