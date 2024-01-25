/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"

class Application
{
public:

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
    int nimatges=9;
	Image framebuffer;
    Image toolbar;
    Image line;
    Image circle;
    Image rectangle;
    Image triangle;
    Image fill;
    Image clear;
    Image load;
    Image eraser;
    
    
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
    
};

//CLASE BUTTON
class Button{
private:

    //ATRIBUTS
    Image image;
    int size;
    bool top;
    float positionx0;
    float positionx;
    float positiony0;
    float positiony;

public:
    //FUNCIONS
    //CONSTRUCTOR
    Button(Image im,int px0, int py0, int px, int py){
        image=im;
        positionx=px;
        positiony=py;
        positionx0=px0;
        positiony0=py0;
    }
    //geters
    float getPostitionx0 ( ){
        return positionx0;
    }
    float getPostitiony0 ( ){
        return positiony0;
    }
    float getPostitionx ( ){
        return positionx;
    }
    float getPostitiony ( ){
        return positiony;
    }
    bool getTop ( ){
        return top;
        
    }
    bool IsMouseInside(Vector2 mousePosition){
        if (positionx0 <= mousePosition.x & mousePosition.x <= positionx){
            // la y0 es el max si el botton es top = true
            if (top==true) {
                if  (postiony0>= mousePosition.y & mousePosition.y >= positiony){
                    return true; // maus entre y< maus.y <y0
                }
            }else{
                // la y0 es 0 si el botton es top = false
                if (positiony0 <= mousePosition.y & mousePosition.y <= positiony){
                    return true; // maus entre y0< maus.y <y
                }
            }
            return false;
        }
    }
};
