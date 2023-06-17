/*
 * SAMPLE SCENE
 * Copyright © 2023+ Jonatan David Vargas Revollo 
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
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
        float    postX;
        float    timer;
        bool     playing;
        bool     Jump;
        bool     gameOver;
        bool     paused;
        int      score;
        int      bestScore;
        float    speed;
        bool     suspended;                       ///< true cuando la aplicación está en segundo plano
        unsigned canvas_width;                    ///< Resolución virtual del display
        unsigned canvas_height;
        bool     visibleText; //permite que se vea el texto helText
        std::wstring helpText;



        typedef std::unique_ptr< basics::Raster_Font > Font_Handle;
        Font_Handle font;
        //vector<shared_ptr<>>

        shared_ptr< Texture_2D > texture;
        shared_ptr< Atlas      > atlas;

        vector< shared_ptr< Sprite > > sprites;



        Sprite * squares[8];

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
