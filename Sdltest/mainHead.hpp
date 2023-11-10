#pragma once
#include <iostream>
#include <chrono>
#include <exception>
#include <string_view>
#include <vector>
#include <type_traits>
#include <list>
#include <functional>
#include <utility>
#include <typeindex>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>





struct App
{

    inline static int keyboard[350];

    const inline static int SCREEN_WIDTH = 1280;
    const inline static int SCREEN_HEIGHT = 720;
};




struct ExceptHandler
{
    static void keys_down(SDL_Event& ev)
    {
        if (ev.key.keysym.scancode == SDL_SCANCODE_UP)
        {
            App::keyboard[SDL_SCANCODE_UP] = 1;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_DOWN)
        {
            App::keyboard[SDL_SCANCODE_DOWN] = 1;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_RIGHT)
        {
            App::keyboard[SDL_SCANCODE_RIGHT] = 1;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_LEFT)
        {
            App::keyboard[SDL_SCANCODE_LEFT] = 1;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            App::keyboard[SDL_SCANCODE_SPACE] = 1;
        }
    }

    static void keys_up(SDL_Event& ev)
    {
        if (ev.key.keysym.scancode == SDL_SCANCODE_UP)
        {
            App::keyboard[SDL_SCANCODE_UP] = 0;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_DOWN)
        {
            App::keyboard[SDL_SCANCODE_DOWN] = 0;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_RIGHT)
        {
            App::keyboard[SDL_SCANCODE_RIGHT] = 0;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_LEFT)
        {
            App::keyboard[SDL_SCANCODE_LEFT] = 0;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            App::keyboard[SDL_SCANCODE_SPACE] = 0;
        }
    }

    static void end(std::string_view er)
    {
        std::cout << er << " : " << SDL_GetError() << std::endl;
        SDL_Quit();
        std::exit(1);
    }
};



//singleton class
class Engine final
{
private:


    static inline Engine* Eng{};

    Engine() { initSDL(); }

public:

    SDL_Renderer* render{};
    SDL_Window* window{};

public:

    void initSDL()
    {
        int rendererFlags, windowFlags;

        rendererFlags = SDL_RENDERER_ACCELERATED;

        windowFlags = 0;


        if (SDL_Init(SDL_INIT_VIDEO) < 0) ExceptHandler::end("initSDL - failure SDL_Init");

        window = SDL_CreateWindow("MainWindow", App::SCREEN_WIDTH, App::SCREEN_HEIGHT, windowFlags);

        if (!window) ExceptHandler::end("initSDL - failure SDL_CreateWindow");

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        render = SDL_CreateRenderer(window, nullptr, rendererFlags);

        if (!render) ExceptHandler::end("initSDL - failure SDL_CreateRenderer");
    }

    void prepareScene()
    {
        SDL_SetRenderDrawColor(render, 20, 20, 20, 255);
        SDL_RenderClear(render);
    }

    void presentScene()
    {
        SDL_RenderPresent(render);
    }



    void doInput()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:ExceptHandler::end("SDL_Event_QUIT"); break;

            case SDL_EVENT_KEY_DOWN:ExceptHandler::keys_down(event); break;

            case SDL_EVENT_KEY_UP:ExceptHandler::keys_up(event); break;

            default:
                break;
            }
        }
    }

public:

    static inline Engine& CreateEngine()
    {
        if (Eng == nullptr)
        {
            Eng = new Engine();
            return *Eng;
        }
        else{
            return *Eng;
        }    
    }

    ~Engine()
    {
        delete Eng;
    }

};




class entity_interface;



class Texturation {

protected:

    static SDL_Texture& load_texture(SDL_Renderer& rend, std::string_view file);

public:

    static void blit(SDL_Renderer& rend,entity_interface& entity, SDL_RendererFlip flip = SDL_FLIP_NONE);

    static void  drawRect(SDL_Renderer& rend,float x, float y, float w, float h);

    static void align_center(SDL_Renderer& rend,entity_interface& obj);

    virtual ~Texturation() = 0;
};


class texturs: public Texturation
{
private:

    static inline SDL_Texture* main_text;
    static inline SDL_Texture* bullet_text;

public:

    static  SDL_Texture& load_main_texture();

    static  SDL_Texture& load_main_bullet();

};



struct  StopWatch final
{
private:
    static inline std::chrono::steady_clock::time_point start_time{};
    static inline std::chrono::steady_clock::time_point end_time{};
    static inline bool flag = false;

    static inline double difference()
    {
        return std::chrono::duration<double,std::ratio<1,1000>>(end_time - start_time).count();
    }

public:
    static inline bool turn(const double delay)
    {
        if (flag == false)
        {
            start_time = std::chrono::steady_clock::now();
            flag = true;
            return flag;
        }
        else
        {

            end_time = std::chrono::steady_clock::now();
            auto temp=difference();
            if (temp >= delay*1000)
            {
                start_time = end_time;
                return true;
            }
               return false;

        }

    }
  
};
