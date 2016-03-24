#include "particle.h"

#include <string>
#include <random>
#include <iostream>
#include <ngl/Vec4.h>
#include <ngl/NGLStream.h>


Particle::Particle(ngl::Vec3 initial_pos, ngl::Vec3 m_wind, float gravity,std::shared_ptr<Attractor> attractor):
  pos (initial_pos),
  p_attPtr(attractor)
{
  std::random_device rng;
  std::mt19937 mt_rand(rng());
  std::uniform_real_distribution<float> life(2,200);

  lifetime = life(mt_rand);
  //currentLife = lifetime;

  //creating random number for velocity

  std::uniform_real_distribution<float> dist_x(-1.0,1.0);
  std::uniform_real_distribution<float> dist_y(-2.0,2.0);
  std::uniform_real_distribution<float> dist_z(-2.0,2.0);

  vel[0] = dist_x(mt_rand);
  vel[1] = dist_y(mt_rand);
  vel[2] = dist_z(mt_rand);

   std::uniform_real_distribution<float> _mass(10,20);
   m_mass = _mass(mt_rand);

   applyForce(m_wind);

   applyForce(ngl::Vec3(0.0f,(gravity),0.0f));
}

void Particle::update()
{
  ngl::Vec3 attractForce = p_attPtr->getForce(pos, m_mass);
  applyForce(attractForce);
  vel += acc;
  pos += vel;
  acc.set(0,0,0);

  lifetime -= 2.0;
  //use lifetime for aplha of particles, dissapears as die
}

bool Particle::isAlive()
{
  if (lifetime>0.0) return true;
  else return false;
}

void Particle::applyForce(ngl::Vec3 force)
{

  m_force=(force/m_mass);
  acc+=force;
}

ngl::Vec4 Particle::getPosition()
{
  ngl::Vec4 posOut(pos);
  posOut[3] = lifetime;
  return posOut;

}
