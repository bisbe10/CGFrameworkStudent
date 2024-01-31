#pragma once
#include <vector>
#include "framework.h"
#include "camera.h"
#include "main/includes.h"
#include "mesh.h"

// DEFINICIÓ DE LA CLASE ENTITY _____________________________________________________________________

enum type{R,T,S,P,RP};





class Entity{

public:
    //Variable per a una animació
    bool parell=true;
    bool points=true;
    bool triangles_r=true;
    float ttl=30; //temps de vida estatic
    bool inactive=false; //activitat de l'animació
    float width_window;
    bool restar =true;
    
    
    
    
    
    Mesh *mesh;
    Matrix44 m_matrix;
    
    Entity();
    Entity(Mesh* m,Matrix44 mm);
    Entity(Mesh* m);
    void Render(Image* framebuffer, Camera* camera, const Color& c);
    void Update(float seconds_elapsed, type t);//afegim tipo t per determinar el tipus de moviment
};

