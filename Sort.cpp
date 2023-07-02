#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <random>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int rectSize = 15;
bool completed = false;
std::vector<int> arr;

std::random_device dev;
std::mt19937 rng(dev());

bool init() {
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize SDL. SDL_Error: " << SDL_GetError();
        success = false;
    }
    else {
        if(!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))) {
            std::cout << "Warning: Linear Texture Filtering not enabled.\n";
        }
        window = SDL_CreateWindow("Sorting Visual", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(!window) {
            std::cout << "Couldn't creat window. SDL_Error: " << SDL_GetError();
            success = false;
        }
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(!renderer) {
                std::cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError();
                success = false;
            }
        }
    }
    return success;
}

void close() {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}

void draw(int x = -1, int y = -1) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    for(int i = 0, j = 0; i <= arr.size() * (rectSize + 1); i += rectSize + 1, ++j) {
        SDL_PumpEvents();
        SDL_Rect rect = {i, 0, rectSize, arr[j]};
     
        if(completed) {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if(j == x) {
            SDL_SetRenderDrawColor(renderer, 0, 191, 255, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if(j == y) {
            SDL_SetRenderDrawColor(renderer, 155, 128, 0, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_Delay(100);
    SDL_RenderPresent(renderer);
}

int rand(int start, int end) {
    std::uniform_int_distribution<std::mt19937::result_type> dist(start, end);
    return dist(rng);
}

void randomizeArray() {
    completed = false;
    //int size = rand(2, SCREEN_WIDTH / (rectSize + 1));
    int size = 50; 
    arr.resize(size);
    for(int i = 0; i < size; ++i) {
        arr[i] = rand(5, (SCREEN_HEIGHT - 100));
    }
}

void bubbleSort() {
    for(int i = 0; i < arr.size(); ++i) {
        bool swapped = false;
        for(int j = 0; j < arr.size() - i - 1; ++j) {
            if(arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                draw(j, j + 1);
                swapped = true;
            }
        }
        if(!swapped) break;
    }
    completed = true;
}

void selectionSort() {
    for(int i = 0; i < arr.size(); ++i) {
        int minIndex = i;
        for(int j = i; j < arr.size(); ++j) {
            draw(j, minIndex);
            if(arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        std::swap(arr[i], arr[minIndex]);
        draw(-1, i);
        SDL_Delay(300);
    }
    completed = true;
}

void insertionSort() {
    for(int i = 1; i < arr.size(); ++i) {
        for(int j = i; j > 0; --j) {
            if(arr[j] < arr[j-1]) {
                std::swap(arr[j], arr[j-1]);
                draw(j-1);
            }
            else break;
        }
    }
    completed = true;
}

// void insertionSort() 
// { 
//     int i, key, j; 
//     for (i = 1; i < arr.size(); i++)
//     { 
//         key = arr[i]; 
//         j = i - 1; 
  
//         while (j >= 0 && arr[j] > key)
//         { 
//             arr[j + 1] = arr[j]; 
//             draw(j, i - 1);
//             --j; 
//         } 
//         arr[j + 1] = key; 
//         draw(j + 1);
//         SDL_Delay(200);
//     } 
// }

int main() {
    if(!init()) {
        std::cout << "SDL Init failed.\n";
    }
    else {
        randomizeArray();
        SDL_Event e;
        bool running = true;
        while(running) {
            while(SDL_PollEvent(&e) != 0) {
                if(e.type == SDL_QUIT) {
                    running = false;
                }
                else if(e.type == SDL_KEYDOWN) {
                    switch(e.key.keysym.sym) {
                        case SDLK_0:
                            randomizeArray();
                            break;
                        case SDLK_1:
                            bubbleSort();
                            break;
                        case SDLK_2:
                            selectionSort();
                            break;
                        case SDLK_3:
                            insertionSort();
                            break;
                    }
                }
            }
            draw();
        }
        close();
    }
}