#pragma once

#include <glm/vec3.hpp>

#define M_PI       3.14159265358979323846   // pi
double drand48();
glm::vec3 random_in_unit_sphere();
glm::vec3 random_in_unit_disk();
glm::vec3 reflect(const glm::vec3 & v, const glm::vec3 & n);
float schlick(float cosine, float ref_idx);
bool refract(const glm::vec3 & v, const glm::vec3 & n, float ni_over_nt, glm::vec3 & refracted);
