#include "bounding_sphere.h"

BoundingSphere::BoundingSphere(SceneTransform *translation_mat, float radius, int interact_type) 
{
	this->translation_mat = translation_mat;
	this->radius = radius;
	this->interact_type = interact_type;
	this->check_interact = false;
}

BoundingSphere::~BoundingSphere()
{
}