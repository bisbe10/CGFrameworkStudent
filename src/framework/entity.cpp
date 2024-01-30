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

#include "entity.h"


Entity::Entity(){
}

Entity::Entity(Mesh* m,Matrix44 mm){
        this->mesh=m;
        this->m_matrix=mm;
        
}

Entity::Entity(const Entity& e){
        this->mesh=e.mesh;
        this->m_matrix=e.m_matrix;
    
}

Entity::Entity(Mesh* m){
        this->mesh = m;
    
    //Matrix44(){ setIdentitiy() { Assigna la matriu identitat.
        this->m_matrix=Matrix44();
        
}

Entity::Entity(const char* filename ){
        Mesh* mesh1=new Mesh();
        mesh1->LoadOBJ(filename);
        this->mesh= mesh1;
        this->m_matrix=Matrix44();
        
}
void Entity::Render(Image* framebuffer, Camera* camera, const Color& c){
   
    // boolea per assegurar-nos de que els punts estiguin dins del rang de la camera
    bool negZ;
    
    //triangle -> 3 vertices -> 3vectors2 -> és a dir 3 punts = 3 vertex del triangle
    Vector2 V_3a[3]; //V_3a , 3a = triangle
    
    int j; // index
    
    std::vector<Vector3> V_render;
    V_render=mesh->GetVertices();
    
    // Llista dinamica de vertex[3] on cada tres posicions formem un triangle
    for(int i = 0; i < V_render.size(); i = i+3){ //sumem de 3 en 3 per fer-ho en blocs (de triangle en triangle)
        j = 0;
        negZ = false; // si es fals es pinta (true = fora del rang de la camera )
        
        //iteració per a cada vertex
        while(j < 3 && negZ == false){ // cada vertex
            
        // LOCAL SPACE -> WORLD SPACE
        //Passem el vertex extret del V_rende de local space a world space, multiplicant-lo per la model matrix.
            Vector3 V_world = m_matrix*V_render[i+j];
            
            
            
            
            
            
            
            // WORLD SPACE -> mult view_matrix->  VIEW SPACE -> mult projection_matrix -> CLIP SPACE

            // dins de la funció ProjectVector multipliquem el V_world per la viewprojection_matrix.
            // Aquesta surt d'una cadena de funcions desde el constructor a UpdateViewProjectionMatrix(){ -> viewprojection_matrix = view_matrix * projection_matrix}
            
            Vector3 V_clip = camera->ProjectVector(V_world, negZ);
        
            
            // CLIP SPACE -> SCREEN SPACE
            Vector2 V_screen;
            
            // DE LA TEORIA VEIEM QUE X:[0,w-1] Y:[0,h-1]
            
            V_screen.x = ((V_clip .x+1)*0.5)* (framebuffer->width-1);
            V_screen.y = ((V_clip .y+1)*0.5) * (framebuffer->height-1);
            
            V_3a[j].set(V_screen.x, V_screen.y);
            V_3a[j].set(V_screen.x, V_screen.y);
            
            
            //DIBUEIXEM NOMES ELS VERTEX DE LA MESH
                        if(j==2 && negZ==false){
                            framebuffer->SetPixel(V_3a[0].x, V_3a[0].y, Color::RED);
                            framebuffer->SetPixel(V_3a[1].x, V_3a[1].y, Color::RED);
                            framebuffer->SetPixel(V_3a[2].x, V_3a[2].y, Color::RED);
                            
                            
                        }
            
            
            //DIBUEIXEM ELS BORDES (I CONSEGÜENTMENT ELS VERTEX) DE LA MESH
            if(j==2 && negZ==false){
                framebuffer->DrawLineDDA(V_3a[0].x+10, V_3a[0].y+10, V_3a[1].x+10,V_3a[1].y+10,c);
                framebuffer->DrawLineDDA(V_3a[1].x+10, V_3a[1].y+10, V_3a[2].x+10,V_3a[2].y+10,c);
                framebuffer->DrawLineDDA(V_3a[2].x+10, V_3a[2].y+10, V_3a[0].x+10,V_3a[0].y+10,c);
            }
                        
            j=j+1;
        }
    }
}

