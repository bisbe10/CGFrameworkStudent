#pragma once
#include <vector>
#include "framework.h"
#include "camera.h"
#include "main/includes.h"
#include "mesh.h"
#include "material.h"


// DEFINICIÓ DE LA CLASE MATERIAL_____________________________________________________________________
struct sLight{
    Vector3 pos;
    Vector3 is; // (is, id)
    Vector3 id;
};
struct sUniformData{
    Matrix44 u_model_matrix;
    Matrix44 u_view_projection;
    Vector3 u_Ia; //ambient light intensity
    sLight u_ligth; //es podria guardar com un vec
    sLight u_ligth2;
    Vector3 u_eye;
    float u_option;
   
};

class Material{

public:

    
    //ATRIBUTS
    Shader* shader=nullptr;
    Texture* texture;
    Vector3 KA; // cada posició és una constant color (Ka, Kd, Ks)
    Vector3 KD;
    Vector3 KS;
    float shininess;
    
    
    
    //constructor
    Material();
    Material(Shader* s,Texture* t, Vector3 Ka,Vector3 Kd,Vector3 Ks,float sh );
    //Variable per a una animació
    

    
    void Enable(const sUniformData& Ud);
    void Disable();
};

