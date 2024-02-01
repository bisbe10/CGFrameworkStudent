#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

Application::Application(const char* caption, int width, int height)
{
    
    //es crea la finestra 
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
    
    // es crea un framebuffer tenint en compte la allargada i amplada de la finestra
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

    //definició del framebuffer
	this->framebuffer.Resize(w, h);
}



Application::~Application()
{
    
    
    // més codi //SDL_DestroyWindow(window); <- ara mateix aixo esta al main
    
    
}

                              //INIT DE L'APLICACIÓ\\

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
    
    
    
    
    //__________________________________PRACTICA 2_________________________________________________
    
    // ---------------------DEFINICIONS DE VARIABLES I INSTANCIES PER LA PRACTICA 2--------------------------
    
    
    //MESH
    anna = new Mesh();
    anna->LoadOBJ("meshes/anna.obj");
    
    cleo = new Mesh();
    cleo->LoadOBJ("meshes/cleo.obj");
    
    lee = new Mesh();
    lee->LoadOBJ("meshes/lee.obj");
    
    
    //MATRIU
    //Matrix44(){ setIdentitiy() { Assigna la matriu identitat.
    mtx1=Matrix44();
    //Creem una model matrix 2
    mtx2=Matrix44();
    mtx2.SetIdentity();
    //Creem una model matrix 3
    mtx3=Matrix44();
    mtx3.SetIdentity();
   
    
    //ENTITAT 1
    entity1 = Entity(anna);
    
    entity1.m_matrix.Translate(0, -0.3, 0.5);
    Scale=Matrix44();
    Scale.M[0][0]=2;
    Scale.M[1][1]=2;
    Scale.M[2][2]=2;
    entity1.m_matrix = Scale * entity1.m_matrix;
    
    
    //ENTITAT 2 + formació de la mtx2;
    entity2 = Entity(cleo,mtx2);
    
    // els vectors axis defineixen l'eix sobre el qual giren les diferents meshes (els signes estan contraris)
    //modifiquem la mtx2
    Vector3 axis2 = Vector3(0,0.5,0);
    //nomes valors de -1 a 1
    entity2.m_matrix.Translate(1, -0.5, 0);
    entity2.m_matrix.Rotate(PI/6, axis2);

    Scale=Matrix44();
    Scale.M[0][0]=0.9;
    Scale.M[1][1]=0.9;
    Scale.M[2][2]=0.9;
    entity2.m_matrix = Scale * entity2.m_matrix;
   
    
    //ENTITAT 3 + formació de la mtx3;
    entity3 = Entity(lee,mtx3);
    
    Vector3 axis3 = Vector3(0,1,0);
    entity3.m_matrix.Translate(-1, -0.5, 0.5);
    entity3.m_matrix.Rotate(PI/6, axis3);
    
    Scale=Matrix44();
    Scale.M[0][0]=1.2;
    Scale.M[1][1]=1.2;
    Scale.M[2][2]=1.2;
    entity3.m_matrix = Scale * entity3.m_matrix;
    
    
    //CAMERA 1
    
    //DEFINIM UNA CAMERA ->
    
    //CONSTRUCTOR -> view_matrix.SetIdentity();
    cam1= new Camera();
 
    //vectors que defineixen la camera
    eye = Vector3(0, 0, 2);
    cen= Vector3(0,0,0);
    //up esta definit application.h ja que sempre és (0,1,0)
    
    //Dins LookAt trobem UpdateViewMatrix() on dincs d'aqeusta hi ha Utilitzem el SetExampleViewMatrix()
    cam1->LookAt(eye, cen, up);
    
    // Si no ho possem en graus peta
        cam1->SetPerspective(fov, (float(framebuffer.width)/framebuffer.height), 0.01, 2);


}






//CODI NOSTRE EXERCICIS----------------------------(RENDER())--------------------------


void Application::Render(void)
{
  //                    ------------------PRACTICA 2-----------------

    
    
        if(Persp_4){
       
            cam1->SetPerspective(fov, (float(framebuffer.width)/framebuffer.height), 0.001+near, 2+far);
        }
        else if(Ortho_3){
            
            cam1->SetOrthographic(-1,1,1,-1,-1,1);
        }
     
    
        if(Exercici_1){
            //cam1->LookAt(eye, cen, up);
            
            entity1.triangles_r=true;
            entity1.Render(&framebuffer,cam1, Color::WHITE);

        }else if(Exercici_2){

            //cam1->LookAt(eye, cen, up);
        
        
            entity3.points=false;
            entity3.Render(&framebuffer,cam1, Color::RED);
        
            entity1.Render(&framebuffer,cam1, Color::WHITE);
            entity2.points=true;
            entity2.triangles_r=false;
            entity2.Render(&framebuffer,cam1, Color::WHITE);
            
    }
    

    framebuffer.Render();
}

//__________________________________________________________________________________________________


// Called after render
void Application::Update(float seconds_elapsed)
{
    if (Exercici_2){
            entity1.Update(seconds_elapsed, RP);
            entity3.Update(seconds_elapsed, R);
            entity2.Update(seconds_elapsed, T);
    }
    framebuffer.Fill(Color::BLACK);


}



//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
            
            
            
//                    ------------------PRACTICA 2-----------------
            
            
//            //EX 1- DRAW 1 ENTITAT
        case SDLK_1:
            framebuffer.Fill(Color::BLACK);
            Exercici_1 = true;
            Exercici_2 = false;
            Ortho_3 = false;
            Persp_4 = false;
            Near_5 = false;
            Far_6 = false;
            Fov_7 = false;

            break;
//
//            //EX 2- DRAW + d'1 ENTITAT (ANIMADES)
        case SDLK_2:
            framebuffer.Fill(Color::BLACK);
            Exercici_1 = false;
            Exercici_2 = true;
            Ortho_3 = false;
            Persp_4 = false;
            Near_5 = false;
            Far_6 = false;
            Fov_7 = false;

            break;
//
//            //EX 3- CAMERA ORTHO
        case SDLK_o:
            Persp_4 = false;
            Ortho_3 = true;
            break;
//
//            //EX 4- CAMERA PERSPECTIVE
        case SDLK_p:
            Persp_4 = true;
            Ortho_3 = false;
            Near_5 = false;
            Far_6 = false;
            Fov_7 = false;
            break;
//
//            //EX 5- NEAR
        case SDLK_n:
            Persp_4 = true;
            Near_5 = true;
            Far_6 = false;
            Fov_7 = false;
            
            break;
            
//            //EX 6- FAR
        case SDLK_f:
            Persp_4 = true;
            Near_5 = false;
            Far_6 = true;
            Fov_7 = false;
            break;

            //EX 7- FOV
              case SDLK_v:
                  Persp_4 = true;
                  Near_5 = false;
                  Far_6 = false;
                  Fov_7 = true;
                  
                  break;
            
//            //FUNCIÓ (+) CURRENT PROPERTY
        case SDLK_PLUS:
            
            if(Near_5==true){
                this->near += 0.1;

            }else if (Far_6==true){
                this->far += 0.1;
                
            }else if (Fov_7==true){
                this->fov += 1;
            }


            break;
//
//           //FUNCIÓ (-)CURRENT PROPERTY
        case SDLK_MINUS:
            
            if(Near_5==true){
                this->near -= 0.1;
                
            }else if (Far_6==true){

                this->far -= 0.1;

                
            }else if (Fov_7==true){
                this->fov -= 1;
            }

            break;
            
                //FINAL DE OnKeyPressed( SDL_KeyboardEvent event )\\
            
	}
}



void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}


//-------------------------------------------------------------------//OPCIONS\\----------------------------

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    
	if (event.button == SDL_BUTTON_LEFT) {
        dreta=false;


    } else if (event.button == SDL_BUTTON_RIGHT){
        printf("hola dreta");
        dreta=true;
        
   }
}
        
        




//-------------------------------------------------------------------------------------------------------------

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    if (dreta==true){
        
        //definim l'eix sobre el qual volem rotar (y i x respectivament) i l'angle que volem que el centre es mogui, fent el vector que va de l'eye al center i sumant-li la coordenada mouse_delta.x i .y respectivament
        
         cam1->Rotate(mouse_delta.x*0.01,up);

        
         cam1->Rotate(mouse_delta.y*0.01,Vector3(1,0,0));
        
     } else if (event.button == SDL_BUTTON_LEFT) {

         //definim l'eix sobre el qual volem rotar (y i x respectivament) i l'angle que volem que l'eye es mogui, fent el vector que va del center a 'eye' i sumant-li la coordenada mouse_delta.x i .y respectivament
         
        cam1->orbit(mouse_delta.x*0.01,up);

        cam1->orbit(mouse_delta.y*0.01,Vector3(1,0,0));
        
    }
    
   
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
