#ifndef PARTICLES_H
#define PARTICLES_H

#include "tools.h"

struct Particle
{
    string textureID;        // Texture
    float size;              // Particle size
    float x, y;              // Spawn position
    float xVel, yVel;        // Velocity
    float life;              // Life (secs)
    uint32_t color;          // Particle color (hex)
};

class ParticlesManager
{
    public:
        ParticlesManager(int maxParticles);
        ~ParticlesManager();

        void update(float deltaTime);
        void render(SDL_Renderer* renderer);
        void spawnParticle(string textureID, float size, float x, float y, float xVel, float yVel, float life, uint32_t color);

    private:
        vector<Particle> particles;
};

extern ParticlesManager particlesManager;


#endif
