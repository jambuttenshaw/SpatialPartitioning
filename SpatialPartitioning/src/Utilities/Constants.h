#pragma once

#include "Vector2.h"


// maths
const Vector2f Vector2f::Zero = { 0, 0 };

#define PI 3.1415926f


/*
While it may be annoying to have to access all colliders through an integer ID rather than directly modifying the object itself,
this is so that any changes made to the AABB are guarenteed to be propogated to the spatial partitioning system
*/
using ColliderID = size_t;


