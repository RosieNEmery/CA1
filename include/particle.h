#include <attractor.h>
#include <string>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <memory>

#ifndef PARTICLE_H__
#define PARTICLE_H__

class Particle
{
	private:
    ngl::Vec3 pos;
    ngl::Vec3 vel;
    ngl::Vec3 acc;
    ngl::Vec3 m_force;

    ngl::Vec3 color;

    std::shared_ptr<Attractor> p_attPtr;

    //int size;
    float m_mass;
    float lifetime; //possibly use for shader
    //float currentLife;

		
	public:
    Particle(ngl::Vec3 initial_pos, ngl::Vec3 m_wind, float gravity, std::shared_ptr<Attractor> attractor);
    ~Particle() = default;

    void update();
    bool isAlive();
    void applyForce(ngl::Vec3 force);
    ngl::Vec4 getPosition();
    inline float getAlpha() {return lifetime;}

};

#endif
