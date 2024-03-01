/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
//important incluir la mesh
#include "mesh.h"
#include "shader.h"
#include "entity1.h"
#include "material.h"


class Application
{
public:


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

    // CPU Global framebuffe
    


    
    Camera* cam1;
    Mesh quad;
    Mesh anna_m;
    Entity1 anna_e;
    Texture* fruites;
    Texture* anna_t;
    Image* anna_i;
    float option;
    float llum;
    float ex;
    Matrix44 Scale;
    
    //                  ------------------PRACTICA 5---------------------------
    Shader* p_shader=nullptr;
    Shader* ps_shader=nullptr;
    Shader* pn_shader=nullptr;
    Shader* g_shader=nullptr;
    sLight l1;
    Texture* anna_tn;
    sLight l2;
    sLight l3;
    Vector3 Ia;
    Material* mg;
    Material* mp;
    Material* mp_s;
    Material* mp_n;
    Material* mp_t;
    sUniformData sUnif;
    bool b_texture=false;
    bool n_texture=false; bool s_texture=false;

    
//FINAL DE LA DECLARACIÓ DE LA CLASE APPLICATION
};
    
    
    
    
    
    
    
    
    
    
    
    //_________________________________________________________________________________________________________
    
    
    
    

    


