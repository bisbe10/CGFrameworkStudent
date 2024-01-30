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

Entity::Entity(Mesh* m){
        this->mesh = m;
    
    //Matrix44(){ setIdentitiy() { Assigna la matriu identitat.
        this->m_matrix=Matrix44();
        
}


void Entity::Render(Image* framebuffer, Camera* camera, const Color& c){
   
    // boolea x assegurar-nos de que els punts estiguin dins del rang.
    bool negZ;
    
    //agafem tres posicions de vertices és a dir 3 punts = 3 vertex del triangle
    Vector4 V4_render;
    int j; // index
    std::vector<Vector3> V_render;
    V_render=mesh->GetVertices();
    
    
    Vector2 V_3a[3]; //dues coordenades de l'screen
    // Llista dinamica de vertex[3] on cada tres posicions formem un triangle
    for(int i = 0; i < V_render.size(); i = i+3){ //sumem de 3 en 3 per fer-ho en blocs (de triangle en triangle)
        j = 0;
        negZ = false; // si es fals es pinta (true = fora del rang de la camera )
        
        
        
        
        //iteració per a cada vertex
        while(j < 3 && negZ == false){ // cada vertex
            
            
            
        // LOCAL SPACE -> WORLD SPACE---------------------------------------------------------------
            
        //Passem el vertex extret del V_rende de local space a world space, multiplicant-lo per la model matrix.
            V4_render = Vector4(V_render[i+j].x, V_render[i+j].y, V_render[i+j].z, 1.0);
            
            Vector4 V4_world = m_matrix*V4_render;
            
        
            Vector3 V3_world = Vector3(V4_world.x, V4_world.y, V4_world.z);
            
            
            
            
            
            
            
// WORLD SPACE -> mult view_matrix->  VIEW SPACE -> mult projection_matrix -> CLIP SPACE------------------------
            

        // dins ProjectVector multipliquem el V_world per la viewprojection_matrix.
            
            Vector3 V_clip = camera->ProjectVector(V3_world, negZ);
        
            
// CLIP SPACE -> SCREEN SPACE---------------------------------------------------------------------------
            
            Vector2 V_screen;
            
            
        // Inicialment tenim X:[-1,1] Y:[-1,1] de clip -> +1 -> X:[0,2] Y:[0,2] -> /2 i mult w-1 i h-1 respectivament     -> Volem arribar a X:[0,w-1] Y:[0,h-1]
            
            V_screen.x = ((V_clip.x+1)*0.5)* (framebuffer->width-1);
            V_screen.y = ((V_clip.y+1)*0.5) * (framebuffer->height-1);
            
            V_3a[j].set(V_screen.x, V_screen.y);

            
            if(points==true){
                if(j==2 && negZ==false){
                    framebuffer->SetPixelSafe(V_3a[0].x, V_3a[0].y, Color::RED);
                    framebuffer->SetPixelSafe(V_3a[1].x, V_3a[1].y, Color::RED);
                    framebuffer->SetPixelSafe(V_3a[2].x, V_3a[2].y, Color::RED);
                    
                    
                }
            }
        //DIBUEIXEM NOMES ELS VERTEX DE LA MESH
            
            
           // DIBUEIXEM ELS BORDES (I CONSEGÜENTMENT ELS VERTEX) DE LA MESH
            if(j==2 && negZ==false &&triangles_r==true){
                framebuffer->DrawLineDDA(V_3a[0].x+10, V_3a[0].y+10, V_3a[1].x+10,V_3a[1].y+10,c);
                framebuffer->DrawLineDDA(V_3a[1].x+10, V_3a[1].y+10, V_3a[2].x+10,V_3a[2].y+10,c);
                framebuffer->DrawLineDDA(V_3a[2].x+10, V_3a[2].y+10, V_3a[0].x+10,V_3a[0].y+10,c);
            }
                        
            j=j+1;
        }
    }
}

