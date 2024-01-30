#pragma once
#include <vector>
#include "framework.h"
#include "camera.h"
#include "main/includes.h"
#include "mesh.h"

// DEFINICIÓ DE LA CLASE ENTITY _____________________________________________________________________
class Entity{

public:
    Mesh *mesh;
    Matrix44 m_matrix;
    
    Entity();
    Entity(Mesh* m,Matrix44 mm);
    Entity(const Entity& e);
    Entity(Mesh* m);
    Entity(const char* filename);
    void Render(Image* framebuffer, Camera* camera, const Color& c);
};
