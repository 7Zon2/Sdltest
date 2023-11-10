#include "mainHead.hpp"
#include "entities.hpp"
#include <SDL3/SDL_main.h> // only include this one in the source file with main()!




template<typename key>
void render_bullets() noexcept
{

    for (auto i = stockman::get_head<key>(); i != stockman::get_tail<key>(); ++i)
    {

        if (i->get_data().y > -30)
        {
            i->change_life(false);
            i->hit();
            Texturation::blit(*Engine::CreateEngine().render, *i);
        }

    }

}



void test_shoot(Weapons& obj)
{

    if (obj.get_data().y > -30)
    {
        obj.hit();
        Texturation::blit(*Engine::CreateEngine().render,obj);
    }
    else
    {
        Texturation::align_center(*Engine::CreateEngine().render,obj);
    }
}


void do_player(mainHero& Ship)
{
    if (App::keyboard[SDL_SCANCODE_UP])
    {
        Ship.change_coordinate(0, -7);
    }
    if (App::keyboard[SDL_SCANCODE_DOWN])
    {
        Ship.change_coordinate(0, 7);
    }
    if (App::keyboard[SDL_SCANCODE_RIGHT])
    {
        Ship.change_coordinate(7, 0);
    }
    if (App::keyboard[SDL_SCANCODE_LEFT])
    {
        Ship.change_coordinate(-7, 0);
    }


    Texturation::blit(*Engine::CreateEngine().render,Ship);


    if (App::keyboard[SDL_SCANCODE_SPACE] != false)
    {
        Ship.hit();
    }

}



int main(int argc, char* argv[])
{

    mainHero Ship{100,100,100,100};


    Texturation::align_center(*Engine::CreateEngine().render, Ship);
    Texturation::align_center(*Engine::CreateEngine().render,Ship.get_weapon());


    while (1)
    {
        Engine::CreateEngine().prepareScene();

        Engine::CreateEngine().doInput();

        do_player(Ship);

        render_bullets<default_weapon>();
       
        Engine::CreateEngine().presentScene();

        SDL_Delay(16);
    }

    return 0;
}
