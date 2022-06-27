

#ifndef BASICS_PROJECT_TEMPLATE_MENU_HPP
#define BASICS_PROJECT_TEMPLATE_MENU_HPP

/*
 * SAMPLE SCENE
 * Copyright © 2021+ Jonatan David Vargas Revollo
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * jontanvargas777@gmail.com
 */

#include <memory>
#include <basics/Scene>
#include <basics/Texture_2D>
#include <basics/Accelerometer>

namespace example
{

    class Menu : public basics::Scene
    {

        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;

    public:



        enum State
        {
            LOADING,
            RUNNING,
        };


        State          state;
        bool           suspended;


        unsigned       canvas_width;
        unsigned       canvas_height;

        Texture_Handle texture;
        Texture_Handle Fondo;






    public:

        Menu();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend    () override;
        void resume     () override;

        void handle     (basics::Event & event) override;
        void update     (float time) override;
        void render     (basics::Graphics_Context::Accessor & context) override;

    private:

        void load ();
        void run  (float time);

    };

}


#endif //BASICS_PROJECT_TEMPLATE_MENU_HPP
