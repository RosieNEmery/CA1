#include "particleSystem.h"
#include <iostream>
#include <ngl/Vec2.h>

ParticleSystem::ParticleSystem(int nparticles, ngl::Vec3 initial_pos, ngl::Vec3 wind, float gravity, std::shared_ptr<Attractor> attractor):
  m_nParticles (nparticles),
  emitter_pos (initial_pos),
  m_wind (wind),
  m_gravity (gravity),
  p_attPtr (attractor)

{

  for (int i = 0; i<m_nParticles;i++)
  {
    particles.push_back(Particle(initial_pos,m_wind,m_gravity,p_attPtr));

	}
}


void ParticleSystem::update()
{
  for (unsigned int i =0; i<particles.size();i++)
    {
      particles[i].update();
      if (particles[i].isAlive() == false)
      {
       //removes particle from vector if dead
        particles.erase(particles.begin() + i);
        particles.push_back(Particle(emitter_pos,m_wind,m_gravity,p_attPtr));


      }
    }
}

std::vector<ngl::Vec4> ParticleSystem::getVector()
{
  std::vector<ngl::Vec4> points;
  for (auto &p: particles)
  {
    points.push_back(p.getPosition());
  }
  return points;
}

float ParticleSystem::getAlpha(int i)
{
  return particles[i].getAlpha();
}

void ParticleSystem::setWind(ngl::Vec3 _wind)
{
  m_wind += _wind;
  for (auto &p:particles)
  {
    p.applyForce(m_wind);
  }
}
