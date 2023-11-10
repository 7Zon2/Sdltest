#include "mainHead.hpp"
#include "entities.hpp"

    SDL_Texture& Texturation::load_texture(SDL_Renderer& rend,std::string_view file)
    {
        IMG_Init(IMG_INIT_PNG);

        SDL_Texture* texture;

        if (IMG_LoadTexture(&rend, file.data()) == 0)
            ExceptHandler::end( "failure - Image load: ");

        texture = IMG_LoadTexture(&rend, file.data());
    
        return *texture;
    }

    void Texturation::blit(SDL_Renderer& rend, entity_interface& entity ,SDL_RendererFlip flip)
    {
        SDL_FRect rect_sr{0,0,1000,1000};
        SDL_FRect rect_dst=entity.get_data();

        SDL_RenderTextureRotated(&rend,entity.get_texture(), &rect_sr, &rect_dst, 0, nullptr, flip);
    }

    void Texturation::drawRect(SDL_Renderer& rend, float x, float y, float w, float h)
    {
        SDL_FRect rect;
        SDL_SetRenderDrawColor(&rend, 20, 220, 220, 255);
        rect.x = x; rect.y = y; rect.w = w; rect.h = h;
        SDL_RenderRect(&rend, &rect);
    }

    void Texturation::align_center(SDL_Renderer& rend, entity_interface& obj)
    {
        SDL_FRect rect_sr=obj.get_data();

        rect_sr.x = App::SCREEN_WIDTH / 2 - rect_sr.w / 2;
        rect_sr.y = App::SCREEN_HEIGHT / 2 - rect_sr.h / 2;

        obj.set_data(rect_sr);
    }

    Texturation::~Texturation() {}


    SDL_Texture& texturs::load_main_texture()
    {
        if (main_text == nullptr)
        {
            main_text = &Texturation::load_texture(*Engine::CreateEngine().render, "pixel_ship.png");
            return *main_text;
        }
        return *main_text;

    }

    SDL_Texture& texturs::load_main_bullet()
    {
        if (bullet_text == nullptr)
        {
            bullet_text = &Texturation::load_texture(*Engine::CreateEngine().render, "main_bullet.png");
            return *bullet_text;
        }
        return *bullet_text;
    }