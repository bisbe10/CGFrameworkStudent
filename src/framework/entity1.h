#pragma once
#include <vector>
#include "framework.h"
#include "camera.h"
#include "main/includes.h"
#include "mesh.h"


// DEFINICIÓ DE LA CLASE ENTITY _____________________________________________________________________

enum type{R,T,S,P,RP};
enum rendertype{PO,WR,TR,TI,TX};




class Entity1{

public:

//    eRenderMode mode;
    
    //Variable per a una animació
    rendertype t=TI;
    bool parell=true;
    bool inactive=false; //activitat de l'animació
    float width_window;
    bool restar =true;
    
    
    
    //TEXTURA
    Image *texture;
    Mesh *mesh;
    Matrix44 m_matrix;
    
    //DIF CONSTRUCTORS
    Entity1();
    Entity1(Mesh* m,Matrix44 mm);
    Entity1(Mesh* m, Image* t);
    Entity1(Mesh* m,Matrix44 mm, Image* t);
    Entity1(Entity1& e);
    Entity1(Mesh* m);
    
    void Render(Camera* cam);
    void Update(float seconds_elapsed, type t);//afegim tipo t per determinar el tipus de moviment
};

