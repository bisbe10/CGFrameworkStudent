#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "entity1.h"


Application::Application(const char* caption, int width, int height)
{
    
    //es crea la finestra 
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;

    // es crea un framebuffer tenint en compte la allargada i amplada de la finestra
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);


}



Application::~Application()
{
    
    
    
}

                              //INIT DE L'APLICACIÓ\\

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
    
    
    //quan jo inicialitzo un shader necesitem dos rutes
    shader= new Shader();
    shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");

    quad.CreateQuad();
    
    //textura:
    fruites= new Texture();
    fruites= Texture::Get("images/fruits.png");
   
    //creem una anna imatge i una anna textura per utilitzar una dins d'entity i l'altre per passar-la com a parametre a raster.vs.
    anna_i = new Image();
    anna_i->LoadTGA("textures/anna_color_specular.tga");

    anna_t= new Texture();
    anna_t= Texture::Get("textures/anna_color_specular.tga");


    anna_m.LoadOBJ("meshes/anna.obj");
    
    // ENITIY

    this->anna_e = Entity1(&anna_m,anna_i);

    
    
    anna_e.m_matrix.Translate(0, -0.2, 1.3);

    
    
    //CAMERA 1
    
    //DEFINIM UNA CAMERA ->
    
    //CONSTRUCTOR -> view_matrix.SetIdentity();
    cam1= new Camera();
 
    //vectors que defineixen la camera
    Vector3 eye = Vector3(0, 0, 2);
    Vector3 cen= Vector3(0,0,0);
    Vector3 up= Vector3(0,1,0);
    //up esta definit application.h ja que sempre és (0,1,0)
    
    //Dins LookAt trobem UpdateViewMatrix() on dincs d'aqeusta hi ha Utilitzem el SetExampleViewMatrix()
    cam1->LookAt(eye, cen, up);
    
    // Si no ho possem en graus peta
        cam1->SetPerspective(45, (float(this->window_width)/this->window_height), 0.01, 100);
    
    //__________________________________PRACTICA 4_________________________________________________
    
    this->time = 1.f;
    this->option = 0.0;
    this->ex = 0.0;
    
}





//CODI NOSTRE EXERCICIS----------------------------(RENDER())--------------------------


void Application::Render(void)
{
    shader->Enable();
   
    if (ex==4.0){
        
        shader->SetMatrix44("u_model", anna_e.m_matrix);
        shader->SetMatrix44("u_viewprojection", cam1->viewprojection_matrix);
        shader->SetTexture("u_anna",anna_t);
        anna_e.Render(cam1);
        
    }else {
        shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");
        shader->SetTexture("u_fruites",fruites);
        shader->SetFloat("time",time);
        shader->SetFloat("option",option);
        shader->SetFloat("ex",ex);
        quad.Render();
    }
    
    
    shader->Disable();
    
    
}

//__________________________________________________________________________________________________


// Called after render
void Application::Update(float seconds_elapsed)
{
    if (ex==3.0&&option==1.0){

        time*=8;
    }else if(ex==3.0&&option==2.0){
        time*=2;
    }
    if (ex==4.0){
        shader=Shader::Get("shaders/raster.vs", "shaders/raster.fs");
    }

}



//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_1: ex=1.0;  break;
        case SDLK_2: ex=2.0; break;
        case SDLK_3: ex=3.0; break;
        case SDLK_4: ex=4.0; break;
        case SDLK_a: option=1.0; break;
        case SDLK_b: option=2.0; break;
        case SDLK_c: option=3.0; break;
        case SDLK_d: option=4.0; break;
        case SDLK_e: option=5.0; break;
        case SDLK_f: option=6.0; break;
        
    }
                //FINAL DE OnKeyPressed( SDL_KeyboardEvent event )\\
            
}



void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}


//-------------------------------------------------------------------//OPCIONS\\----------------------------

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    

}

//-------------------------------------------------------------------------------------------------------------

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
   
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
