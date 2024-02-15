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
    
    //definició del zbuffer
    this->zbuffer.Resize(w,h);
}



Application::~Application()
{
    
    
    
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
    
    anna_t = new Image();
    anna_t->LoadTGA("textures/anna_color_specular.tga");
    
    lee_t = new Image();
    lee_t->LoadTGA("textures/lee_color_specular.tga");
    
    cleo_t = new Image();
    cleo_t->LoadTGA("textures/cleo_color_specular.tga");
    

    
    
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
    entity1 = Entity(anna,anna_t);
    
    entity1.m_matrix.Translate(0, -0.3, 0.5);
    Scale=Matrix44();
    Scale.M[0][0]=2;
    Scale.M[1][1]=2;
    Scale.M[2][2]=2;
    entity1.m_matrix = Scale * entity1.m_matrix;
    
    
    //ENTITAT 2 + formació de la mtx2;
    entity2 = Entity(cleo,mtx2,cleo_t);
    
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
    entity3 = Entity(lee,mtx3,lee_t);
    
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
    this->zbuffer.Fill(10000000.f);
    framebuffer.Fill(Color::BLACK);
  //------------------PRACTICA 2-----------------
        if(C==true){

            
            entity1.t=TR;
            tipus=1;
            entity1.Render(&framebuffer,cam1, Color::WHITE,Color::RED,Color::BLUE, &zbuffer,tipus);

        }else if(C==false&& T==false){
            if (c==1){
                if(Z){
                    tipus=2;
                    entity1.t=TI;
                    entity1.Render(&framebuffer,cam1, Color::GREEN,Color::RED,Color::BLUE, &zbuffer, tipus);
                } else{
                    
                    entity1.t=TI;
                    tipus=1;
                    entity1.Render(&framebuffer,cam1, Color::GREEN,Color::RED,Color::BLUE, &zbuffer,tipus);
                }
            } else if (c==0){
                entity1.t=TR;
                tipus=1;
                entity1.Render(&framebuffer,cam1, Color::WHITE,Color::RED,Color::BLUE, &zbuffer,tipus);
           
            } else { // c=-1 (inicialitzem el framebuffer en pantalla negra)
                framebuffer.Fill(Color::BLACK);
            }
            
        
        } else if(T==true&&C==false){
            entity1.t=TX;
            tipus=3;
            entity1.Render(&framebuffer,cam1, Color::GREEN,Color::RED,Color::BLUE, &zbuffer, tipus);
        }
    

    framebuffer.Render();
}

//__________________________________________________________________________________________________


// Called after render
void Application::Update(float seconds_elapsed)
{
//    if (Exercici_2){
//            entity1.Update(seconds_elapsed, P);
//            entity3.Update(seconds_elapsed, R);
//            entity2.Update(seconds_elapsed, T);
//    }
    framebuffer.Fill(Color::BLACK);


}



//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
            
            
            
//                    ------------------PRACTICA 2-----------------
            
            
//            //EX 1- Intercalar entre (Plain Color/interpolated)
        case SDLK_c:
            framebuffer.Fill(Color::BLACK);
            c=1;
            if (parell_c==true){
                C = true;
                T= false;
                parell_c=false;
            }else{
                C = false;
                T= false;
                parell_c=true;
            }


            break;
//
//            //EX 2- OCCLUSION
        case SDLK_z:
            framebuffer.Fill(Color::BLACK);
            if (parell_z==true){
                Z=true;
                parell_z=false;
            }else{
                Z=false;
                parell_z=true;
            }
            break;
//
//            //EX 3- CAMERA ORTHO
        case SDLK_t:
            c=0;
            if (parell_t==true){
                C = false;
                T= true;
                parell_t=false;
            }else{
                C = false;
                T= false;
                parell_t=true;
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
    cam1->Zoom(dy < 0 ? 1.1 : 0.9);
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
