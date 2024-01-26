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
    
    framebuffer.Fill(Color::BLACK);
    //CODI PER CARREGAR LES IMATGES AL INICIALITZAR L'APP
    // carreguem dins de la variable toolbar (de tipus Imatge) la imatge toolbar alhora que comprobem si aquesta ultima existeix.
    
    if(toolbar.LoadPNG("images/toolbar.png", false)){
        std::cout << "Image found!" << std::endl;
    }else{
        std::cout << "Image not found!" << std::endl;
        
    }
    // MOSTREM LES IMATGES DELS BOTONS DEMANATS PER PANTALLA
    
    // Guardem les imatges dins de les variabwles
    line.LoadPNG("images/line.png", false);
    circle.LoadPNG("images/circle.png", false);
    triangle.LoadPNG("images/triangle.png", false);
    rectangle.LoadPNG("images/rectangle.png", false);
    fill.LoadPNG("images/fill.png", false);
    clear.LoadPNG("images/clear.png", false);
    load.LoadPNG("images/load.png", false);
    eraser.LoadPNG("images/eraser.png", false);
    
    //Guardem les imatges dels colors dins de les variables
    red.LoadPNG("images/red.png", false);
    black.LoadPNG("images/black.png", false);
    cyan.LoadPNG("images/cyan.png", false);
    blue.LoadPNG("images/blue.png", false);
    pink.LoadPNG("images/pink.png", false);
    green.LoadPNG("images/green.png", false);
    
    
    
//    for(int i = 0; i < 100; i++){
//            state[i].pos_x = rand()%framebuffer.width+1;
//            state[i].pos_y = rand()%framebuffer.height+1;
//            state[i].size = rand()%10;
//            state[i].vel.x = 0.3;
//            state[i].vel.y = 0.6;
//  }
}






//CODI NOSTRE EXERCICIS----------------------------(RENDER())--------------------------
// Render one frame


void Application::Render(void)
{
    
//      EXERCICI 1: IMPLEMENT NOSTRE (DRAWLINEDDA 1p) \\
    
//        framebuffer.DrawLineDDA(x, y, x + 200* cos(i), y+ 200 * sin(i), Color::BLUE);
    
    
//      EXERCICI 2: IMPLEMENTACIÓ NOSTRE (Drawing Rectangles (1 point)) \\
    
// framebuffer.DrawRect(300, 200, 500, 200, Color::RED, 8, true, Color::BLUE;
    

//      EXERCICI 3: IMPLEMENTACIÓ NOSTRE DE (3.3 Rasterizing Circles (2 points) \\
    
//    framebuffer.DrawCircle(this->window_width / 2, this->window_height / 2, 100, Color::RED,4, true,Color::BLUE);
    
    
//      EXERCICI 4 IMPLEMENT NOSTRE DE (DRAWTRIANGLE 2P)\\
    
    
    
    
//      EXERCICI 5 IMPLEMENT NOSTRE DE (DRAWIMAGE 0.5p)\\
//  definim  una linia de codi per entrar la toolbar en el nostre framebuffer (ha d'estar al (0,0)).
    
    //Entrem la toolbar com a referencia pels nostres butons més petits
    framebuffer.DrawImage(toolbar, 0, 0, false);
    //Entrem les imatges al buffer.
    framebuffer.DrawImage(line, 0, 0, true);
    framebuffer.DrawImage(circle, 35, 0, true);
    framebuffer.DrawImage(rectangle, 70, 0, true);
    framebuffer.DrawImage(clear, 105, 0, true);
    framebuffer.DrawImage(load, 140, 0, true);
    framebuffer.DrawImage(eraser, 175, 0, true);
    framebuffer.DrawImage(fill, 205, 0, true);
    framebuffer.DrawImage(triangle, 237, 0, true);
    //Entrem les imatges dels colors al buffer.
    framebuffer.DrawImage(red, 165, 16, false);
    framebuffer.DrawImage(black, 115, 16, false);
    framebuffer.DrawImage(green, 215, 16, false);
    framebuffer.DrawImage(cyan, 415, 16, false);
    framebuffer.DrawImage(pink, 365, 16, false);
    framebuffer.DrawImage(blue, 265, 16, false);
  
    
    framebuffer.Render();
}

//__________________________________________________________________________________________________


// Called after render
void Application::Update(float seconds_elapsed)
{

}



//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
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

                                //OPCIONS\\
        
        std::cout << "pitjat" << std::endl;
        
       // EXERCICI 1: IMPLEMENT NOSTRE (DRAWLINEDDA 1p) \\ OPCIO 1
        
        if (line_b->IsMouseInside(mouse_position)==true){
            std::cout << "PITJAT line" << std::endl;
            opcio=1;
        }
        
       // EXERCICI 3: IMPLEMENTACIÓ NOSTRE DE (3.3 Rasterizing Circles (2 points) \\ OPCIO 2
        
        else if (circle_b->IsMouseInside(mouse_position)==true){
            std::cout << "PITJAT circle" << std::endl;
            opcio=2;
        }
        
        // EXERCICI 2: IMPLEMENTACIÓ NOSTRE (Drawing Rectangles (1 point)) \\ OPCIO 3
        
        else if (rectangle_b->IsMouseInside(mouse_position)==true){
            std::cout << "PITJAT rectangle" << std::endl;
            opcio=3;
        }
        
        // EXERCICI 4 IMPLEMENT NOSTRE DE (DRAWTRIANGLE 2P)\\ OPCIO 4
        
        else if (triangle_b->IsMouseInside(mouse_position)==true){
            std::cout << "PITJAT triangle" << std::endl;
            opcio=4;
        }
                            // FILL + CLEAR \\
        
        else if (fill_b->IsMouseInside(mouse_position)==true){
            std::cout << "PITJAT fill" << std::endl;
            fill_cnt=true;
        }else if (clear_b->IsMouseInside(mouse_position)==true){
            std::cout << "PITJAT clear" << std::endl;
            framebuffer.Fill(Color::BLACK);
            opcio=0;
        }
                                //COLORS \\
        
        else if (red_b->IsMouseInside(mouse_position)==true){
            std::cout << "PITJAT red" << std::endl;
            color_b=Color::RED;
        }else if (blue_b->IsMouseInside(mouse_position)==true){
            std::cout << "PITJAT blue" << std::endl;
            color_b=Color::BLUE;
        }else if (cyan_b->IsMouseInside(mouse_position)==true){
            std::cout << "PITJAT cyan" << std::endl;
            color_b=Color::CYAN;
        }else if (green_b->IsMouseInside(mouse_position)==true){
            std::cout << "PITJAT green" << std::endl;
            color_b=Color::GREEN;
        }
        
        
        
        // EXECUCIÓ DE L'OPCIÓ EN LA QUE ESTIGUEM ( si l'usuari no ha pitjat cap boto (else) ) 
        else{
            
            if(opcio==0){
                
            // Per dibuixar la linia hem de pitxar una vegada per marcar l'inici
            }else if (opcio==1){
                    x0_r=this->window_width/2;
                    y0_r=this->window_height/2;
                    framebuffer.DrawLineDDA(x0_r,y0_r, mouse_position.x,mouse_position.y, color_b);
            }else if (opcio==2){
                framebuffer.DrawCircle(mouse_position.x, mouse_position.y, 100, color_b,4, fill_cnt,Color::BLUE);
            }else if (opcio==3){
                framebuffer.DrawRect(mouse_position.x, mouse_position.y, 100, 200, color_b, 8, fill_cnt, Color::BLUE);
            }else if (opcio==4){
                //dibuix triangle
            }
            
        }
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
