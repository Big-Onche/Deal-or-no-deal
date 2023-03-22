#include "main.h"
#include "particles.h"
#include "textures.h"

const int maxParticles = 1024;

ParticlesManager particlesManager(maxParticles);

ParticlesManager::ParticlesManager(int maxParticles) { particles.reserve(maxParticles); }

ParticlesManager::~ParticlesManager() {}

void ParticlesManager::update(float deltaTime)
{
    for (auto it = particles.begin(); it != particles.end();)
    {
        it->x += it->xVel * deltaTime;
        it->y += it->yVel * deltaTime;
        it->life -= deltaTime;

        if (it->life <= 0.0f) it = particles.erase(it);
        else ++it;
    }
}

void ParticlesManager::render(SDL_Renderer* renderer)
{
    for (const auto& particle : particles)
    {
        TextureManager& textureManager = TextureManager::getInstance();
        textureManager.setColorMod(particle.textureID, particle.color);
        textureManager.drawAlphaTex(particle.textureID, particle.x, particle.y, particle.size, particle.size, renderer, particle.life<0.512f ? particle.life * 128.f : 255);
    }
}

void ParticlesManager::spawnParticle(string textureID, float size, float x, float y, float xVel, float yVel, float life, uint32_t color)
{
    if (particles.size() < particles.capacity()) particles.push_back({textureID, size, x, y, xVel, yVel, life, color});
}
