/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
//important incluir la mesh
#include "mesh.h"
#include "entity.h"

class Application
{
public:

    //PARTICLE SYSTEM
//    ParticleSystem ps;
	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;

	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
    
    Image framebuffer;
    
    // Constructor and main methods
    Application(const char* caption, int width, int height);
    ~Application();

    void Init( void );
    void Render( void );
    void Update( float dt );

    // Other methods to control the app
    void SetWindowSize(int width, int height) {
        glViewport( 0,0, width, height );
        this->window_width = width;
        this->window_height = height;
    }

    Vector2 GetWindowSize()
    {
        int w,h;
        SDL_GetWindowSize(window,&w,&h);
        return Vector2(float(w), float(h));
    }

    
    //-----------VARIABLES NECESSARIES PER L'EXERICIC5 [ Create a drawing tool (2.5 points)]----------------
    
    // Variables predeterminades per passar els botons per pantalla
//    int opcio=0;
//    Color color_b= Color::BLUE;
//    bool fill_cnt=false;
//    int recta=0;
//    int x0_r;
//    int y0_r;
//    int borde=0;
//    Vector2 v1={200,100};
//    Vector2 v2={300,200};
//    Vector2 v3={200,500};
//
//    // Definim els diferents exercicis
//    bool Exercici_1 = false;
//    bool Exercici_2 = false;
//    bool Exercici_3 = false;
//    bool Exercici_4 = false;
//    bool PAINT = false;
//    bool ANIMATION = false;
//    bool F = false;
//    bool MASF = false;
//    bool MENOSF = false;
//
//                                                //IMAGES \\
//    // Definicions de les imatges dels botons
//    Image toolbar;
//    Image line;
//    Image circle;
//    Image rectangle;
//    Image triangle;
//    Image fill;
//    Image clear;
//    Image load;
//    Image eraser;
//    //colors
//    Image red;
//    Image groc;
//    Image blue;
//    Image pink;
//    Image green;
//    Image black;
//    Image cyan;
//
//
//                                    //INICIALITZACIÓ BUTTONS \\
//    // Definicions dels botons
//    Button* toolbar_b = new Button( toolbar, 0,0,1280,60,false);
//    Button* line_b = new Button( line, 0,720,32,688,true);
//    Button* circle_b = new Button( circle, 35,720 ,67,688,true);
//    Button* rectangle_b = new Button( rectangle, 70,720 ,102,688,true);
//    Button* triangle_b = new Button( triangle, 237,720 ,269,688,true);
//    Button* clear_b = new Button( clear, 105,720 ,137,688,true);
//    Button* fill_b = new Button( fill, 205,720 ,237,688,true);
//    // Definicions dels botons de colors
//    Button* red_b = new Button( red, 165,16 ,197,48,false);
//    Button* black_b = new Button(black, 115,16 ,147,48,false);
//    Button* green_b = new Button( green, 215,16 ,247,48,false);
//    Button* cyan_b = new Button( cyan, 415,16 ,447,48,false);
//    Button* pink_b = new Button( pink, 365,16 ,397,48,false);
//    Button* blue_b = new Button( blue, 265,16 ,297,48,false);
//    Button* groc_b = new Button( groc, 313,16 ,345,48,false);
//___________________________________________________________________________________________________________
    
    
    
    
    
    //                  //PRACTICA 2\\
    
    // ---------------------DEFINICIONS DE VARIABLES I INSTANCIES PER LA PRACTICA 2--------------------------
    
    //                                               // MESHES \\
    
    Mesh* anna; // PUNTER ??
    
    //                                               // MATRIX \\

    //les matrius estan definides a framework.h
    Matrix44 mtx1;
    //Exemples
    
    //                                              // ENTITIES \\
    
    Entity entity1;
    
    //                                               // CAMERAS\\
    
    Camera* cam1; // PUNTER ?? 
    
    Vector3 eye;
    Vector3 cen;
    Vector3 up = Vector3(0.0, 1.0, 0.0) ;
    
    
    
    
    
//FINAL DE LA DECLARACIÓ DE LA CLASE APPLICATION
};
    
    
    
    
    
    
    
    
    
    
    
    //_________________________________________________________________________________________________________
    
    
    
    

    


