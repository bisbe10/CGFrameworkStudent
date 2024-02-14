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
#include "cmath"

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

//------------------------------------PRACTICA 3:-----------------------------------------------------------\\

void Image::DrawTriangleInterpolated(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Color& c0, const Color& c1, const Color& c2,FloatImage* zbuffer, Image * tex, const Vector2 &uv0, const Vector2 &uv1, const Vector2 &uv2,int tipus){
    
    
    Matrix44 m;
    m.SetIdentity();
    
    std::vector<Cell> table(height);
    //Update table with the min and max x values of the triangle
    ScanLineDDA(p0.x, p0.y, p1.x, p1.y, table);
    ScanLineDDA(p1.x, p1.y, p2.x, p2.y, table);
    ScanLineDDA(p2.x, p2.y, p0.x, p0.y, table);
    //Paint the triangleaula
    
    //RECORDAR QUE PRIMER LA COLUMNA I DESPRES LA FILA
    //p0.x p0.y 1                p1.x p1.y 1             p2.x p2.y 1
    m.M[0][0]=p0.x;             m.M[1][0]=p1.x;         m.M[2][0]=p2.x;
    m.M[0][1]=p0.y;             m.M[1][1]=p1.y;         m.M[2][1]=p2.y;
    m.M[0][2]=1;                m.M[1][2]=1;            m.M[2][2]=1;
    
    m.Inverse();
    
    for (int i = 0; i < table.size(); i++) {
        //Paint each row of the triangle from minx to maxx (included)
        for (int j = table[i].min_x; j <= table[i].max_x; j++) {

            Vector2 v4 = Vector2(j, i); // el punt que volem pintar

            Vector3 bCoords = m*Vector3(v4.x,v4.y,1);
            bCoords.Clamp(0,1);
            
            // Normalitzem els pesos (les coordenades bCoords) dividint per la suma
            float sum = bCoords.x + bCoords.y+ bCoords.z;
            bCoords= bCoords/sum;
            Color finalColor=(bCoords.x) * c0 +(bCoords.y) * c1 + (bCoords.z) * c2;
            
            //tipus 1:
            
            if (tipus==1){
                SetPixelSafe(v4.x,v4.y,finalColor);
            }
            else if(tipus==2){
                //tipus 2:
                //3.3 Draw a mesh with occlusions (Z-Buffer) (3 points)
                //trobem la zi de cada pixel utilitzant els pesos de cada vertex al nostre punt p, multiplicats per la coordinada z de cada vertex
                float zi = (p0.z * bCoords.x) + (p1.z * bCoords.y) + (p2.z * bCoords.z);
                
                    if ( zi < zbuffer->GetPixel(j,i)){
                        SetPixel(j, i, finalColor);
                        zbuffer->SetPixel(j, i, zi);
    
                    }
            } else if (tipus==3){
                
                //tipus 3:
                Vector2 tex_f = (uv0 * bCoords.x + uv1 * bCoords.y + uv2 * bCoords.z);
                const float& zi = (p0.z * bCoords.x) + (p1.z * bCoords.y) + (p2.z * bCoords.z);
               
                //agafar colo de la textura en el punt que estigui i fer setpixelsafe:
                if ( zi < zbuffer->GetPixel(j,i)){
                    Color color_t = tex->GetPixelSafe(tex_f.x,(tex->height-1)  -tex_f.y);
                    SetPixelSafe(j, i, color_t);
                    zbuffer->SetPixel(j, i, zi);

                }
            }
    }

  }
}
                                    
//------------------------------------PRACTICA 1:-----------------------------------------------------------\\



void Image::ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell>& table){
   
    float dx = x1 - x0;
        float dy = y1 - y0;

        float d = std::max(abs(dx), abs(dy));
        Vector2 v = Vector2(dx / d, dy / d);
        float x = x0, y = y0;

        for (float i = 0; i <= d; i++) {
            //Update the table only if the calculated y coordinates are within the range of the image
            if (y >= 0 && y < table.size()) {
                table[floor(y)].min_x = fmin(floor(x), table[floor(y)].min_x);
                table[floor(y)].max_x = fmax(floor(x), table[floor(y)].max_x);
            }
            x += v.x;
            y += v.y;
        }
}



void Image::DrawTriangle(const Vector2& p0,const Vector2& p1, const Vector2& p2, const Color& borderColor, bool isFilled, const Color& fillColor){

    if (isFilled) {
            //Create table
            std::vector<Cell> table(height);
            //Update table with the min and max x values of the triangle
            ScanLineDDA(p0.x, p0.y, p1.x, p1.y, table);
            ScanLineDDA(p1.x, p1.y, p2.x, p2.y, table);
            ScanLineDDA(p0.x, p0.y, p2.x, p2.y, table);
            //Paint the triangle
            for (int i = 0; i < table.size(); i++) {
                //Paint each row of the triangle from minx to maxx (included)
                for (int j = table[i].min_x; j <= table[i].max_x; j++) {
                    SetPixelSafe(j, i, fillColor);
                }
            }
        }

        DrawLineDDA(p0.x, p0.y, p1.x, p1.y, borderColor);
        DrawLineDDA(p0.x, p0.y, p2.x, p2.y, borderColor);
        DrawLineDDA(p1.x, p1.y, p2.x, p2.y, borderColor);
}





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
        
        SetPixelSafe(floor(A.x), floor(A.y), c);
        
        //operator:funció de A que defineix el tipus de clase de "+=" [ aquesta linia suma la direcció i la distancia 1 al nostre vector normalitzat A] així doncs a va guanyant distancia del punt x0 y0 a x y
        A+=(direccio);
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



//hemos investigado sobre algoritmos que generaban el circulo de forma de manera eficiente. Nos hemos decantado por intentar el algoritmo de Bresenham, el cual va generando el círculo por octavos.
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
void FloatImage::Resize(unsigned int width, unsigned int height){
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
 


//--------------------------------------------------------------------------------------------------------\\
