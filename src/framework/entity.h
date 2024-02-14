#pragma once
#include <vector>
#include "framework.h"
#include "camera.h"
#include "main/includes.h"
#include "mesh.h"

// DEFINICIÓ DE LA CLASE ENTITY _____________________________________________________________________

enum type{R,T,S,P,RP};
enum rendertype{PO,WR,TR,TI,TX};

//enum eRenderMode{POINTCLOUD,WIREFRAME,TRIANGLES,TRIANGLES_INTERPOLATED,};



class Entity{

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
    Entity();
    Entity(Mesh* m,Matrix44 mm);
    Entity(Mesh* m, Image* t);
    Entity(Mesh* m,Matrix44 mm, Image* t);
    Entity(Entity& e);
    Entity(Mesh* m);
    
    void Render(Image* framebuffer, Camera* camera, const Color& c,const Color& fc,const Color& c3, rendertype t,FloatImage* zBuffer,int tipus);
    void Update(float seconds_elapsed, type t);//afegim tipo t per determinar el tipus de moviment
};

