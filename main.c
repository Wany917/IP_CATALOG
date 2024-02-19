#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef enum {
    MENU_ADD_IP,
    MENU_REMOVE_IP,
    MENU_DISPLAY_IPS,
    MENU_APPLY_MASK,
    MENU_QUIT
} MenuOption;

void render_menu(SDL_Renderer *renderer, TTF_Font *font);
MenuOption handle_menu_input(const SDL_Event *event, int* selectedOption);
void add_ip_address();
void remove_ip_address();
void display_ip_addresses();
void apply_mask_to_ip();
void quit_application();
void initialize_sdl();
void quit_sdl();
void render_text(SDL_Renderer *renderer, const char *text, int x, int y, TTF_Font *font);


int main() {
    initialize_sdl();
    SDL_Window *window = SDL_CreateWindow("IP C-atalogue", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont("/Users/yvann/CLionProjects/IP_CATALOG/font/font.ttf", 24);
    if (font == NULL) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    int selectedOption = 0; // Variable pour suivre l'option de menu sélectionnée
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }

            MenuOption menuSelection = handle_menu_input(&event, &selectedOption);
            switch (menuSelection) {
                case MENU_ADD_IP:
                    add_ip_address();
                    break;
                case MENU_REMOVE_IP:
                    remove_ip_address();
                    break;
                case MENU_DISPLAY_IPS:
                    display_ip_addresses();
                    break;
                case MENU_APPLY_MASK:
                    apply_mask_to_ip();
                    break;
                case MENU_QUIT:
                    quit_application();
                    running = false;
                    break;
                default:
                    break;
            }

            SDL_RenderClear(renderer);
            render_menu(renderer, font);
            SDL_RenderPresent(renderer);
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    quit_sdl();

    return 0;
}


void initialize_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        exit(1);
    }
}

void quit_sdl() {
    TTF_Quit();
    SDL_Quit();
}

void render_text(SDL_Renderer *renderer, const char *text, int x, int y, TTF_Font *font) {
    SDL_Color color = {255, 255, 255, 255}; 
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void render_menu(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Rect bgRect = {50, 50, 700, 500};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &bgRect);

    render_text(renderer, "1. Add IP Address", 100, 100, font);
    render_text(renderer, "2. Remove IP Address", 100, 150, font);
    render_text(renderer, "3. Display IP Addresses", 100, 200, font);
    render_text(renderer, "4. Apply Mask to IP", 100, 250, font);
    render_text(renderer, "5. Quit Application", 100, 300, font);
}

MenuOption handle_menu_input(const SDL_Event *event, int* selectedOption) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
                if (*selectedOption > 0) {
                    (*selectedOption)--;
                }
                break;
            case SDLK_DOWN:
                if (*selectedOption < MENU_QUIT) {
                    (*selectedOption)++;
                }
                break;
            case SDLK_RETURN:
                return (MenuOption)(*selectedOption);
            default:
                break;
        }
    } 

    else if (event->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        SDL_Rect addIpRect = {100, 100, 200, 50};
        if (mouseX >= addIpRect.x && mouseX <= addIpRect.x + addIpRect.w &&
            mouseY >= addIpRect.y && mouseY <= addIpRect.y + addIpRect.h) {
            return MENU_ADD_IP;
        }

        if(mouseX >= 100 && mouseX <= 300 && mouseY >= 100 && mouseY <= 150) {
            return MENU_ADD_IP;
        }
        if(mouseX >= 100 && mouseX <= 300 && mouseY >= 150 && mouseY <= 200) {
            return MENU_REMOVE_IP;
        }
        if(mouseX >= 100 && mouseX <= 300 && mouseY >= 200 && mouseY <= 250) {
            return MENU_DISPLAY_IPS;
        }
        if(mouseX >= 100 && mouseX <= 300 && mouseY >= 250 && mouseY <= 300) {
            return MENU_APPLY_MASK;
        }
        if(mouseX >= 100 && mouseX <= 300 && mouseY >= 300 && mouseY <= 350) {
            return MENU_QUIT;
        }
    }

    return (MenuOption)(*selectedOption); 
}


void add_ip_address() {
    // just print to the console for now
    printf("Adding IP address\n");
}

void remove_ip_address() {
    // same to add_ip_address
    printf("Removing IP address\n");
}

void display_ip_addresses() {
    // same to add_ip_address
    printf("Displaying IP addresses\n");
}

void apply_mask_to_ip() {
    // same to add_ip_address
    printf("Applying mask to IP address\n");
}

void quit_application() {
    // same to add_ip_address
    printf("Quitting application\n");
}