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


Entity1::Entity1(){
}

Entity1::Entity1(Mesh* m,Matrix44 mm){
        this->mesh=m;
        this->m_matrix=mm;
        
}
Entity1::Entity1(Entity1& e){
        this->mesh=e.mesh;
        this->m_matrix=e.m_matrix;
        
}
Entity1::Entity1(Mesh* m,Matrix44 mm, Image* t){
    this->mesh = m;
    this->m_matrix=mm;
    this->texture = t;
}
Entity1::Entity1(Mesh* m, Image* t){
        this->mesh = m;
        this->m_matrix= Matrix44();
        this->texture = t;
        
}
Entity1::Entity1(Mesh* m, Image* t, Material* mt){
        this->mesh = m;
        this->m_matrix= Matrix44();
        this->texture = t;
        this->material=mt;
        
    
        
}

Entity1::Entity1(Mesh* m){
        this->mesh = m;
    
    //Matrix44(){ setIdentitiy() { Assigna la matriu identitat.
        this->m_matrix=Matrix44();
        
}


void Entity1::Render(sUniformData Ud){
    //ENABLE
    material->Enable(Ud);
    material->shader->SetFloat("option", Ud.u_option);
   //codi que ja teniem igual a la pràctica anterior.
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glDepthFunc(GL_LEQUAL);
   
   this->mesh->Render();
    
   glDisable(GL_DEPTH_TEST);
    
    
    //DISABLE
    material->Disable();



}
    
void Entity1::Update(float seconds_elapsed, type t){

}
    
    
//void Entity::Render(Image* framebuffer, Camera* camera, const Color& c,const Color& fc,const Color& c3,  FloatImage* zBuffer,int tipus){
//}



