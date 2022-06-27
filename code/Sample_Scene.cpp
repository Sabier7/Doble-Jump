/*
 * SAMPLE SCENE
 * Copyright © 2021+ Jonatan David Vargas REvollo
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * jontanvargas777@gmail.com
 */

#include "Sample_Scene.hpp"
#include <basics/Canvas>
#include <basics/Accelerometer>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>

#include <cstdlib>

using namespace basics;
using namespace std;


namespace example
{
    constexpr unsigned Sample_Scene::number_of_buildings;
    constexpr unsigned Sample_Scene::number_of_colors;
    constexpr float    Sample_Scene::banana_speed_scale;
    constexpr float    Sample_Scene::gravity_force;

    constexpr float    Sample_Scene::Building::window_width;
    constexpr float    Sample_Scene::Building::window_height;
    constexpr float    Sample_Scene::Building::window_step_x;
    constexpr float    Sample_Scene::Building::window_step_y;

    const Vector3f Sample_Scene::predefined_colors[] =
            {
                    { 0.5f, 1.0f, 1.0f },
                    { 1.0f, 0.5f, 1.0f },
                    // { 0.5f, 0.5f, 0.5f },
            };

    Sample_Scene::Building::Building(float left_x,float left_y, float width, int min_height, int max_height)
    {
        position    = {  float(0 + rand () % (1080 - 200 )), left_y };
        size.width  = width;
        size.height = 40;

        left_x = float(min_height + rand () % (max_height - min_height));
        color       = predefined_colors[ rand () % number_of_colors];

        for (float y = size.height - window_step_y; y > -window_step_y; y -= window_step_y)
        {
            for (float x = left_x + window_step_x; x < left_x + size.width - window_step_x; x += window_step_x)
            {
                windows.emplace_back
                        (
                                Vector2f{ x, y },
                                Size2f{ window_width, window_height },
                                rand () % 2 == 0 ? Vector3f{ 0, 0, 0 } : Vector3f{ 1, 1, 1 }
                        );
            }
        }
    }

    Sample_Scene::Sample_Scene()
    {
        canvas_width  = 1080;
        canvas_height =  1920;
    }

    bool Sample_Scene::initialize ()
    {
        state     = LOADING;
        suspended = false;
        x         = 365;
        y         = 360;
        MAX_SPEED_Y =500;
        speed_Y = 300;
        speed_X = 365;
        impuls = 20;

        altura  = (rand()% 200) + 200 ;
        altura1 = (rand()% 200) + altura;
        altura2 = (rand()% 200)+ altura1;
        altura3 =(rand()% 200) +altura2;
        altura4 = (rand()% 200)+ altura3;
        altura5 = (rand()% 200) +altura4;
        posXrandom  = (rand()% 520) + 100 ;
        posXrandom1 = (rand()% 520) +100 ;
        posXrandom2 = (rand()% 520)+100;
        posXrandom3 =(rand()% 520)+100;
        posXrandom4 = (rand()% 520) +100;
        posXrandom5 = (rand()% 520) +100;
        return true;
        graviton = true;
        diferencia =0;

    }

    void Sample_Scene::suspend ()
    {
        suspended = true;
        Accelerometer * accelerometer = Accelerometer::get_instance ();

        if (accelerometer) accelerometer->switch_off ();
    }

    void Sample_Scene::resume ()
    {
        suspended = false;
        Accelerometer * accelerometer = Accelerometer::get_instance ();

        if (accelerometer) accelerometer->switch_on ();
    }

    void Sample_Scene::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):

                case ID(touch-moved):
                    {
                        x = *event[ID(x)].as< var::Float > ();
                        touch = true;
                    break;
                    }

                case ID(touch-ended):
                {
                    x = x;
                    y = *event[ID(y)].as< var::Float > ();
                    touch = false;
                    break;
                }
            }
        }
    }

    void Sample_Scene::update (float time)
    {
        Accelerometer * accelerometer = Accelerometer::get_instance ();
        float timer = time;
        if (accelerometer)
        {
            const Accelerometer::State & acceleration = accelerometer->get_state ();



            x -= acceleration.x * 30.f  * speed * time;

        }

        if (isJumping)
        {
            y += time;
        }
        else
        {
            y -= time;
        }
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
        }
        switch (status)
        {

            case PAUSED: {}
                break;
            case GAME_OVER:
            {
               // if (timer.get_elapsed_seconds () > 3) rebuild_scene ();
            }
                break;
        }

    }

    void Sample_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && state == RUNNING)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (speed_Y <= 50)//jumper toca plataforma
            {
                graviton = false;
                impuls = 30;

            }
            if(graviton == false)
            {
                impuls--;
                speed_Y += ( impuls  + graviton - 1 );

            }
            if (speed_Y >= MAX_SPEED_Y)
            {
                speed_Y = MAX_SPEED_Y;
                graviton = true;
                diferencia -= graviton  ;
            }
            Accelerometer * accelerometer = Accelerometer::get_instance ();

            if (accelerometer)
            {
                const Accelerometer::State & acceleration = accelerometer->get_state ();

                speed_X -= acceleration.x * 30.f  * speed;

            }

//diferencia -= (graviton - impuls) ;
           // Accelerometer::State s = Accelerometer::get_instance()->get_state();



            if (graviton == true )
            {
                speed_Y -= GRAVITY;

            }

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear        ();
                canvas->set_color    (1, 1, 1);
                canvas->draw_point   ({ 360, 360 });
                canvas->draw_segment ({   0,   0 }, { 1280, 720 });
                canvas->draw_segment ({   0, 720 }, { 1280,   0 });


                if (fondo)
                {
                    canvas->fill_rectangle ({ canvas_width/2, canvas_height/2  }, { 2000, 2000}, fondo.get ());
                }
                for (int i = 0; i > 10 -1 ; ++i)
                {
                    Plat[i].platX = (rand()%520) + 200;
                    Plat[i].platY = rand()%500;

                }
                if(altura1 -altura < 50)
                {
                    altura1 += 50;
                }
                if(altura2 -altura1 < 50)
                {
                    altura2 += 50;
                }
                if(altura3 -altura2 < 50)
                {
                    altura3 += 50;
                }
                if(altura4 - altura3 < 50)
                {
                    altura4 += 50;
                }
                if(altura5 -altura4 < 50)
                {
                    altura5 += 50;
                }
if(speed_X < posXrandom + 100 && speed_X > posXrandom -100 )
{
    diferencia = 0;
}


diferencia += 2;
                if (plataforma)
                {
                  //  for (int i = 0; i < 10 ; ++i) {

                  //  }
                    canvas->fill_rectangle ({ posXrandom, altura - diferencia }, { 200, 50 }, plataforma.get ());
                    canvas->fill_rectangle ({ posXrandom1, altura1 - diferencia}, { 200, 50 }, plataforma.get ());
                    canvas->fill_rectangle ({ posXrandom2, altura2 - diferencia}, { 200, 50 }, plataforma.get ());
                    canvas->fill_rectangle ({ posXrandom3, altura3 - diferencia}, { 200, 50 }, plataforma.get ());
                    canvas->fill_rectangle ({ posXrandom4, altura4 - diferencia}, { 200, 50 }, plataforma.get ());
                    canvas->fill_rectangle ({ posXrandom5, altura5 - diferencia}, { 200, 50 }, plataforma.get ());

                }


                if (isJumping)
                {
                    speed_Y += 1;
                }
                else
                {
                    //y -= time;
                }
                if(altura - diferencia == speed_Y)
                {
                    speed_Y +=1;
                }

                for (int i = 0; i <10 ; ++i)
                {
                    if(plataforma)
                    {
                      //  Vector2f position[i] = { Plat[i].platX,Plat[i].platY};
                    }
                   // canvas
                }
                if(x > canvas_width/2 && touch)
                {
                    speed_X +=10;
                    posJumperX=speed_X;
                }
                if(x < canvas_width/2 && touch)
                {
                    speed_X -=10;
                    posJumperX=speed_X;
                }
                if(!touch)
                {
                    speed_X = posJumperX;
                }
                if(x == 365)
                {
                    speed_X = 365;
                }
                if(speed_X < 0)
                {
                    speed_X = canvas_width;
                }
                if(speed_X > canvas_width)
                {
                    speed_X = 0;
                }
                if (jumper)
                {
                    canvas->fill_rectangle ({ speed_X, speed_Y }, { 100, 100 }, jumper.get ());
                }
                if(altura < 0)
                {
                    altura  = (rand()% 200) + altura5 ;
                }
                if(altura1 < 0)
                {
                    altura1  = (rand()% 200) + altura ;
                }
                if(altura2 < 0)
                {
                    altura2  = (rand()% 200) + altura1 ;
                }
                if(altura3 < 0)
                {
                    altura3  = (rand()% 200) + altura2 ;
                }
                if(altura4 < 0)
                {
                    altura4  = (rand()% 200) + altura3 ;
                }
                if(altura5 < 0)
                {
                    altura5  = (rand()% 200) + altura4 ;
                }

            }
        }
    }

    void Sample_Scene::load ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                texture     = Texture_2D::create (ID(test), context, "test.png");
                fondo       = Texture_2D::create (ID(test), context, "fondo.png");
                jumper      = Texture_2D::create (ID(test), context, "Jumper.png");
                plataforma  = Texture_2D::create (ID(test), context, "plataforma.png");



                if (texture)
                {
                    context->add (texture);

                    state = RUNNING;
                }
                if (fondo)
                {
                    context->add (fondo);

                    state = RUNNING;
                }
                if (jumper)
                {
                    context->add (jumper);

                    state = RUNNING;
                }
                if (plataforma)
                {
                    context->add (plataforma);

                    state = RUNNING;
                }
            }
        }
    }

    void Sample_Scene::run (float )
    {
    }

    void Sample_Scene::rebuild_scene ()
    {

        background_boxes.clear ();
        foreground_boxes.clear ();
        updateable_sprites.clear ();
        buildings.clear ();
        holes.clear ();
        //Jumpers.clear ();
        banana.reset ();
        sun.reset ();

        float building_left_x     = 0.f;
        float building_width      = float(canvas_width / number_of_buildings);

        int   building_min_height = int(canvas_height) / 5;
        int   building_max_height = int(canvas_height) * 3 / 4;
        //float origen              =  float(building_min_height + rand () % (canvas_width - 10));
        float building_left_y     = 0.f;


        for (unsigned count = 0; count < number_of_buildings; ++count)
        {
            buildings.emplace_back (origen ,building_left_y + 50, building_width ,  building_min_height , building_max_height );

            //origen = origen +1.f;
            building_left_x += building_width + 10.f;
            building_left_y += building_width + 100.f;
        }

        auto & building_left  = buildings[1];
        auto & building_right = buildings[number_of_buildings - 2];




        //  auto & building_left  = buildings[number_of_buildings - 2];
        //   auto & building_right = buildings[1];

        auto * Jumper_stand_slice       = atlas->get_slice (ID(Jumper-standing));
        auto * Jumper_throw_left_slice  = atlas->get_slice (ID(Jumper-arm-up-1));
        auto * Jumper_throw_right_slice = atlas->get_slice (ID(Jumper-arm-up-2));

       /* Jumpers.emplace_back
                (

                        new Animated_Sprite
                                (

                                        { posJumperX - Jumper_stand_slice->width * 0.5f , posJumperY + 500 - gravity_force },
                                        "stand",
                                        Animated_Sprite::Animation_Set
                                                {
                                                        { "stand", { Jumper_stand_slice } },
                                                        { "throw", { Jumper_throw_left_slice } },
                                                        { "happy", { Jumper_throw_left_slice, Jumper_throw_right_slice } },
                                                }
                                )
                );

        Jumpers.emplace_back
        (
            new Animated_Sprite
            (
                { building_right.position[0] + building_width * 0.5f - Jumper_stand_slice->width * 0.5f, building_right.size[1] },
                "stand",
                 Animated_Sprite::Animation_Set
                {
                    { "stand", { Jumper_stand_slice } },
                    { "throw", { Jumper_throw_right_slice } },
                    { "happy", { Jumper_throw_right_slice, Jumper_throw_left_slice } },
                }
            )
        );*/

        auto * banana_keyframe_0_slice = atlas->get_slice (ID(banana-0));
        auto * banana_keyframe_1_slice = atlas->get_slice (ID(banana-1));
        auto * banana_keyframe_2_slice = atlas->get_slice (ID(banana-2));
        auto * banana_keyframe_3_slice = atlas->get_slice (ID(banana-3));

        banana = make_shared< Animated_Sprite >
                (
                        Vector2f{ 0.f, 0.f },
                        "roll",
                        Animated_Sprite::Animation_Set
                                {
                                        { "roll", { banana_keyframe_0_slice, banana_keyframe_1_slice, banana_keyframe_2_slice, banana_keyframe_3_slice } }
                                }
                );

        banana->visible = false;

        auto * sun_slice = atlas->get_slice (ID(sun));

        sun = make_shared< Sprite >
                (
                        Vector2f{ (float(canvas_width) - sun_slice->width) * 0.5f, float(canvas_height * 4 / 5)  },
                        Size2f{ sun_slice->width, sun_slice->height },
                        *sun_slice
                );

        auto * game_over_slice = atlas->get_slice (ID(game-over));

        game_over = make_shared< Sprite >
                (
                        Vector2f{ (float(canvas_width) - game_over_slice->width) * 0.5f, (float(canvas_height) - game_over_slice->height) * 0.5f },
                        Size2f{ game_over_slice->width, game_over_slice->height },
                        *game_over_slice
                );

        game_over->visible = false;

        auto * explosion_slice = atlas->get_slice (ID(explosion));


        for (auto & building : buildings)
        {
            background_boxes.push_back (&building);
        }

       // for (auto & Jumper : Jumpers )
       // {
      //      foreground_boxes.push_back (Jumper.get ());
     //       updateable_sprites.push_back (Jumper.get ());
      //  }

        background_boxes.push_back (sun.get ());

        foreground_boxes.push_back (banana.get ());
        updateable_sprites.push_back (banana.get ());



        foreground_boxes.push_back (game_over.get ());

       state = RUNNING;


    }
}
