#include <SDL.h>

#include <glm/glm.hpp>

Uint32 ColorFloatToUint32(glm::vec3 color);

class Ray
{
public:
	Ray() 
	{
		m_Origin = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Direction = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	Ray(glm::vec3 origin, glm::vec3 direction)
	{
		m_Origin = origin;
		m_Direction = direction;
	}

	glm::vec3 Origin() const { return m_Origin; }
	glm::vec3 Direction() const { return m_Direction; }

	glm::vec3 PointAt(float t) { return m_Origin + (t * m_Direction); }

private:
	glm::vec3 m_Origin;
	glm::vec3 m_Direction;
};

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	int windowWidth = 256;
	int windowHeight = 256;

	SDL_Window* window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, windowWidth, windowHeight);

	Uint32* pixels = new Uint32[windowWidth * windowHeight];

	bool running = true;
	bool rendered = false;
	SDL_Event input;
	while (running)
	{
		while (SDL_PollEvent(&input))
		{
			if (input.type == SDL_QUIT) { running = false; }
		}

		if (!rendered) {
			for (int y = 0; y < windowHeight; y++)
			{
				SDL_Log("%d%%", (int)((float)y / ((float)windowHeight - 1.0f) * 100.0f));
				for (int x = 0; x < windowWidth; x++)
				{
					// Defaults color value with just alpha and black
					glm::vec3 pixelColor(float(x) / (windowWidth - 1), float(y) / (windowHeight - 1), .25f);

					// Filling row at a time from bottom to top
					pixels[windowWidth * windowHeight - windowWidth + x - y * windowHeight] = ColorFloatToUint32(pixelColor);
				}
			}
		}
		rendered = true;

		SDL_UpdateTexture(screen, NULL, pixels, windowWidth * sizeof(Uint32));

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, screen, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	delete[] pixels;
	SDL_DestroyTexture(screen);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}


Uint32 ColorFloatToUint32(glm::vec3 color)
{
	// Defaults color value with just alpha and black
	Uint32 colorValue = 0xff000000;

	// Convert color to 8 bit value
	Uint32 red = int(255.999f * color.r);
	Uint32 green = int(255.999f * color.g);;
	Uint32 blue = int(255.999f * color.b);

	// Shift bits into place and set color
	red = red << 16;
	green = green << 8;
	return colorValue | red | green | blue;
}