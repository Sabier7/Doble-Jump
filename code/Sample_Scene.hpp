/*
 * SAMPLE SCENE
 * Copyright © 2023+ Jonatan David Vargas Revollo
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 */

#include <memory>
#include <vector>
#include <basics/Canvas>
#include <basics/Scene>
#include <basics/Sprite>
#include <basics/Vector>
#include <basics/Raster_Font>


namespace example
{
    using    std::shared_ptr;
    using    std::vector;
    using basics::Atlas;
    using basics::Sprite;
    using basics::Texture_2D;
    using std::string;

    class Sample_Scene : public basics::Scene
    {
    private:

        enum
        {
            UNINITIALIZED, READY, ERROR
        }

        status;
        float     postX;                   // util para el accelerometro
        float     timer;                  // tiempo para el reinicio despues de perder
        float     timer_logo;            // tiempo para manter el logo visible
        bool      playing;              // variable para inidicar cuando el juego esta o no en pausa
        bool      Jump;                // india si puede o no saltar
        bool      active_logo;        // permite decidir cuando esta activa el creonometro del logo
        int       score;             // puntuacion que se resetea al morir
        int       bestScore;        // la puntuacion maxima que se logro en todas las partidas
        float     speed;           //permite modificar la velocidad en el eje x

        bool      suspended;                         ///< true cuando la aplicación está en segundo plano
        unsigned  canvas_width;                     ///< Resolución virtual del display
        unsigned  canvas_height;                   ///< Resolución virtual del display
        bool      visibleText;                    //permite que se vea el texto helText
        Sprite*   logo;                          //imagen del logotipo
        Sprite*  menu_help;                     //imagen de help del menu inicial
        std::wstring helpText;                 //el texto de ayuda que se muestra al precionar help

        typedef std::unique_ptr< basics::Raster_Font > Font_Handle;
        Font_Handle font;


        shared_ptr< Texture_2D > texture;
        shared_ptr< Atlas      > atlas;

        vector< shared_ptr< Sprite > > sprites;

        Sprite * squares[15];          //array que tiene los sprite player 10

     public:

        Sample_Scene();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void finalize   () override;
        void suspend    () override;
        void resume     () override;

        void handle (basics::Event & event) override;
        void update (float delta) override;
        void render (basics::Graphics_Context::Accessor & context) override;

     private:

        bool load_images    ();
        void create_sprites ();
        void restart        ();
        void inicio         ();
    };
}
