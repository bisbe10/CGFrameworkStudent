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
    g_shader= new Shader();
    g_shader = Shader::Get("shaders/gouraud.vs", "shaders/gouraud.fs");
    
    
    
    p_shader= new Shader();
    p_shader = Shader::Get("shaders/phong.vs", "shaders/phong.fs");
    
    ps_shader= new Shader();
    ps_shader = Shader::Get("shaders/phong.vs", "shaders/phong_s.fs");
    pn_shader= new Shader();
    pn_shader = Shader::Get("shaders/phong.vs", "shaders/phong_n.fs");

    //quad.CreateQuad();
    
    //creem una anna_imatge i una anna_textura per utilitzar una dins d'entity i l'altre per passar-la com a parametre a la gpu
    anna_i = new Image();
    anna_i->LoadTGA("textures/anna_color_specular.tga");

    anna_t= new Texture();
    anna_t= Texture::Get("textures/anna_color_specular.tga");
    
    anna_tn= new Texture();
    anna_tn= Texture::Get("textures/anna_normal.tga");


    anna_m.LoadOBJ("meshes/anna.obj");
    


    
    
    //CAMERA 1
    
    //DEFINIM UNA CAMERA ->
    
    //CONSTRUCTOR -> view_matrix.SetIdentity();
    cam1= new Camera();
 
    //vectors que defineixen la camera
    Vector3 eye = Vector3(0.0, 0.0,1.0);
    Vector3 cen= Vector3(0,0,0);
    Vector3 up= Vector3(0,1,0);
    //up esta definit application.h ja que sempre és (0,1,0)
    //Dins LookAt trobem UpdateViewMatrix() on dincs d'aqeusta hi ha Utilitzem el SetExampleViewMatrix()
    cam1->LookAt(eye, cen, up);
    cam1->SetPerspective(45, (float(this->window_width)/this->window_height), 0.01, 100);
    
    this->time = 1.f;
    this->option = 0.0;
    this->llum = 2.0;
    this->ex = 0.0;
    //__________________________________PRACTICA 5_________________________________________________
    

        //               shader   text  colors: ka (ambient)      kd  (llum)     ks (reflexe)
        mg = new Material(g_shader, anna_t,Vector3(0.9,0.3,0.3),  Vector3(1,1,1), Vector3(5,0.9,0.9),  50);
        mp = new Material(p_shader, anna_t, Vector3(0.9,0.3,0.3),  Vector3(1,1,1), Vector3(5,0.9,0.9),  50);
        mp_s = new Material(ps_shader, anna_t,Vector3(0.9,0.3,0.3),  Vector3(1,1,1), Vector3(5,0.9,0.9),  50);
        mp_n = new Material(pn_shader, anna_tn, Vector3(0.9,0.3,0.3),  Vector3(1,1,1), Vector3(5,0.9,0.9),  50);
        mp_t = new Material(pn_shader, anna_t, Vector3(0.9,0.3,0.3),  Vector3(1,1,1), Vector3(5,0.9,0.9),  50);

  //LLUM PRINCIPAL -> L1
        l1.is= Vector3(0.3, 0.3, 2.25); //is
        l1.id = Vector3(3, 3, 5.25); //intesitat de la llum que passo
        
        //posició llum
        l1.pos = Vector3(2, 1.5, 0.2);
    
    //LLUM SECUNDARIA  -> L2
    l2.is= Vector3(0.3, 0.3, 10.25); //is
    l2.id = Vector3(10.25, 3, 3); //intesitat de la llum que passo

    //posició llum
    l2.pos = Vector3(-2, 1.5, 0.2);
    
    //LLUM APAGADA  -> L3
    l3.is= Vector3(0,0,0); //is
    l3.id = Vector3(0,0,0); //intesitat de la llum que passo

    //posició llum
    l3.pos = Vector3(-2, 1.5, 0.2);
        
    //LLUM AMBIENT  -> IA
        Ia = Vector3(0.5);


}





//CODI NOSTRE EXERCICIS----------------------------(RENDER())--------------------------


void Application::Render(void){

//OMPLIM LA UNIFORM STRUCT
    sUnif.u_Ia = Ia;
    sUnif.u_model_matrix = anna_e.m_matrix;
    sUnif.u_view_projection = cam1->viewprojection_matrix;
    sUnif.u_eye = cam1->eye;
    sUnif.u_option = option;
    sUnif.u_ligth= l1;
    sUnif.u_ligth2= l2;
    
    //GOURAUD.
    if(ex==1.0){
        this->anna_e = Entity1(&anna_m,anna_i,mg);
        anna_e.m_matrix.Translate(0, -0.2, 0);
        if (llum==2){
            sUnif.u_ligth2= l2;
        }else{
            sUnif.u_ligth2= l3;
        }
        if (llum==2){
            sUnif.u_ligth2= l2;
        }else{
            sUnif.u_ligth2= l3;
        }
        anna_e.Render(sUnif);

    //PHONG.
    }else if (ex==2) {
        if (option==0){
            this->anna_e = Entity1(&anna_m,anna_i,mp);
        }else if(option==2){
            sUnif.u_Ia = 1;
            this->anna_e = Entity1(&anna_m,anna_i,mp_s);
            
        }else if(option==1){
            this->anna_e = Entity1(&anna_m,anna_i,mp_t);
            
        }else if(option==3){

                this->anna_e = Entity1(&anna_m,anna_i,mp_n);

            }
        anna_e.m_matrix.Translate(0, -0.2, 0);
        if (llum==2){
            sUnif.u_ligth2= l2;
        }else{
            sUnif.u_ligth2= l3;
        }
        anna_e.Render(sUnif);


    }


    
    
    
    
}

//__________________________________________________________________________________________________


// Called after render
void Application::Update(float seconds_elapsed)
{


}


//MANUAL
// 1 / 2 -> amb la tecla 1 surt una llum /amb la tecla 2 surten dues llums.
// G / P -> amb la tecla g apreix l'entitat amb gouraud /amb la tecla p apreix l'entitat amb Phong.
// C -> controla si es pot o no utilitzar textures | Si C ESTA FALSE NO FUNCIONA NI S NI N
//                                                 | TRUE -> COLOR TEXTURA
                                    //QUAN C ESTA TRUE\\
// S -> controla si la textura especular| TRUE -> spec  FALSE -> COLOR TEXTURA
// N -> controla si la textura normal| TRUE -> norm  FALSE -> COLOR TEXTURA

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_g: ex=1.0;  break;
        case SDLK_p: ex=2.0; break;

        case SDLK_1: llum=1.0; break;
        case SDLK_2: llum=2.0; break;
            
        case SDLK_c:
            if (b_texture==false){
                b_texture=true;
                option=1;
            }else{
                b_texture=false;
                option=0;
            }
            break;
        
        case SDLK_s:
            if (b_texture==true&&s_texture==false){
                option= 2.0;
                s_texture=true;
            } else if(b_texture==true&&s_texture==true){
                option=1.0;
                s_texture=false;
            } else if(b_texture==false){
                option=0.0;
            }
            break;
            
        case SDLK_n:
            if (b_texture==true&&n_texture==false){
                option= 3.0;
                n_texture=true;
            }else if(b_texture==true&&n_texture==true){
                option=1.0;
                n_texture=false;
            }else if(b_texture==false){
                option=0.0;
            }
            break;
        
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
    float dy=event.preciseY;
    cam1->Zoom(dy<0 ?1.1 : 0.9);
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
