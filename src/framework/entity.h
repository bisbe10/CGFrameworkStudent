#pragma once
#include <vector>
#include "framework.h"
#include "camera.h"
#include "main/includes.h"
#include "mesh.h"

// DEFINICIÓ DE LA CLASE ENTITY _____________________________________________________________________
class Entity{
    bool points=true;
    bool triangles_r=true;

public:
    Mesh *mesh;
    Matrix44 m_matrix;
    
    Entity();
    Entity(Mesh* m,Matrix44 mm);
    Entity(Mesh* m);
    void Render(Image* framebuffer, Camera* camera, const Color& c);
};
//PARTICULES:
//___________________________PRACTICA 1 DEF CLASES (FORA DEF  D'IMAGE)__________________________________________


// DEFINICIÓ DE LA CLASE Particlesystem _________________________________________________________

//class ParticleSystem {
//
//        static const int MAX_PARTICLES = 970;
//
//        struct Particle {
//                Vector2 position;
//                Vector2 velocity; // Normalized speed and direction of the particle
//                Color color;
//                float acceleration;
//                float ttl; // Time left until the particle expires
//                bool inactive; // Particle is not used/expired, so it can be recreated
//        };
//
//        Particle particles[MAX_PARTICLES];
//
//public:
//        void Init();//inicialitza
//        void Render(Image* framebuffer);//imprimeix per pantalla
//        void Update(float dt);//actualitza
//};

