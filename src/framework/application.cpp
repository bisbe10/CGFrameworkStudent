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
    
    // EXERCICI 1: IMPLEMENT NOSTRE (DRAWLINEDDA 1p) \\
    
//    framebuffer.Fill(Color::BLACK);

//    //podriem demanar el i per pantalla
//    float x=500;
//    float y=300;
//    float i=0;
//    float pi=3.14;
//    while(i<2*pi){
//        framebuffer.DrawLineDDA(x, y, x + 200* cos(i), y+ 200 * sin(i), Color::BLUE);
//        i= i+(pi/4);
//
//    }
    
    
//      EXERCICI 2: IMPLEMENTACIÓ NOSTRE (Drawing Rectangles (1 point)) \\
    
//      framebuffer.DrawRect(300, 200, 500, 200, Color::RED, 8, true, Color::BLUE);
    
    
    
    
    
//      EXERCICI 3: IMPLEMENTACIÓ NOSTRE DE (3.3 Rasterizing Circles (2 points) \\
    
//      framebuffer.DrawCircle(800, 300, 2, Color::RED, false);
    
//      framebuffer.DrawCircle(this->window_width / 2, this->window_height / 2, 100, Color::RED, true);
    
    
    
    
    // EXERCICI 4 IMPLEMENT NOSTRE DE (DRAWTRIANGLE 2P)\\
    
    
    
    
    //EXERCICI 5 IMPLEMENT NOSTRE DE (DRAWIMAGE 0.5p)\\
    //definim  una linia de codi per entrar la toolbar en el nostre framebuffer (ha d'estar al (0,0)).
    framebuffer.DrawImage(toolbar, 0, 0, false);
    
    // MOSTREM LES IMATGES DELS BOTONS DEMANATS PER PANTALLA
    // Def les variables on guardarem les imatges
    Image line;
    Image circle;
    Image rectangle;
    Image triangle;
    Image fill;
    Image clear;
    Image load;
    Image eraser;
    
    // Guardem les imatges dins de les variables
    line.LoadPNG("images/line.png", false);
    circle.LoadPNG("images/circle.png", false);
    rectangle.LoadPNG("images/rectangle.png", false);
    fill.LoadPNG("images/fill.png", false);
    clear.LoadPNG("images/clear.png", false);
    load.LoadPNG("images/load.png", false);
    eraser.LoadPNG("images/eraser.png", false);
    
    //Entrem les imatges al buffer.
    framebuffer.DrawImage(line, 0, 0, true);
    framebuffer.DrawImage(circle, 35, 0, true);
    framebuffer.DrawImage(rectangle, 70, 0, true);
    framebuffer.DrawImage(clear, 105, 0, true);
    framebuffer.DrawImage(load, 140, 0, true);
    framebuffer.DrawImage(eraser, 175, 0, true);
    framebuffer.DrawImage(fill, 205, 0, true);
    framebuffer.Render();
}








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

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        
        // si dins es true el mouse inside vol dri que s'ha fet click
        // comprovar tots els botons

	}
}

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
