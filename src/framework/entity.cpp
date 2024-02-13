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

#include "entity.h"


Entity::Entity(){
}

Entity::Entity(Mesh* m,Matrix44 mm){
        this->mesh=m;
        this->m_matrix=mm;
        
}
Entity::Entity(Entity& e){
        this->mesh=e.mesh;
        this->m_matrix=e.m_matrix;
        
}
Entity::Entity(Mesh* m,Matrix44 mm, Image* t){
    this->mesh = m;
    this->m_matrix=mm;
    this->texture = t;
}
Entity::Entity(Mesh* m, Image* t){
        this->mesh = m;
        this->m_matrix= Matrix44();
        this->texture = t;
        
}

Entity::Entity(Mesh* m){
        this->mesh = m;
    
    //Matrix44(){ setIdentitiy() { Assigna la matriu identitat.
        this->m_matrix=Matrix44();
        
}


void Entity::Render(Image* framebuffer, Camera* camera, const Color& c,const Color& fc,const Color& c3, rendertype t, FloatImage* zBuffer){
    
    //Fill funció que trobem a la clase FloatImage
    zBuffer->Fill(10000.0); //omplim el z-buffer.
    width_window = framebuffer->width-10;
    // boolea x assegurar-nos de que els punts estiguin dins del rang.
    bool negZ;
    
    //agafem tres posicions de vertices és a dir 3 punts = 3 vertex del triangle
    Vector4 V4_render;
    int j; // index
    
    //Guardem els vertex i UVs de la mesh en variables locals.
    std::vector<Vector3> V_render;
    V_render=mesh->GetVertices();
    
    std::vector<Vector2> V_uvs;
    V_uvs=mesh->GetUVs();
    
    
    Vector2 V_3a[3]; //dues coordenades de l'screen
    
    //drawtriangleinterpolar
    Vector3 V_3inter[3];
    
    // Llista dinamica de vertex[3] on cada tres posicions formem un triangle
    for(int i = 0; i < V_render.size(); i = i+3){ //sumem de 3 en 3 per fer-ho en blocs (de triangle en triangle)
        j = 0;
        negZ = false; // si es fals es pinta (true = fora del rang de la camera )
        
        //iteració per a cada vertex
        while(j < 3 && negZ == false){ // cada vertex
            
            
        // LOCAL SPACE -> WORLD SPACE---------------------------------------------------------------
        //Passem el vertex extret del V_rende de local space a world space, multiplicant-lo per la model matrix.
            
            //passem a v4 per mult per m_matrix
            V4_render = Vector4(V_render[i+j].x, V_render[i+j].y, V_render[i+j].z, 1.0);
            
            Vector4 V4_world = m_matrix*V4_render;
            
            //passem a v3
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

            
   
            j=j+1;

    }
        //            ////DIBUEIXEM NOMES ELS VERTEX DE LA MESH
                    if(t==PO){
                        if( negZ==false){
                            framebuffer->SetPixelSafe(V_3a[0].x, V_3a[0].y, Color::RED);
                            framebuffer->SetPixelSafe(V_3a[1].x, V_3a[1].y, Color::RED);
                            framebuffer->SetPixelSafe(V_3a[2].x, V_3a[2].y, Color::RED);
                        }
                    }
                    // DIBUEIXEM ELS BORDES (I CONSEGÜENTMENT ELS VERTEX) DE LA MESH
                    else if (t==WR && negZ==false){
                        framebuffer->DrawLineDDA(V_3a[0].x, V_3a[0].y, V_3a[1].x,V_3a[1].y,c);
                        framebuffer->DrawLineDDA(V_3a[1].x, V_3a[1].y, V_3a[2].x,V_3a[2].y,c);
                        framebuffer->DrawLineDDA(V_3a[2].x, V_3a[2].y, V_3a[0].x,V_3a[0].y,c);
                    }
                    
                   // DIBUEIXEM ELS TRIANGLES PLENS DE LA MESH AMB UN COLOR C DE BORDE I UN FC DE FILL
                   
                    else if(t==TR && negZ==false){
                        framebuffer->DrawTriangle(V_3a[0], V_3a[1], V_3a[2], c,true ,fc);
                    
                   // DIBUEIXEM ELS TRIANGLES PLENS DE LA MESH AMB ELS COLORS INTERPOLATS
                    }else if(t==TI && negZ==false){
                        
                        V_3inter[0]= Vector3(V_3a[0].x,V_3a[0].y,1);
                        V_3inter[1]= Vector3(V_3a[1].x,V_3a[1].y,1);
                        V_3inter[2]= Vector3(V_3a[2].x,V_3a[2].y,1);
                        
                        
                        //selecionem els 3 V_uvs dels vertex que estem estudiant
                         Vector2 UVs1 = V_uvs[i];  Vector2 UVs2= V_uvs[i+1];  Vector2 UVs3 = V_uvs[i+2];
                         
                        //passem a texturespace
                    
                        //   U(X): 0 a W-1                             V(Y): 0 a H-1
                        UVs1.x = UVs1.x * (texture->width-1);  UVs1.y = UVs1.y * (texture->height-1);
                         
                        UVs2.x = UVs2.x * (texture->width-1);  UVs2.y = UVs2.y * (texture->height-1);
                         
                        UVs3.x = UVs3.x * (texture->width-1);  UVs3.y = UVs3.y * (texture->height-1);
                        
                        
                        framebuffer->DrawTriangleInterpolated(V_3inter[0], V_3inter[1], V_3inter[2], c,fc,c3,zBuffer,texture, UVs1, UVs2, UVs3);
                    
                        
                    }
        }
    

}
    
void Entity::Update(float seconds_elapsed, type t){

    if (!this->inactive) {

        
        if(t==R){
            Vector3 axis= Vector3(0,1,0);
            m_matrix.Rotate(2*DEG2RAD, axis);
            
            
            //sabent superior +0.7 que el limit dret de la pantalla és 1
        }else if(t==T){

            if (m_matrix.M[3][1]>-0.7 && m_matrix.M[3][1]<-0.6){
                restar=false;
            }else if (m_matrix.M[3][1]<0.7 && m_matrix.M[3][1]>0.6){
                restar=true;
            }
            
            if (restar){
                m_matrix.Translate(0,-0.05,0);
            }else{
                m_matrix.Translate(0,+0.05,0);
            }

           
            
        }else if(t==P){
            if (parell==true){
                this->t=TI;
                parell=false;
                std::this_thread::sleep_for(std::chrono::milliseconds(80));

            }else{
                this->t=WR;
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
                parell=true;
            }
            

        }else if(t==RP){
            Vector3 axis= Vector3(0,1,0);
            if (parell==true){
                this->t=TI;
                parell=false;
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
                m_matrix.Rotate(25*DEG2RAD, axis);
                parell=true;
          
            }else{
                this->t=WR;
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
                m_matrix.Rotate(25*DEG2RAD, axis);
                parell=true;

            }
            
    
        }else{
            std::cout << "El tipus seleccionat no existeix" << std::endl;
        }
        
    
    
    
    
    
    }
}
    
    
    


