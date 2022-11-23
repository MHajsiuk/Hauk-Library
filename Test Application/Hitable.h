#pragma once

#ifndef  __HITABLE_H_

#define __HITABLE_H_

#include "Ray.h"

class material;

struct hit_record
{
	float t;
	vec3 p;
	vec3 normal;
	material *mat_ptr;
};

class hitable
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;

};


#endif // ! __HITABLE_H_

