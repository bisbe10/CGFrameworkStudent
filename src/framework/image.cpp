#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"

Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++x)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.SetPixel( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8* pos = (Uint8*)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size)
		return false;

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0)
		return false;

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);
	
	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3) {
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4) {

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if( (pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixel(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		perror("Failed to open file: ");
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);

	return true;
}

void Image::DrawRect(int x, int y, int w, int h, const Color& c)
{
	for (int i = 0; i < w; ++i) {
		SetPixel(x + i, y, c);
		SetPixel(x + i, y + h - 1, c);
	}

	for (int j = 0; j < h; ++j) {
		SetPixel(x, y + j, c);
		SetPixel(x + w - 1, y + j, c);
	}
}















//------------------------------------PRACTICA 1:-------------------------------------------\\








                    // EXERCICI 1: IMPLEMENT NOSTRE (DRAWLINEDDA 1p) \\


void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color &c){
    
    // Busquem les diferenciés entre les coordenades x i y dels punts inicials i finals
    int difx =  x1-x0;
    int dify = y1-y0;
    // Amb std:: cridem la funció sqrt (arrel), la qual utilitzem per calcular la longitud de la línia en base a l'arrel dels quadrats de la componenet del vector entre el punt final i inicial)
    float d = std::sqrt(std::pow(difx, 2) + std::pow(dify, 2));
    
    // Definim la direcció normalitzada de la línia com un vector
    Vector2 direccio =  Vector2(difx/d, dify/d);
    // Definim el propi vector amb coordenades inicials x0 i y0
    Vector2 A = Vector2(x0, y0);

    //iterem tantes vegades com a pixels del x0y0 a xy hi hagi
    for(int i = 0; i < d; i++){
        
        SetPixel(floor(A.x), floor(A.y), c);
        
        //operator:funció de A que defineix el tipus de clase de "+=" [ aquesta linia suma la direcció i la distancia 1 al nostre vector normalitzat A] així doncs a va guanyant distancia del punt x0 y0 a x y
        A.operator+=(direccio);
    }
}








            // EXERCICI 2: IMPLEMENTACIÓ NOSTRE (Drawing Rectangles (1 point)) \\


void Image::DrawRect(int x, int y, int w, int h, const Color& borderColor, int borderWidth, bool isFilled, const Color& fillColor){
   
    // Determinem quantes vegades hem de fer rectangles depenent del width.
    for(int b=0; b<(borderWidth); b++){
        //agafem de referencia l'eix de les x
        for(int i = x; i < (x+w); i++){
            
        // Extrems del rectangle han de juntar les dues linies horitzontals (w) amb una distancia (h)
        
        //Extrem inicial
           if (i==x){

                    for (int j=y; j<(h+y); j++){
                        // desde la posició x(=i) pinta pixels en direcció amunt h vegades, fins a (h+y).
                         SetPixel(i, j, borderColor);
                    }

        //Extrem final
        } else if( i==((x+w)-1)){
                    for (int j=y; j<(h+y); j++){
                        // desde la posició x+w(=i+w) pinta pixels en direcció amunt h vegades, fins a (h+y).
                          SetPixel(x+w, j, borderColor);
                    }
                }
        // formem les dues linies horitzontals
            SetPixel(i, y, borderColor); // desde l'altura y (pixel x,y pixel x+1,y .... pixel x+w, y)
            SetPixel(i, y+h, borderColor);// desde l'altura y+h (pixel x,y+h pixel x+1,y+h .... pixel x+w, y+h)
        
        }
        // depenent del borde que necessitem col·loquem el nostre punt de referncia més endins i dalt i restel la altura i amplada necessaries al realitzr aquest canvi
        x+=1;
        y+=1;
        w-=2;
        h-=2;
    }
    // Si filled==true formem un rectangle omplert dins del borde format. agafant de referencia la x i y resultants de les orperacions per engruixir el borde.
    if (isFilled==true){
        for(int wf=0; wf<w+1; ++wf){
            for(int hf=0; hf<h+1;++hf){
                SetPixel((x+wf),(y+hf),fillColor);
            }
        }
    }

}
    







  
        // EXERCICI 3: IMPLEMENTACIÓ NOSTRE DE (3.3 Rasterizing Circles (2 points) \\




void Image::DrawCircle(int x, int y, int r, const Color & borderColor,int borderWidth, bool fill, const Color& fillColor){
    
    // Definim el centre del cercle CentreX=X CentreY=Y centre=(x,y) .
    int centreX;
    int centreY;
    //CentreX=X incial CentreY=Y incial -> centre=(x,y)
    centreX = x;
    centreY = y;
    int v;
    x = 0;
    y = r;
    v = 1-r;
    
    // Dibuixem els quatre punts inicials (superior, inferior, dret i esquerre) utilitzant la funció SetPixelSafe per assegurar-se que no es superi la mida de la imatge.
    SetPixelSafe(centreX, centreY+y, borderColor); //Superior
    SetPixelSafe(centreX, centreY-y, borderColor);//Inferior
    SetPixelSafe(centreX + y, centreY, borderColor);//Dret
    SetPixelSafe(centreX-y, centreY, borderColor);//Esquerra

    // (algorisme de Bresenham) !!comprobar
    
    while( y > x){
        // condició que determina el numero de particions que trobem en un quart de cercle(=numero d'iteracions del while)
        if( v < 0){
            // la v, quan el radi és més gran a 1, defineix les particions de pixels que tenim en un quart de cercle ( donant una iteració més a la y).
            v = v+2*x+3;
            x++;
        }else{
            // la v, quan el radi és més petit a 1, defineix les particions de pixels que tenim en un quart de cercle.
            v = v+2*(x-y)+5;
            x++;
            y--;
        }
        // linies de codi que dibueixen els pixels simetrics al primer quart.
        SetPixelSafe(centreX + x, centreY + y, borderColor); // (x,y)
        SetPixelSafe(centreX - x, centreY + y,borderColor); // (-x, y)
        SetPixelSafe(centreX + x, centreY - y, borderColor); // (x, -y)
        SetPixelSafe(centreX - x, centreY - y, borderColor); // (-x, -y)
        SetPixelSafe(centreX + y, centreY + x, borderColor); // (y, x)
        SetPixelSafe(centreX - y, centreY + x, borderColor); // (-y, x)
        SetPixelSafe(centreX + y, centreY - x, borderColor);// (y, -x)
        SetPixelSafe(centreX - y, centreY - x, borderColor); // (-y, -x)
    
        
        
        // CODI PER EMPLENAR EL CERCLE
        if(fill == true){
            // emplenarem el cercle formant linies entre els diferents punts que anem creant en el perimetre de la rodona.
            
            for(int j = centreX - x; j< centreX + x-2; j++ ){ // primera porció del cercle
                SetPixelSafe(j+1, centreY + y-1, fillColor);

            }

            for(int j = centreX - y; j < centreX + y-2; j++){ //  segona porició del cercle
                SetPixelSafe(j+1, centreY + x+1, fillColor);

            }

            for(int j = centreX - x; j < centreX + x-2; j++){ // última porció del cercle
                SetPixelSafe(j+2, centreY - y+1, fillColor);
            }

            for(int j =centreX - y; j < centreX + y-2; j++ ){ // penùltima porció del cercle
                SetPixelSafe(j+1, centreY - x+2, fillColor);
            }

        
        }
    }
}
        
   








                // EXERCICI 4 IMPLEMENT NOSTRE DE (DRAWTRIANGLE 2P)\\



void Image::ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell>& table){
    // Busquem les diferenciés entre les coordenades x i y dels punts inicials i finals
    int difx =  x1-x0;
    int dify = y1-y0;
    // Amb std:: cridem la funció sqrt (arrel), la qual utilitzem per calcular la longitud de la línia en base a l'arrel dels quadrats de la componenet del vector entre el punt final i inicial)
    float d = std::sqrt(std::pow(difx, 2) + std::pow(dify, 2));
    
    // Definim la direcció normalitzada de la línia com un vector
    Vector2 direccio =  Vector2(difx/d, dify/d);
    // Definim el propi vector amb coordenades inicials x0 i y0
    Vector2 A = Vector2(x0, y0);

    //iterem tantes vegades com a pixels del x0y0 a xy hi hagi
    for (int i = 0; i < d; i++) {
        
        
        
        
        // Check if A is within the bounds of the table
//        if (A.x >= 0 && A.x < tableWidth && A.y >= 0 && A.y < tableHeight) {
//            // Update minX and maxX values in the table
//            int tableIndex = static_cast<int>(A.y) * tableWidth + static_cast<int>(A.x);
//            table[tableIndex].minX = std::min(table[tableIndex].minX, static_cast<int>(A.x));
//            table[tableIndex].maxX = std::max(table[tableIndex].maxX, static_cast<int>(A.x));
//        }

        
        
        A.operator+=(direccio);
    }
}




void Image::DrawTriangle(const Vector2& p0,const Vector2& p1, const Vector2& p2, const Color& borderColor, bool isFilled, const Color& fillColor){
    
    
    // BORDE DEL TRIANGLE (0,5p)
    DrawLineDDA(p0.x, p0.y, p1.x, p1.y, borderColor);
    DrawLineDDA(p1.x, p1.y, p2.x, p2.y, borderColor);
    DrawLineDDA(p2.x, p2.y, p0.x, p0.y, borderColor);

    // creas una taula amb un maxim de les mides de p0 p1 p2 
}
    
    
 










                        //EXERCICI 5 IMPLEMENT ALBA (DRAWIMAGE 0.5p)\\


    void Image::DrawImage(const Image& image, int x, int y, bool top){
        
        //Definim l'alçada en la que dibuixarem la imatge restant l'alçada de l'imatge a la del framebuffer.
        int alçada = this->height - image.height;
        
        for(int i = 0; i < image.width ; i++){
            for(int j = 0 ; j <  image.height; j++){
                Color color = image.GetPixelSafe(i,j);
                
                if(top){
                    //this->top = true;
                    SetPixelSafe(i+x, j+alçada , color);
                    
                }else{
                    //this->top = false;
                    SetPixelSafe(i+x, j , color);
                }
                
            }
        }
    }
    
    
    
    
    


















#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}
