//
// Created by jonat on 10/02/2021.
//

#include "Menu.hpp"
#include "Sample_Scene.hpp"

/*
 * SAMPLE SCENE
 * Copyright © 2021+ Jonatan David Vargas Revollo
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * jontanvargas777@gmail.com
 */


#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>
#include <basics/Accelerometer>

using namespace basics;
using namespace std;

namespace example
{

    Menu::Menu()
    {
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool Menu::initialize ()
    {
        state     = LOADING;
        suspended = false;



        return true;

    }

    void Menu::suspend ()
    {
        suspended = true;
    }

    void Menu::resume ()
    {
        suspended = false;
    }

    void Menu::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                {
                    director.run_scene (shared_ptr< Scene >(new Sample_Scene));
                    break;
                }
                case ID(touch-moved):
                case ID(touch-ended):
                {

                    break;
                }
            }
        }
    }

    void Menu::update (float time)
    {
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
        }

    }

    void Menu::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && state == RUNNING)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear        ();
               // canvas->set_color    (1, 1, 1);
              //  canvas->draw_point   ({ 360, 360 });
               // canvas->draw_segment ({   0,   0 }, { 1280, 720 });
              //  canvas->draw_segment ({   0, 720 }, { 1280,   0 });


                if (Fondo)
                {
                    canvas->fill_rectangle ({ canvas_width/2, canvas_height/2  }, { 1280, 720 }, Fondo.get ());
                }
                if (texture)
                {
                    canvas->fill_rectangle ({ canvas_width/2, canvas_height/2  }, { 1000, 500 }, texture.get ());
                }
            }
        }
    }

    void Menu::load ()
    {
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                texture = Texture_2D::create (ID(test), context, "pulsa.png");
                Fondo =  Texture_2D::create (ID(test), context, "fondo.png");

                if (texture)
                {
                    context->add (texture);

                    state = RUNNING;
                }
                if (Fondo)
                {
                    context->add (Fondo);

                    state = RUNNING;
                }
            }
        }
    }

    void Menu::run (float )
    {
    }

}
