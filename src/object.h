#pragma once
#ifndef OBJECT
#define OBJECT

#include <iostream>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "triangle.h"

extern std::vector<vec3> position_queue;
extern std::vector<vec3> normal_queue;
extern std::vector<vec2> uv_queue;

class Object
{

	Object() 
	{

		printf("[Error]Create an object without data!\n");
		assert(0);

	}

	Object(float m, std::vector<triangle> indices_queue, vec3 trans, vec3 rot, vec3 sca) 
	{

		mass = m;
		glm::translate(translate, trans);
		mat4 rotation_tmp;
		glm::rotate_slow(rotation_tmp, rot.x, vec3(1, 0, 0));
		rotation = rotation_tmp;
		glm::rotate_slow(rotation, rot.y, vec3(0, 1, 0));
		rotation *= rotation_tmp;
		glm::rotate_slow(rotation, rot.z, vec3(0, 0, 1));
		rotation *= rotation_tmp;
		glm::scale(scale, sca);
		size = indices_queue.size();
		triangles = indices_queue;

		GetCenter();

	}

public:
protected:
private:

	void GetCenter() {

		std::map<int, bool> flag;
		for (int i = 0; i < size; i++)
		{

			int index = triangles[i].A();
			if (flag[index] == 0)
			{

				c += position_queue[index];
				flag[index] = true;

			}

			int index = triangles[i].B();
			if (flag[index] == 0)
			{

				c += position_queue[index];
				flag[index] = true;

			}

			int index = triangles[i].C();
			if (flag[index] == 0)
			{

				c += position_queue[index];
				flag[index] = true;

			}

		}

		c /= (float)size;

	}

	float mass;
	mat4 translate;
	mat4 rotation;
	mat4 scale;
	vec3 x = vec3(0);
	vec3 v = vec3(0);
	vec3 w = vec3(0);
	vec3 q = vec3(0);
	vec3 c = vec3(0);
	int size;
	std::vector<triangle> triangles;

};

#endif // !OBJECT
