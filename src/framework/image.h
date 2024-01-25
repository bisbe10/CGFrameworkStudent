/*
	+ This file defines the class Image that allows to manipulate images.
	+ It defines all the need operators for Color and Image
*/

#pragma once

#include <string.h>
#include <stdio.h>
#include <iostream>
#include "framework.h"

//remove unsafe warnings
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#endif

class FloatImage;
class Entity;
class Camera;

// A matrix of pixels
class Image
{
	// A general struct to store all the information about a TGA file
	typedef struct sTGAInfo {
		unsigned int width;
		unsigned int height;
		unsigned int bpp; // Bits per pixel
		unsigned char* data; // Bytes with the pixel information
	} TGAInfo;

public:
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel = 3; // Bits per pixel

	Color* pixels;

	// Constructors
	Image();
	Image(unsigned int width, unsigned int height);
	Image(const Image& c);
	Image& operator = (const Image& c); // Assign operator

	// Destructor
	~Image();

	void Render();

	// Get the pixel at position x,y
	Color GetPixel(unsigned int x, unsigned int y) const { return pixels[ y * width + x ]; }
	Color& GetPixelRef(unsigned int x, unsigned int y)	{ return pixels[ y * width + x ]; }
	Color GetPixelSafe(unsigned int x, unsigned int y) const {	
		x = clamp((unsigned int)x, 0, width-1); 
		y = clamp((unsigned int)y, 0, height-1); 
		return pixels[ y * width + x ]; 
	}

	// Set the pixel at position x,y with value C
	void SetPixel(unsigned int x, unsigned int y, const Color& c) { pixels[ y * width + x ] = c; }
	void SetPixelSafe(unsigned int x, unsigned int y, const Color& c) const { if(x < 0 || x > width-1) return; if(y < 0 || y > height-1) return; pixels[ y * width + x ] = c; }

	void Resize(unsigned int width, unsigned int height);
	void Scale(unsigned int width, unsigned int height);
	
	void FlipY(); // Flip the image top-down

	// Fill the image with the color C
	void Fill(const Color& c) { for(unsigned int pos = 0; pos < width*height; ++pos) pixels[pos] = c; }

	// Returns a new image with the area from (startx,starty) of size width,height
	Image GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height);

	// Save or load images from the hard drive
	bool LoadPNG(const char* filename, bool flip_y = true);
	bool LoadTGA(const char* filename, bool flip_y = false);
	bool SaveTGA(const char* filename);

	void DrawRect(int x, int y, int w, int h, const Color& c);
    
    
    
    
    
    
    
    
    
                                // CODI NOSTRE PRACTICA 1 \\

    // EXERCICI 1 IMPLEMENT NOSTRE  DE (DRAWLINEDDA 1p)
    void DrawLineDDA(int x0, int y0, int x1, int y1, const Color& c);
    
    // EXERCICI 2 IMPLEMENT NOSTRE DE ( DRAWRECT 1P)
    void DrawRect(int x, int y, int w, int h, const Color& borderColor, int borderWidth, bool isFilled, const Color& fillColor);
    
    // EXERCICI 3 IMPLEMENT NOSTRE DE (DRAWCIRCLE 2p)
    void DrawCircle(int x, int y, int r, const Color & borderColor,int borderWidth, bool fill, const Color& fillColor);
    
    // EXERCICI 4 IMPLEMENT NOSTRE DE (DRAWTRIANGLE 2P)
    
        //estructura Cell
    struct Cell{
        int min_x=INT_MIN;
        int max_x=INT_MAX;
    };
        //funció ScanLineDDA
    void ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell>& table);
    
        //funció DrawTriangle
    void DrawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Color& borderColor, bool isFilled, const Color& fillColor);
    
    
    //EXERCICI 5 IMPLEMENT NOSTRE DE (DRAWIMAGE 0.5p)
    void DrawImage(const Image& image, int x, int y, bool top);
    
        
  
    
    
    
    
    
    
    
    
    
    
	// Used to easy code
	#ifndef IGNORE_LAMBDAS

	// Applies an algorithm to every pixel in an image
	// you can use lambda sintax:   img.forEachPixel( [](Color c) { return c*2; });
	// or callback sintax:   img.forEachPixel( mycallback ); //the callback has to be Color mycallback(Color c) { ... }
	template <typename F>
	Image& ForEachPixel( F callback )
	{
		for(unsigned int pos = 0; pos < width*height; ++pos)
			pixels[pos] = callback(pixels[pos]);
		return *this;
	}
	#endif
};

// Image storing one float per pixel instead of a 3 or 4 component Color

class FloatImage
{
public:
	unsigned int width;
	unsigned int height;
	float* pixels;

	// CONSTRUCTORS 
	FloatImage() { width = height = 0; pixels = NULL; }
	FloatImage(unsigned int width, unsigned int height);
	FloatImage(const FloatImage& c);
	FloatImage& operator = (const FloatImage& c); //assign operator

	//destructor
	~FloatImage();

	void Fill(const float& v) { for (unsigned int pos = 0; pos < width * height; ++pos) pixels[pos] = v; }

	//get the pixel at position x,y
	float GetPixel(unsigned int x, unsigned int y) const { return pixels[y * width + x]; }
	float& GetPixelRef(unsigned int x, unsigned int y) { return pixels[y * width + x]; }

	//set the pixel at position x,y with value C
	inline void SetPixel(unsigned int x, unsigned int y, const float& v) { pixels[y * width + x] = v; }

	void Resize(unsigned int width, unsigned int height);
    

};


// DEFINICIÓ DE LA CLASE BUTTON _________________________________________________________

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
    Button(Image im,int px0, int py0, int px, int py, bool t){
        image=im;
        positionx=px;
        positiony=py;
        positionx0=px0;
        positiony0=py0;
        top=t;
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
                if  (positiony0>= mousePosition.y & mousePosition.y >= positiony){
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
