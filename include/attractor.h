#include <ngl/Vec3.h>

#ifndef _ATTRACTOR_H__
#define _ATTRACTOR_H__

class Attractor
{
  private:
    ngl::Vec3 a_position;
    int a_mass;

  public:
    inline Attractor( ngl::Vec3 position, int mass):
           a_position(position), a_mass(mass) {}

    ~Attractor() = default;

    ngl::Vec3 getForce(ngl::Vec3 particle_pos, float particle_mass);

};

#endif
