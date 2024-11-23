/* date = September 24th 2021 6:15 am */

#ifndef PARTICLES_H
#define PARTICLES_H
#include <common.h>
#include <types.h>

typedef struct
{
    vec2 position;
    vec3 color;
    vec2 velocity;
    float life;
}Particle;

/*
* Check from last known index if particle is dead else override 0
*/
int FindLastDeadParticle(Particle particles[], int particle_count, int *last_dead_particle)
{
    for(int i=0; i< particle_count; i++)
    {
        if(particles[i].life <=0)
        {
            *last_dead_particle = i;
            return i;
        }
    }
    *last_dead_particle = 0;
    return 0;
}

void InitParticles(Particle particles[], int particle_count)
{
    for(int i=0;i<particle_count;i++)
    {
        particles[i].position[0] = 0.0f;
        particles[i].position[1] = 0.0f;
        particles[i].life = float_rand(0.25f, 1.0f);
        
        particles[i].velocity[0] = float_rand(-0.009, 0.009);
        particles[i].velocity[1] = float_rand(-0.009, 0.009);
        
        particles[i].color[0] = float_rand(0,1.0f);
        particles[i].color[1] = float_rand(0,1.0f);
        particles[i].color[2] = float_rand(0,1.0f);
        
    }
}

void RespawnParticle(Particle *particle)
{
    particle->position[0] = 0.0f;
    particle->position[1] = 0.0f;
    particle->life = float_rand(0.25f, 1.0f);
    
    particle->color[0] = float_rand(0.0f, 1.0f);
    particle->color[1] = float_rand(0.0f, 1.0f);
    particle->color[2] = float_rand(0.0f, 1.0f);
    
}

void UpdateParticles(
                     Particle particles[], 
                     size_t particle_count, 
                     int particles_per_frame, 
                     GLfloat deltaTime, 
                     float gravity, 
                     int *last_dead_particle
                     )
{
    //update particles
    for(int i=0; i<particle_count; i++)
    {
        // reduce life 
        particles[i].life -= 1 * deltaTime;
        
        //add velocity and clamp it
        particles[i].position[0] += particles[i].velocity[0];
        particles[i].position[1] += particles[i].velocity[1];
        
        if(particles[i].velocity[0] > 0.30) particles[i].velocity[0] = float_rand(-0.5, 0.5) * deltaTime;
        if(particles[i].velocity[1] > 0.30) particles[i].velocity[1] = float_rand(-0.5, 0.5) * deltaTime;
        
        //clamp borders 
        if(particles[i].position[0] > 1) particles[i].position[0] = 1.0f, particles[i].velocity[0] += -0.005;
        if(particles[i].position[1] > 1) particles[i].position[1] = 1.0f, particles[i].velocity[1] += -0.005;
        if(particles[i].position[0] < -1) particles[i].position[0] = -1.0f, particles[i].velocity[0] += 0.005;
        if(particles[i].position[1] < -1) particles[i].position[1] = -1.0f, particles[i].velocity[1] += 0.005;
        
        // gravity  
        particles[i].velocity[1] += gravity;
        
    }
    
    // add new particles
    for(int i = 0; i < particles_per_frame; i++)
    {
        int dead_particle = FindLastDeadParticle(particles, particle_count, last_dead_particle);
        RespawnParticle(&particles[*last_dead_particle]);
    }
}


#endif //PARTICLES_H
