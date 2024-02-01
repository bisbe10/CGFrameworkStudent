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
    bool dreta=false;
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

    
    
    //                  ------------------PRACTICA 2---------------------------
    
    // ---------------------DEFINICIONS DE VARIABLES I INSTANCIES PER LA PRACTICA 2--------------------------
    
    bool Exercici_1 = false;
    bool Exercici_2 = false;
    bool Ortho_3 = false;
    bool Persp_4 = false;
    bool Near_5 = false;
    bool Far_6 = false;
    bool MASF = false;
    bool MENOSF = false;
    bool Fov_7 = false;
    
    //                                               // MESHES \\
    
    Mesh* anna;
    Mesh* cleo;
    Mesh* lee;
    //                                               // MATRIX \\
    //les matrius estan definides a framework.h
    Matrix44 mtx1;
    Matrix44 mtx2;
    Matrix44 mtx3;
    Matrix44 Scale;
    //Exemples
    
    //                                              // ENTITIES \\
    
    Entity entity1;
    Entity entity2;
    Entity entity3;
    //                                               // CAMERAS\\
    
    Camera* cam1;
    
    float near = 0.0;
    float far = 0.0;
    float fov = 45.0;
    
    Vector3 eye;
    Vector3 cen;
    Vector3 up = Vector3(0.0, 1.0, 0.0) ;
    bool avant= true;
    
    
    
    
    
    
//FINAL DE LA DECLARACIÓ DE LA CLASE APPLICATION
};
    
    
    
    
    
    
    
    
    
    
    
    //_________________________________________________________________________________________________________
    
    
    
    

    


