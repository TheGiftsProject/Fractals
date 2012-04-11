#include "SDL/SDL.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void renderMandelbrot(SDL_Surface *surface, int width, int height, double left, double top, double mbw, double mbh, int n_iterations = 255);
 
int main( int argc, char* argv[])
{
   	// Fire up SDL, this starts all subsystems; audio video etc.
    	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface* s = SDL_SetVideoMode( 700, 400, 32, SDL_HWSURFACE);
	SDL_Surface* map = SDL_CreateRGBSurface(SDL_HWSURFACE, 700/3, 400/3, 32, 0, 0, 0, 0);
	SDL_Event event;

	bool quit = false;
	int i = 0;
	srand(time(NULL));

	int depth = 0;
	double mbw = 3.5;
	double mbh = 2.0;
	double offsetX = -2.5;
	double offsetY = -1.0;
	renderMandelbrot(s, 700, 400, offsetX, offsetY, mbw, mbh);
	SDL_Flip(s);
	while(!quit){
		//If there's an event to handle
	        while ( SDL_PollEvent( &event ) )
        	{
	            //If the user has Xed out the window
	            if( event.type == SDL_QUIT )
	            {
	                //Quit the program
	                quit = true;
	            }

			if (event.type == SDL_MOUSEBUTTONDOWN) {
				depth++;
				renderMandelbrot(map, 700/3, 400/3, offsetX, offsetY, mbw, mbh, 50);
				double clickX = (double)event.button.x/700.0;
				double clickY = (double)event.button.y/400.0;
				offsetX = clickX * mbw + offsetX - mbw*0.25;
				offsetY = clickY * mbh + offsetY - mbh*0.25;
				mbw /= 2.0;
				mbh /= 2.0;

				renderMandelbrot(s, 700, 400, offsetX, offsetY, mbw, mbh);
				SDL_BlitSurface(map, NULL, s, NULL);
				SDL_Flip(s);
				cout << "OffsetX " << offsetX << endl;
				cout << "OffsetY " << offsetY << endl;
			}
		}
	}

	SDL_Quit();
    	
	return 0;
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }

}

void renderMandelbrot(SDL_Surface *surface, int width, int height, double left, double top, double mbw, double mbh, int max_iteration) {
	for (int px = 0; px < width; ++px) {
		for (int py = 0; py < height; ++py) {
			double x0 = (double)px/(double)width * mbw + left;
			double y0 = (double)py/(double)height * mbh + top;
			//double x0 = ((double)px/(double)width * 3.5 - 2.5) / scale + ((double)offsetX / width * 3.5 - 2.5)/scale;
			//double y0 = ((double)py/(double)height * 2.0 - 1.0) / scale + ((double)offsetY / height * 2.0 - 1.0)/scale;

			double x = 0.0;
			double y = 0.0;

			int iteration = 0;

			while (x*x + y*y < 4 && iteration < max_iteration) {
				double xtemp = x*x - y*y + x0;
				y = 2*x*y + y0;
				x = xtemp;
				iteration++;
			}

			unsigned char color = (unsigned char)(((double)iteration/max_iteration) * 255.0);
			putpixel(surface, px, py, 0xFF << 24 | color << 16 | color/2 << 8 | color/3);
		}
	}
}



