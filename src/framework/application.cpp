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
    
    //MESH 1
    anna = new Mesh();
    anna->LoadOBJ("meshes/anna.obj");
    
    cleo = new Mesh();
    cleo->LoadOBJ("meshes/cleo.obj");
    
    //MATRIU 1
    //Matrix44(){ setIdentitiy() { Assigna la matriu identitat.
    mtx1=Matrix44();
    mtx2=Matrix44();
    mtx2.SetIdentity();
    
    
    //ENTITAT 1
    entity1 = Entity(anna,mtx1);
    entity2 = Entity(anna,mtx1);
    //CAMERA 1
    
    //DEFINIM UNA CAMERA ->
    
    //CONSTRUCTOR -> view_matrix.SetIdentity();
    
    cam1= new Camera();
    //cam2= new Camera();
    //vectors que defineixen la camera
    // Eye hauriem de canvi
    eye = Vector3(0.5, 1, 1);
    cen = Vector3(0.0, 0.0 ,0.0);
    //up esta definit application.h ja que sempre és (0,1,0)
    eye2 = Vector3(0.5, 1, 2);
    cen2 = Vector3(0.0, 0.0 ,0.0);
    //Dins LookAt trobem UpdateViewMatrix() on dincs d'aqeusta hi ha Utilitzem el SetExampleViewMatrix()
    cam1->LookAt(eye, cen, up);
    //cam2->LookAt(eye2, cen2, up);
    //void Camera::SetPerspective(float fov, float aspect, float near_plane, float far_plane)
    //float aspect; Aspect -> (width/height)
    
    
    // si no ho possem en graus peta
    cam1->SetPerspective(45, (float(framebuffer.width)/framebuffer.height), 0.01, 100.0);
    
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //__________________________________PRACTICA 1_________________________________________________
//    ps.Init();
//
//    framebuffer.Fill(Color::BLACK);
//    //CODI PER CARREGAR LES IMATGES AL INICIALITZAR L'APP
//    // carreguem dins de la variable toolbar (de tipus Imatge) la imatge toolbar alhora que comprobem si aquesta ultima existeix.
//    toolbar.LoadPNG("images/toolbar.png", false);
//    // MOSTREM LES IMATGES DELS BOTONS DEMANATS PER PANTALLA
//
//    // Guardem les imatges dins de les variabwles
//    line.LoadPNG("images/line.png", false);
//    circle.LoadPNG("images/circle.png", false);
//    triangle.LoadPNG("images/triangle.png", false);
//    rectangle.LoadPNG("images/rectangle.png", false);
//    fill.LoadPNG("images/fill.png", false);
//    clear.LoadPNG("images/clear.png", false);
//    load.LoadPNG("images/load.png", false);
//    eraser.LoadPNG("images/eraser.png", false);
//
//    //Guardem les imatges dels colors dins de les variables
//    red.LoadPNG("images/red.png", false);
//    black.LoadPNG("images/black.png", false);
//    cyan.LoadPNG("images/cyan.png", false);
//    blue.LoadPNG("images/blue.png", false);
//    pink.LoadPNG("images/pink.png", false);
//    green.LoadPNG("images/green.png", false);
//    groc.LoadPNG("images/groc.png", false);
    //__________________________________________________________________________________________________
    
}






//CODI NOSTRE EXERCICIS----------------------------(RENDER())--------------------------


void Application::Render(void)
{
  //                    ------------------PRACTICA 2-----------------
    
    entity1.Render(&framebuffer,cam1, Color::WHITE);
    entity2.Render(&framebuffer,cam1, Color::BLUE);
    framebuffer.Render();
    
   
    
    
    
    
    
    
    
    
    
    
    
    
    
    //__________________________________PRACTICA 1_________________________________________________
//    if(Exercici_1){
//        //framebuffer.Fill(Color::BLACK);
//        framebuffer.DrawLineDDA(this->window_width/2, this->window_height/2, this->window_width/2+100, this->window_height/2+100,  Color::RED);
//
//
//    }else if(Exercici_2){
//        framebuffer.Fill(Color::BLACK);
//        framebuffer.DrawRect(this->window_width / 2, this->window_height / 2, 500, 200, Color::RED, 10+borde, fill_cnt, Color::BLUE);
//
//    }else if(Exercici_3){
//            framebuffer.DrawCircle(this->window_width / 2, this->window_height / 2, 100, Color::RED,4+borde, fill_cnt,Color::BLUE);
//
//    }else if(Exercici_4){
//        framebuffer.DrawTriangle(v1,v2,v3,Color::BLUE,fill_cnt,Color::RED);
//
//    }else if(PAINT){
//        int opcio=0;
//        Color color_b= Color::BLUE;
//        bool fill_cnt=false;
//        framebuffer.DrawImage(toolbar, 0, 0, false);
//        //Entrem les imatges al buffer.
//        framebuffer.DrawImage(line, 0, 0, true);
//        framebuffer.DrawImage(circle, 35, 0, true);
//        framebuffer.DrawImage(rectangle, 70, 0, true);
//        framebuffer.DrawImage(clear, 105, 0, true);
//        framebuffer.DrawImage(load, 140, 0, true);
//        framebuffer.DrawImage(eraser, 175, 0, true);
//        framebuffer.DrawImage(fill, 205, 0, true);
//        framebuffer.DrawImage(triangle, 237, 0, true);
//        //Entrem les imatges dels colors al buffer.
//        framebuffer.DrawImage(red, 165, 16, false);
//        framebuffer.DrawImage(groc, 313, 16, false);
//        framebuffer.DrawImage(black, 115, 16, false);
//        framebuffer.DrawImage(green, 215, 16, false);
//        framebuffer.DrawImage(cyan, 415, 16, false);
//        framebuffer.DrawImage(pink, 365, 16, false);
//        framebuffer.DrawImage(blue, 265, 16, false);
//
//
//    }else if(ANIMATION){
//      //si no has vist l'animació es pq comença tant bon punt comença quan s'obre la pantalla del framebuffer
//        ps.Render(&framebuffer);
//
//
//    }else if(F){
//        fill_cnt=true;
//
//    }else if(MASF){
//
//
//    }else if(MENOSF){
//        borde=borde-1;
//    }------------------------------------------------------------------------------------------------
}

//__________________________________________________________________________________________________


// Called after render
void Application::Update(float seconds_elapsed)
{
//    ps.Update(seconds_elapsed);
}



//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
            
            
            
//                    ------------------PRACTICA 2-----------------
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
// __________________________________PRACTICA 1___________________________________________________________
//            //EX 1- DRAW LINES
//        case SDLK_1:
//            framebuffer.Fill(Color::BLACK);
//            Exercici_1 = true;
//            Exercici_2 = false;
//            Exercici_3 = false;
//            Exercici_4 = false;
//            PAINT = false;
//            ANIMATION = false;
//            F = false;
//            MASF = false;
//            MENOSF = false;
//            break;
//
//            //EX 2- DRAW RECT
//        case SDLK_2:
//            framebuffer.Fill(Color::BLACK);
//            Exercici_1 = false;
//            Exercici_2 = true;
//            Exercici_3 = false;
//            Exercici_4 = false;
//            PAINT = false;
//            ANIMATION = false;
//            F = false;
//            MASF = false;
//            MENOSF = false;
//            break;
//
//            //EX 3- DRAW CIRCLE
//        case SDLK_3:
//            framebuffer.Fill(Color::BLACK);
//            Exercici_1 = false;
//            Exercici_2 = false;
//            Exercici_3 = true;
//            Exercici_4 = false;
//            PAINT = false;
//            ANIMATION = false;
//            F = false;
//            MASF = false;
//            MENOSF = false;
//            break;
//
//            //EX 4- DRAW Triangle
//        case SDLK_4:
//            framebuffer.Fill(Color::BLACK);
//            Exercici_1 = false;
//            Exercici_2 = false;
//            Exercici_3 = false;
//            Exercici_4 = true;
//            PAINT = false;
//            ANIMATION = false;
//            F = false;
//            MASF = false;
//            MENOSF = false;
//            break;
//
//            //EX 5- PAINT (CREATIVE MODE)
//        case SDLK_5:
//            framebuffer.Fill(Color::BLACK);
//            Exercici_1 = false;
//            Exercici_2 = false;
//            Exercici_3 = false;
//            Exercici_4 = false;
//            PAINT = true;
//            ANIMATION = false;
//            F = false;
//            MASF = false;
//            MENOSF = false;
//            break;
//
//            //EX 6- DRAW ANIMATION
//        case SDLK_6:
//            framebuffer.Fill(Color::BLACK);
//            Exercici_1 = false;
//            Exercici_2 = false;
//            Exercici_3 = false;
//            Exercici_4 = false;
//            PAINT = false;
//            ANIMATION = true;
//            F = false;
//            MASF = false;
//            MENOSF = false;
//            break;
//
//            //FUNCIÓ FILL
//        case SDLK_f:
//            framebuffer.Fill(Color::BLACK);
//            Exercici_1 = false;
//            Exercici_2 = false;
//            Exercici_3 = false;
//            Exercici_4 = false;
//            PAINT = false;
//            ANIMATION = false;
//            F = true;
//            MASF = false;
//            MENOSF = false;
//
//            break;
//
//            //FUNCIÓ (+)
//        case SDLK_PLUS:
//            framebuffer.Fill(Color::BLACK);
//            borde++;
//
//
//            break;
//
//            //FUNCIÓ (per augmentar el contorn de les formes dibuixades)
//        case SDLK_MINUS:
//            framebuffer.Fill(Color::BLACK);
//            borde--;
           
            
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
        
        
        
        
        
// __________________________________PRACTICA 1___________________________________________________________
                                    //OPCIONS\\
        
        
//       // EXERCICI 1: IMPLEMENT NOSTRE (DRAWLINEDDA 1p) \\ OPCIO 1
//
//        if (line_b->IsMouseInside(mouse_position)==true){
//            opcio=1;
//        }
//
//       // EXERCICI 3: IMPLEMENTACIÓ NOSTRE DE (3.3 Rasterizing Circles (2 points) \\ OPCIO 2
//
//        else if (circle_b->IsMouseInside(mouse_position)==true){
//            opcio=2;
//        }
//
//        // EXERCICI 2: IMPLEMENTACIÓ NOSTRE (Drawing Rectangles (1 point)) \\ OPCIO 3
//
//        else if (rectangle_b->IsMouseInside(mouse_position)==true){
//            opcio=3;
//        }
//
//        // EXERCICI 4 IMPLEMENT NOSTRE DE (DRAWTRIANGLE 2P)\\ OPCIO 4
//
//        else if (triangle_b->IsMouseInside(mouse_position)==true){
//            opcio=4;
//        }
//                            // FILL + CLEAR \\
//
//        else if (fill_b->IsMouseInside(mouse_position)==true){
//            fill_cnt=true;
//        }else if (clear_b->IsMouseInside(mouse_position)==true){
//            framebuffer.Fill(Color::BLACK);
//            opcio=0;
//        }
//                                //COLORS \\
//
//        else if (red_b->IsMouseInside(mouse_position)==true){
//            color_b=Color::RED;
//        }else if (blue_b->IsMouseInside(mouse_position)==true){
//            color_b=Color::BLUE;
//        }else if (cyan_b->IsMouseInside(mouse_position)==true){
//            color_b=Color::CYAN;
//        }else if (green_b->IsMouseInside(mouse_position)==true){
//            color_b=Color::GREEN;
//        }else if (groc_b->IsMouseInside(mouse_position)==true){
//            color_b=Color::YELLOW;
//        }else if (pink_b->IsMouseInside(mouse_position)==true){
//            color_b=Color::PURPLE;
//        }
//
//
//
//        // EXECUCIÓ DE L'OPCIÓ EN LA QUE ESTIGUEM ( si l'usuari no ha pitjat cap boto (else) )
//        else{
//
//            if(opcio==0){
//
//            // Per dibuixar la linia hem de pitxar una vegada per marcar l'inici
//            }else if (opcio==1){
//                    x0_r=this->window_width/2;
//                    y0_r=this->window_height/2;
//                    framebuffer.DrawLineDDA(x0_r,y0_r, mouse_position.x,mouse_position.y, color_b);
//            }else if (opcio==2){
//                framebuffer.DrawCircle(mouse_position.x, mouse_position.y, 100, color_b,4, fill_cnt,Color::BLUE);
//            }else if (opcio==3){
//                framebuffer.DrawRect(mouse_position.x, mouse_position.y, 100, 200, color_b, 8, fill_cnt, Color::BLUE);
//            }else if (opcio==4){
//                framebuffer.DrawTriangle(v1,v2,v3,color_b,fill_cnt,Color::BLUE);
//                //dibuix triangle
//            }
//
//        }
                    //FINAL DE OnKeyPressed( SDL_KeyboardEvent event )\\
        
    }
}




//-------------------------------------------------------------------------------------------------------------

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
