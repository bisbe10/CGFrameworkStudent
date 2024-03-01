//
//  entity.cpp
//  ComputerGraphics
//
//  Created by Miquel Bisbe Armengol on 26/1/24.
//

#include "main/includes.h"
#include <map>
#include <string>

#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "image.h"

//includes per poder fer servir l'sleep i que la funció esperi
#include <iostream>
#include <chrono>
#include <thread>

#include "entity1.h"
#include "material.h"

Material::Material(){
}
Material::Material(Shader* s,Texture* t, Vector3 Ka,Vector3 Kd,Vector3 Ks,float sh){
    this->shader=s;
    this->texture=t;
    this->shininess=sh;
    this->KA=Ka;
    this->KD=Kd;
    this->KS=Ks;


}
void Material::Enable(const sUniformData& Ud){
    this->shader->Enable();

    //enviem les dades al gpu (shader)
    this->shader->SetFloat("shine", this->shininess); //shininess

     //opció

    this->shader->SetTexture("u_anna", this->texture); //3D object
    
    //light properties:
    this->shader->SetVector3("ka", KA);
    this->shader->SetVector3("kd", KD);
    this->shader->SetVector3 ("ks", KS);
    
    this->shader->SetMatrix44("u_model", Ud.u_model_matrix);
    this->shader->SetMatrix44("u_viewprojection",Ud.u_view_projection);
    //cam properties
    this->shader->SetVector3("eye", Ud.u_eye);
    this->shader->SetVector3("Ia", Ud.u_Ia);
    this->shader->SetVector3("Is", Ud.u_ligth.is); //is
    this->shader->SetVector3("Id", Ud.u_ligth.id); //id
    this->shader->SetVector3("pos", Ud.u_ligth.pos);
    this->shader->SetVector3("Is1", Ud.u_ligth2.is); //is
    this->shader->SetVector3("Id1", Ud.u_ligth2.id); //id
    this->shader->SetVector3("pos1", Ud.u_ligth2.pos);
    

    
}

void Material::Disable(){
    //cridem el disable de shader
    shader->Disable();
}


