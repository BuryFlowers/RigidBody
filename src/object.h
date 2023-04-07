#pragma once
#ifndef OBJECT
#define OBJECT

#define epsilon 1e-2f
#define elasticity 0.1f

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

public:

	Object() 
	{

		printf("[Error]Create an object without data!\n");
		assert(0);

	}

	Object(float m, std::vector<triangle> indices_queue, vec3 trans, vec3 rot) 
	{

		mass = m;
		x = trans;
		q = qua<float>(radians(rot));
		size = indices_queue.size();
		triangles = indices_queue;
		hasGravity = false;

		points.clear();
		GetCenter();
		InitInertia();

	}

	int GetSize() { return size; }
	void SetGravity(bool value) { hasGravity = value; }
	bool GetGravity() { return hasGravity; }
	float GetMass() { return mass; }
	triangle GetTriangle(int index)
	{
	
		assert(index < size);
		return triangles[index];
	
	}
	void GetVBO(int& count, float* &vertices) 
	{

		count = size * 24;
		vertices = new float[count];

		for (int i = 0; i < size; i++)
		{
		
			vertices[i * 24 +  0] = position_queue[triangles[i].P_A()].x;
			vertices[i * 24 +  1] = position_queue[triangles[i].P_A()].y;
			vertices[i * 24 +  2] = position_queue[triangles[i].P_A()].z;
			vertices[i * 24 +  3] = normal_queue[triangles[i].N_A()].x;
			vertices[i * 24 +  4] = normal_queue[triangles[i].N_A()].y;
			vertices[i * 24 +  5] = normal_queue[triangles[i].N_A()].z;
			vertices[i * 24 +  6] = uv_queue[triangles[i].T_A()].x;
			vertices[i * 24 +  7] = uv_queue[triangles[i].T_A()].y;

			vertices[i * 24 +  8] = position_queue[triangles[i].P_B()].x;
			vertices[i * 24 +  9] = position_queue[triangles[i].P_B()].y;
			vertices[i * 24 + 10] = position_queue[triangles[i].P_B()].z;
			vertices[i * 24 + 11] = normal_queue[triangles[i].N_B()].x;
			vertices[i * 24 + 12] = normal_queue[triangles[i].N_B()].y;
			vertices[i * 24 + 13] = normal_queue[triangles[i].N_B()].z;
			vertices[i * 24 + 14] = uv_queue[triangles[i].T_B()].x;
			vertices[i * 24 + 15] = uv_queue[triangles[i].T_B()].y;

			vertices[i * 24 + 16] = position_queue[triangles[i].P_C()].x;
			vertices[i * 24 + 17] = position_queue[triangles[i].P_C()].y;
			vertices[i * 24 + 18] = position_queue[triangles[i].P_C()].z;
			vertices[i * 24 + 19] = normal_queue[triangles[i].N_C()].x;
			vertices[i * 24 + 20] = normal_queue[triangles[i].N_C()].y;
			vertices[i * 24 + 21] = normal_queue[triangles[i].N_C()].z;
			vertices[i * 24 + 22] = uv_queue[triangles[i].T_C()].x;
			vertices[i * 24 + 23] = uv_queue[triangles[i].T_C()].y;
		
		}

	}
	
	mat4 GetTranslate() 
	{
		
		mat4 m = mat4(1.0f);
		return glm::translate(m, x);
	
	}

	mat4 GetRotation() { return glm::mat4_cast(q); }

	mat4 GetModel() 
	{

		/*mat4 model = glm::mat4_cast(q);
		model = glm::translate(model, x);*/
		return GetTranslate() * GetRotation();

	}

	vec3 GetTorque(vec3 force, mat4 R)
	{

		vec3 torque = vec3(0);
		int pointsN = points.size();
		for (int i = 0; i < pointsN; i++)
		{

			vec3 Rri = R * vec4((points[i] - c), 0.0f);
			torque += cross(Rri, force);

		}

		return torque;

	}

	bool CheckIntersection(Object o)
	{
	
		std::vector<vec3> pointsInside;
		pointsInside.clear();
		vec3 delta_x = vec3(0);
		vec3 delta_v = vec3(0);
		vec3 delta_w = vec3(0);
		int pointsN = points.size();
		for (int i = 0; i < pointsN; i++) 
		{

			vec3 Rri = GetRotation() * vec4((points[i] - c), 0.0f);
			vec3 point = x + Rri;
			bool isInside = true;
			int triangleN = o.GetSize();
			for (int j = 0; j < triangleN; j++) 
			{

				triangle T = o.GetTriangle(j);
				// get the triangle's plane normal
				mat4 model = o.GetModel();
;				vec3 t_normal = normal_queue[T.N_A()] + normal_queue[T.N_B()] + normal_queue[T.N_C()];
				vec3 p = o.GetModel() * vec4(position_queue[T.P_A()], 1.0f);
				t_normal = transpose(inverse(model)) * vec4(normalize(t_normal), 0.0f);

				float d = dot((point - p), t_normal);
				// if the point intersects with the object
				if (d > epsilon)
				{

					isInside = false;
					break;

				}

			}

			if (isInside) pointsInside.push_back(points[i]);

		}

		if (!pointsInside.empty())
		{

			vec3 point = vec3(0);
			int pointsInsideN = pointsInside.size();
			for (int i = 0; i < pointsInsideN; i++) point += pointsInside[i];
			point /= pointsInsideN;
			vec3 Rri = GetRotation() * vec4((point - c), 0.0f);
			point = x + Rri;

			float distance = 0;
			vec3 normal;
			int triangleN = o.GetSize();
			for (int i = 0; i < triangleN; i++)
			{

				triangle T = o.GetTriangle(i);
				// get the triangle's plane normal
				mat4 model = o.GetModel();
				vec3 t_normal = normal_queue[T.N_A()] + normal_queue[T.N_B()] + normal_queue[T.N_C()];
				vec3 p = o.GetModel() * vec4(position_queue[T.P_A()], 1.0f);
				t_normal = transpose(inverse(model)) * vec4(normalize(t_normal), 0.0f);

				float d = dot((point - p), t_normal);
				// if the point intersects with the object
				if (d < epsilon)
				{

					if (i == 0) distance = d, normal = t_normal;
					else if (distance < d) distance = d, normal = t_normal;

				}
				else return false;

			}

			x += (epsilon - distance) * normal;

			vec3 vi = v + cross(w, Rri);
			if (dot(vi, normal) < 0) {

				printf("[Debug]intersected!\n");

				vec3 viN = dot(vi, normal) * normal;
				vec3 viT = vi - viN;

				vec3 new_viN = -elasticity * viN;
				float a = max(0.0f, 1.0f - elasticity * (1 + elasticity) * length(viN) / length(viT));
				vec3 new_viT = a * viT;

				mat3 I_1 = mat3(inverse(I));
				mat3 K = mat3(1.0f / mass) - GetMatrixProduct(Rri) * mat3(inverse(I)) * GetMatrixProduct(Rri);
				vec3 j = inverse(K) * (new_viN + new_viT - vi);

				v += j / mass;
				w += mat3(inverse(I)) * cross(Rri, j);

				return true;

			}

		}

		return false;

	}

	void IntegrateLinearVelocity(vec3 force, float dt)
	{

		vec3 a = force / mass;

		vec3 new_v;
		vec3 new_x;
		new_v = v + a * dt;
		new_x = x + new_v * dt;

		v = new_v;
		x = new_x;

	}

	void IntegrateAngularVelocity(vec3 force, float dt)
	{

		mat4 R = GetRotation();
		vec3 torque = GetTorque(force, R);
		mat4 Iref = R * I * transpose(R);
		
		vec3 new_w;
		qua<float> new_q;
		new_w = w + vec3(inverse(Iref) * vec4(torque, 0.0f)) * dt;
		qua<float> p = qua<float>(0, new_w * 0.5f * dt);
		new_q = q + p * q;

		w = new_w;
		q = new_q;

	}

protected:
private:

	void GetCenter() 
	{

		std::map<int, bool> flag;
		for (int i = 0; i < size; i++)
		{

			int index = triangles[i].P_A();
			if (flag[index] == 0)
			{

				points.push_back(position_queue[index]);
				c += position_queue[index];
				flag[index] = true;

			}

			index = triangles[i].P_B();
			if (flag[index] == 0)
			{

				points.push_back(position_queue[index]);
				c += position_queue[index];
				flag[index] = true;

			}

			index = triangles[i].P_C();
			if (flag[index] == 0)
			{

				points.push_back(position_queue[index]);
				c += position_queue[index];
				flag[index] = true;

			}

		}

		c /= (float)size;

	}

	void InitInertia()
	{

		mat3 m = mat3(0);
		int pointsN = points.size();
		for (int i = 0; i < pointsN; i++)
		{

			vec3 ri = points[i] - c;
			mat3 ririT = mat3(ri.x * ri.x, ri.x * ri.y, ri.x * ri.z, ri.x * ri.y, ri.y * ri.y, ri.y * ri.z, ri.x * ri.z, ri.y * ri.z, ri.z * ri.z);
			m += (mat3(dot(ri, ri)) - ririT);

		}

		m = m * (mass / pointsN);
		I = mat4(m);

	}

	mat3 GetMatrixProduct(vec3 r)
	{
	
		return mat3(0, r.z, -r.y, -r.z, 0, r.x, r.y, -r.x, 0);

	}

	float mass = 0;
	vec3 x = vec3(0);
	vec3 v = vec3(0);
	qua<float> q;
	vec3 w = vec3(0);
	mat4 I = mat4(0);
	vec3 c = vec3(0);
	int size = 0;
	std::vector<triangle> triangles;
	std::vector<vec3> points;
	bool hasGravity = false;

};

#endif // !OBJECT
