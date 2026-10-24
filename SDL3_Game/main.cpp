#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <cstdlib>

bool collision(const SDL_FRect& a, const SDL_FRect& b) { // this is a collision that I coded in other game converted to c++ format with sdl's rect
    return (
        a.x <= b.x + b.w &&
        a.x + a.w >= b.x &&
        a.y <= b.y + b.h &&
        a.y + a.h >= b.y
        );
}

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Surface* bgsurface;
    SDL_Surface* healthsurface;
    SDL_Surface* rocksurface;
    SDL_Texture* rocktexture;
    SDL_Texture* healthtexture;
    SDL_Texture* texture;
    SDL_Texture* bgtexture;
    SDL_Event event;
    Uint64 fps = 0;
    Uint64 lastTime = 0;
    Uint64 characterx = 18;
    Uint64 rocky = 0;
    Uint64 rockx = rand() % 211;
    Uint64 score = 0;
    int health = 196;
    bool gameover = false;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Space Cat", 500, 410, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    surface = IMG_Load("character.png");
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load character image: %s", SDL_GetError());
        return 3;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return 3;
    }
    rocksurface = IMG_Load("rock.png");
    if (!rocksurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load rock image: %s", SDL_GetError());
        return 3;
    }
    rocktexture = SDL_CreateTextureFromSurface(renderer, rocksurface);
    if (!rocktexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create rock texture from surface: %s", SDL_GetError());
        return 3;
    }
    healthsurface = IMG_Load("healthbar.png");
    if (!healthsurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load health bar: %s", SDL_GetError());
        return 3;
    }
    healthtexture = SDL_CreateTextureFromSurface(renderer, healthsurface);
    if (!healthtexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create health bar texture from surface: %s", SDL_GetError());
        return 3;
    }
    bgsurface = SDL_LoadBMP("background.bmp");
    if (!bgsurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background: %s", SDL_GetError());
        return 3;
    }
    bgtexture = SDL_CreateTextureFromSurface(renderer, bgsurface);
    if (!bgtexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create background texture from surface: %s", SDL_GetError());
        return 3;
    }
    SDL_DestroySurface(surface);
    SDL_DestroySurface(bgsurface);
    SDL_DestroySurface(healthsurface);
    SDL_DestroySurface(rocksurface);

    const bool* keys = SDL_GetKeyboardState(nullptr);

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        }
        //else if (event.type == SDL_EVENT_KEY_DOWN) {
            //SDL_Log("A key was pressed! it was %d", event.key.key);
        //}

        Uint64 currentTick = SDL_GetTicks();
        SDL_Delay(16);
        fps++;
        Uint64 deltaTime = SDL_GetTicks() - currentTick;
        if (currentTick > lastTime + 1000) {
            lastTime = currentTick;
            std::string title;
            title += "Space Cat - FPS " + std::to_string(fps);
            SDL_SetWindowTitle(window, title.c_str());
            fps = 0;
        }

        if (keys[SDL_SCANCODE_LEFT] == true) { // left arrow
            if (characterx < 4) {
                characterx = 4;
            }
            else if (characterx > 212) { // just incase for inverted key
                characterx = 212;
            }
            characterx -= 3;
        }
        if (keys[SDL_SCANCODE_RIGHT] == true) {
            if (characterx < 4) {
                characterx = 4;
            }
            else if (characterx > 212) { // just incase for inverted key
                characterx = 212;
            }
            characterx += 3;
        }
        if (keys[SDL_SCANCODE_A] == true) { // left arrow
            if (characterx < 4) {
                characterx = 4;
            }
            else if (characterx > 212) { // just incase for inverted key
                characterx = 212;
            }
            characterx -= 3;
        }
        if (keys[SDL_SCANCODE_D] == true) {
            if (characterx < 4) {
                characterx = 4;
            }
            else if (characterx > 212) { // just incase for inverted key
                characterx = 212;
            }
            characterx += 3;
        }

        rocky += 3;
        
        SDL_FRect rect{ characterx, 170, 32, 32 };
        SDL_FRect rockrect{ rockx, rocky, 48, 48 };
        SDL_FRect healthrect{ 30, 20, 200, 22 };
        SDL_FRect healthrect2{ 32, 22, health, 18 }; // 196 max
        std::string scoretext;
        scoretext += "Score: " + std::to_string(score);
        std::string gameovertext = "GAME OVER";
        std::string restarttext = "PRESS R TO RESTART";
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);

        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, bgtexture, NULL, NULL);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_RenderTexture(renderer, healthtexture, NULL, &healthrect);
        SDL_RenderTexture(renderer, rocktexture, NULL, &rockrect);
        SDL_RenderFillRect(renderer, &healthrect2);

        if (collision(rect, rockrect)) {
            SDL_Log("Collided");
            score += 1;
            rocky = 0;
            rockx = rand() % 211;
        }
      
        if (rocky > 213) {
            health -= 40;
            rocky = 0;
            rockx = rand() % 211;
        }

        if (health < 0) {
            SDL_RenderDebugText(renderer, 100.0f, 120.0f, gameovertext.c_str());
            SDL_RenderDebugText(renderer, 90.0f, 100.0f, restarttext.c_str());
            rockx = 0;
            characterx = 1000;
            rocky = 0;
            if (keys[SDL_SCANCODE_R]) {
                health = 196;
                characterx = 18;
                rockx = rand() % 211;
                score = 0;
            }
        }

        SDL_RenderTexture(renderer, texture, NULL, &rect);   
        SDL_RenderDebugText(renderer, 50.0f, 50.0f, scoretext.c_str());
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}