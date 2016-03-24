#include "attractor.h"

ngl::Vec3 Attractor::getForce(ngl::Vec3 particle_pos,float particle_mass)
{

  ngl::Vec3 force = a_position - particle_pos;
  float distance = force.length();

  //constrain distance
  if (distance>100) distance = 100;
  if (distance<5) distance = 5;

  //2000 = mass of attractor, can change later - create attractor class?
  float magnitude = (a_mass * particle_mass)/ (distance*distance);

  force.normalize();
  force*=magnitude;

  return force;

}
