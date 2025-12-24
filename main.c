#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIDTH  640
#define HEIGHT 480
int pos_x = 0;
int pos_y = 0;
int size_x = 25;
int size_y = 200;
int speed = 1;


int pos2_x = WIDTH-25;
int pos2_y = 0;
int size2_x = 25;
int size2_y = 200;
int speed2 = 1;

int pos3_x = WIDTH/2;
int pos3_y = HEIGHT/2;
int size3_x = 50;
int size3_y = 50;
int speedx = 2;
int speedy = 2;

void draw(uint32_t *pixels, int x, int y, uint32_t color) {
    pixels[y * WIDTH + x] = color;
}

void rect(uint32_t *pixels, int x, int y, int w, int h) {
    for (int v = y; v < y + h; v++) {
        for (int z = x; z < x + w; z++) {
            draw(pixels, z, v, 0xFFFFFFFF);
        }
    }
}


int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        0
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    // Create a texture we can write pixels to
    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );
    uint32_t *pixels = malloc(WIDTH * HEIGHT * sizeof(uint32_t));
    bool running = true;



    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
            running = false;

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            running = false;
        }
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        
        if (keys[SDL_SCANCODE_LSHIFT]) speed =6;
        else speed =1;

        if (keys[SDL_SCANCODE_W]) 
            if (pos_y - speed <= 0)
                pos_y = 0;
            else
                pos_y -= speed;

        if (keys[SDL_SCANCODE_S]){
            if (pos_y + size_y + speed >= HEIGHT)
                pos_y = HEIGHT - size_y;
            else
                pos_y += speed;
        }

        if (keys[SDL_SCANCODE_RSHIFT]) speed2 =6;
            else speed2 =1;
        if (keys[SDL_SCANCODE_UP]) 
            if (pos2_y - speed2 <= 0)
                pos2_y = 0;
            else
                pos2_y -= speed2;

        if (keys[SDL_SCANCODE_DOWN]){
            if (pos2_y + size2_y + speed2 >= HEIGHT)
                pos2_y = HEIGHT - size2_y;
            else
                pos2_y += speed2;
        }


        for (int i = 0; i < WIDTH * HEIGHT; i++)
            pixels[i] = 0xFF000000; // black

        int shift = 0;
        for (int y = 0; y < HEIGHT; y++){
            for (int x = shift; x < WIDTH; x+=2){
                pixels[y * WIDTH +x]=0xFFFFFFFF;
            }
            if (shift ==0)
                shift=1;
            else
                shift=0;
        }

        pos3_x+=speedx;
        pos3_y+=speedy;

        if (pos3_x + size3_x >= WIDTH-size2_x){
            if (pos3_y + size3_y < pos2_y)
                running=false;
            if (pos3_y > pos2_y+size2_y)
                running=false;
            pos3_x= WIDTH-size2_x-size3_x;
            speedx = -1*speedx;
            
        }
        if (pos3_y + size3_y >= HEIGHT){
                pos3_y = HEIGHT-size3_y;
                speedy = -1*speedy;
        }
        if (pos3_y <= 0){
            pos3_y=0;
            speedy= -1 * speedy;
        }
        if (pos3_x <= size_x){
            if (pos3_y+size3_y < pos_y)
                running=false;
            if (pos3_y > pos_y+size_y)
                running=false;
            pos3_x = size_x;
            speedx = -1 * speedx;
        }
        rect(pixels, pos_x,pos_y,size_x,size_y);
        rect(pixels,pos2_x,pos2_y,size2_x,size2_y);
        rect(pixels,pos3_x,pos3_y,size3_x,size3_y);
        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
