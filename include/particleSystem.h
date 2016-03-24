#include <ngl/Vec3.h>
#include <vector>
#include <memory>
#include "particle.h"
#include "attractor.h"

#ifndef PARTICLE_SYSTEM_H__
#define PARTICLE_SYSTEM_H__
class ParticleSystem
{
	private:
    std::vector<Particle> particles;
    int m_nParticles;
    ngl::Vec3 emitter_pos;

    ngl::Vec3 m_wind;
    float m_gravity;

    std::shared_ptr<Attractor> p_attPtr;

	public:
    ParticleSystem(int nparticles, ngl::Vec3 initial_pos, ngl::Vec3 wind, float gravity, std::shared_ptr<Attractor> attractor);
    void update();
		void print();
    float getAlpha(int i);
    void setWind(ngl::Vec3 _wind);
    std::vector<ngl::Vec4> getVector();

};

#endif
