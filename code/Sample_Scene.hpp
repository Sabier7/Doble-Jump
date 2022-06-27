/*
 * SAMPLE SCENE
 * Copyright © 2021+ Jonatan David Vargas Revollo
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * jontanvargas777@gmail.com
 */
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <basics/Atlas>
#include <basics/Scene>
#include <basics/Raster_Font>
#include <basics/Texture_2D>
#include <basics/Accelerometer>
#include <basics/vector>
#include <basics/Canvas>
#include <basics/Timer>


namespace sf{}

namespace example
{

    using basics::Atlas;
    using basics::Canvas;
    using basics::Point2f;
    using basics::Size2f;
    using basics::Timer;
    using basics::Vector2f;
    using basics::Vector3f;

    using std::map;
    using std::shared_ptr;
    using std::string;
    using std::vector;
    class Sample_Scene : public basics::Scene
    {

        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;
        typedef std::unique_ptr< basics::Atlas       > Atlas_Handle;
        typedef std::unique_ptr< basics::Raster_Font >  Font_Handle;

    public:

        enum Status
        {
            PLAYING,
            PAUSED,
            GAME_OVER,
        };
        enum State
        {
            LOADING,
            RUNNING,
            //ERROR,
        };
        struct Box
        {
            Vector2f position;
            Size2f   size;
            Vector3f color;


            Box() : position{ 0, 0 }, size{ 0, 0 }, color{ 0, 0, 0 }
            {
            }

            Box(const Vector2f & position, const Size2f & size, const Vector3f & color)
                    :
                    position(position), size(size), color(color)
            {
            }

            basics::Point2f center () const
            {
                return position + Vector2f(size[0], size[1]) * 0.5f;
            }

            bool contains (const Vector2f & point) const
            {
                return
                        point[0] > position[0] &&
                        point[1] > position[1] &&
                        point[0] < position[0] + size[0] &&
                        point[1] < position[1] + size[1];
            }

            virtual void render (basics::Canvas & canvas) const
            {
                canvas.set_color (color[0], color[1], color[2]);
                canvas.fill_rectangle (position, size);
            }
        };

        struct Sprite : Box
        {
            const Atlas::Slice * slice;
            Vector2f speed;
            bool     visible;

            Sprite() : slice(nullptr), speed{ 0.f, 0.f }, visible(true)
            {
            }

            Sprite(const Vector2f & position, const Size2f & size, const Atlas::Slice & slice)
                    :
                    Box(position, size, { 1, 1, 1 }),
                    slice(&slice),
                    speed(0, 0),
                    visible(true)
            {
            }

            virtual void update (float delta)
            {
                position += speed * delta;

            }

            void render (basics::Canvas & canvas) const override
            {
                if (slice && visible)
                {
                    canvas.fill_rectangle (position, size, slice, basics::BOTTOM | basics::LEFT);
                }
            }
        };
        struct Building : public Box
        {
            static constexpr float window_width  =  20.f;
            static constexpr float window_height =  40.f;
            static constexpr float window_step_x =  55.f;
            static constexpr float window_step_y = 100.f;

            vector< Box > windows;

            Building(float left_x,float left_y, float width, int min_height, int max_height);

            virtual void render (Canvas & canvas) const override
            {
                Box::render (canvas);

                for (auto & window : windows) window.render (canvas);
            }
        };

        struct Plataforma
        {
            float center_X;
            float Top_Y;
            float widthP;
            float heightP;


        };


        struct Animated_Sprite : Sprite
        {
        public:

            typedef vector< const Atlas::Slice * > Animation;
            typedef map   < string, Animation    > Animation_Set;

        protected:

            static constexpr float keyframe_duration = 0.1f;

            Animation_Set animations;

            Animation         * active_animation;
            Animation::iterator active_keyframe;
            float               keyframe_elapsed;

        public:

            Animated_Sprite
                    (
                            const Vector2f & initial_position,
                            const string & default_animation_id,
                            const Animation_Set & given_animations
                    ) :
                    animations(given_animations)
            {
                position = initial_position;

                set_animation (default_animation_id);
            }

            void set_animation (const string & id)
            {
                active_animation = &animations[id];
                active_keyframe  =  active_animation->begin ();

                update (keyframe_elapsed = 0.f);
            }

            void update (float delta) override
            {
                Sprite::update (delta);


                if (active_animation)
                {
                    if ((keyframe_elapsed += delta) >= keyframe_duration)
                    {
                        if (++active_keyframe == active_animation->end ()) active_keyframe = active_animation->begin ();

                        keyframe_elapsed = 0.f;
                    }

                    slice = *active_keyframe;
                    size  = { slice->width, slice->height };
                }
            }

        };

        Font_Handle  font;
        struct postPlatfm
        {
            int platX,platY;
        };
        //array plataformas
        postPlatfm Plat[20];
       // Status  status;


        State          state;
        bool           suspended;
        bool           graviton;
        bool           touch;
        unsigned       canvas_width;
        unsigned       canvas_height;

        Texture_Handle texture;
        Texture_Handle jumper;
        Texture_Handle fondo;
        Texture_Handle plataforma;

        float origen;
        float bajar;
        float salto;
        bool jump;
        int posJumperX;
        int posJumperY;
        float    x;
        float    y;
        float    speed;
        bool isJumping;

        float speed_X, speed_Y, MAX_SPEED_Y,impuls;
        static constexpr float GRAVITY = 10.f;
        static constexpr float HORIZONTAL_SPEED = 3.f;

        int altura = rand() ;
        int altura1 = rand() ;
        int altura2 = rand() ;
        int altura3 = rand() ;
        int altura4 = rand() ;
        int altura5 = rand() ;

        int posXrandom = rand() ;
        int posXrandom1 = rand() ;
        int posXrandom2 = rand() ;
        int posXrandom3 = rand() ;
        int posXrandom4 = rand() ;
        int posXrandom5 = rand() ;
        int diferencia;


    public:

        Sample_Scene();

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
        void draw_slice (basics::Canvas * canvas, const basics::Point2f & where, basics::Atlas & atlas, basics::Id slice_id);

        static constexpr unsigned number_of_buildings =    10;
        static constexpr unsigned number_of_colors    =    2;
        static constexpr float    banana_speed_scale  =    2.f;
        static constexpr float    gravity_force       = -100.f * banana_speed_scale;

        static const Vector3f predefined_colors[number_of_colors];

        shared_ptr< Atlas >            atlas;

        vector< Building >             buildings;
        vector< shared_ptr< Sprite > > holes;
        vector< shared_ptr< Animated_Sprite > > gorillas;
        shared_ptr< Sprite >           banana;
        //shared_ptr< Explosion >        explosion;
        shared_ptr< Sprite >           sun;
        shared_ptr< Sprite >           game_over;

        vector< Box    * >             background_boxes;
        vector< Box    * >             foreground_boxes;
        vector< Sprite * >             updateable_sprites;

        Status   status;
       // bool     suspended;                                 ///< true cuando la aplicación está en segundo plano

       // unsigned canvas_width;                              ///< Resolución virtual del display
        //unsigned canvas_height;

        Animated_Sprite * player_gorilla;
        Animated_Sprite * touched_gorilla;
        Vector2f touch_location;

        Timer    timer;
    private:

        void rebuild_scene      ();
        void shoot_banana       ();
        void make_explosion     ();
        void make_hole          ();
        void player_lost_game   ();
        void draw_thick_segment (Canvas & canvas, const Vector2f & a, const Vector2f & b);
    };


}
