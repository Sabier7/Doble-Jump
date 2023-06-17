/*
 * SAMPLE SCENE
 * Copyright © 2021 Jonatan David vargas Revollo
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Sample_Scene.hpp"
#include <basics/Application>
#include <basics/Canvas>
#include <basics/Display>
#include <basics/Director>
#include <basics/Log>
#include <basics/Window>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <basics/Accelerometer>
#include <basics/Text_Layout>
#include <random>

using namespace basics;
using namespace std;

namespace example
{

    Sample_Scene::Sample_Scene()
    {
        status        = UNINITIALIZED;
        canvas_width  = 1080;
        canvas_height = 1920;
        speed         = 100;
        score         = 0;
        timer         = 0;

    }

    bool Sample_Scene::initialize ()
    {
        suspended = false;

        display.set_prevent_sleep (true);

        return true;
    }

    void Sample_Scene::finalize ()
    {
        display.set_prevent_sleep (false);
    }

    void Sample_Scene::suspend ()
    {
        suspended = true;

        display.set_prevent_sleep (false);
        //accelerometer
        Accelerometer* accelerometer = Accelerometer::get_instance();

        if (accelerometer) accelerometer->switch_off();
    }

    void Sample_Scene::resume ()
    {
        suspended = false;

        display.set_prevent_sleep (true);
        //accelerometer
        Accelerometer* accelerometer = Accelerometer::get_instance();

        if (accelerometer) accelerometer->switch_on();
    }

    void Sample_Scene::handle (basics::Event & event)
    {
        if (!suspended && status == READY) switch (event.id)
            {

                case ID(touch-started):
                {
                    float x = *event[ID(x) ].as< var::Float > ();
                    float y = *event[ID(y) ].as< var::Float > ();
                    Jump = true;
                    //Help
                    if(squares[11]->contains({x,y}) &&squares[11]->is_visible())
                    {
                        visibleText = !visibleText;
                    }
                }
                    //restar game
                    /*
                        if (!playing && gameOver)
                        {
                           inicio();
                        }
                        */
                    // El usuario toca la pantalla
                case ID(touch-moved):
                {
                    float x = *event[ID(x) ].as< var::Float > ();
                    float y = *event[ID(y) ].as< var::Float > ();

                    if (squares[13]->contains({ x, y }) && !playing && squares[13]->is_visible())
                    {
                        Jump= true;
                        for (int i = 0; i < 10; ++i)
                        {
                            squares[i]->show();
                        }
                        squares[13]->hide();
                        playing = true;
                    }
                    //active pause
                    if (squares[9]->contains({ x, y }) && squares[9]->is_visible())
                    {


                        for (int i = 0; i < 10; ++i)
                        {
                            squares[i]->hide();
                        }
                        playing = false;
                        squares[10]->show();
                        squares[11]->show();
                    }
                    //reanudar partida
                    if (squares[10]->contains({ x, y }))
                    {
                        for (int i = 0; i < 10; ++i)
                        {
                            squares[i]->show();
                        }
                        squares[10]->hide();
                        squares[11]->hide();
                        visibleText = false;
                        playing = true;
                    }

                    /*
                    for (auto square : squares)
                    {
                        // square->set_slice (square->contains ({ x, y }) ? ID(play-menu) : ID(blue-square));
                    }
*/
                    break;
                }

                case ID(touch-ended):
                {
                    break;
                }
            }
    }

    void Sample_Scene::update (float delta)
    {
        if (status == UNINITIALIZED)
        {
            if (load_images ()) create_sprites ();
        }
        if(playing) {


            Accelerometer *accelerometer = Accelerometer::get_instance();

            if (accelerometer)
            {
                const Accelerometer::State &acceleration = accelerometer->get_state();
                // Obtener el valor de la aceleración en el eje X
                float acceleration_x = acceleration.x;

                // Modificar la posición del sprite "player" según el valor de la aceleración
                postX = acceleration_x * - speed * delta, 0.f;
                squares[0]->set_position_x(squares[0]->get_position_x() + postX);

                //cambio de lados
                if (squares[0]->get_position_x() > canvas_width) { squares[0]->set_position_x(0); }
                if (squares[0]->get_position_x() < -squares[0]->get_width()) { squares[0]->set_position_x(canvas_width); }

            }
            //impacta
            //Point2f playerLeft = Point2f( squares[0]->get_position_x() - squares[0]->get_width()*0.5f,squares[0]->get_position_y() - squares[0]->get_height()*0.5f);
           // Point2f playerRight = Point2f( squares[0]->get_position_x() + squares[0]->get_width()*0.5f,squares[0]->get_position_y() - squares[0]->get_height()*0.5f);
            Point2f playerLeft = Point2f(  squares[0]->get_left_x(),squares[0]->get_bottom_y());
            Point2f playerRight = Point2f( squares[0]->get_right_x() ,squares[0]->get_bottom_y());

            for (int i = 1; i < 9; ++i)
            {
                if (squares[i]->contains(playerLeft) || squares[i]->contains(playerRight))
                {
                   Jump = true;
                }
            }
            if (squares[0]->get_position_y() > canvas_height *0.5f - 500) {
                Jump = false;
            }

            //salto
            if (Jump)
            {
                //update score

                float y = squares[0]->get_position_y() + (30000 * delta);
                squares[0]->set_position_y(y);
                if (squares[0]->get_position_y() > canvas_height * 0.5f )
                {
                    score++;  // Inicializar la puntuación a cero
                    Text_Layout sample_text(*font, L"Puntos: " + to_wstring(squares[0]->get_speed_y()));
                }
                //Down platform
                for (int i = 1; i < 9; ++i) {
                    squares[i]->set_position_y(squares[i]->get_position_y() - (3000 * delta));
                    if (squares[i]->get_position_y() < 0) {

                        squares[i]->set_position_y(canvas_height);

                        //change platform position x with a random number
                        float random_x = float(0 + rand() % (1080 - 200));
                        squares[i]->set_position_x(random_x);
                    }
                }
            }

            if(!Jump )
            {
                squares[0]->set_position_y(squares[0]->get_position_y() - 1000 * delta);

            }

            if (!font) {
                Graphics_Context::Accessor context = director.lock_graphics_context();

                if (context) {
                    font.reset(new Raster_Font("fonts/impact.fnt", context));
                }
            }
            //best score
            if (score > bestScore) {
                bestScore = score;

            }
            //game-over
            if (squares[0]->get_position_y() < 0)
            {
                for (int i = 0; i < 12; ++i)
                {
                    squares[i]-> hide ();
                }
                //muestra game-over
                squares[12]->show();
                score = 0;
                timer++;
                if (timer > 80) { restart(); }
            }
        }
    }

    void Sample_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                if (status == ERROR)
                    canvas->set_clear_color (1, 0, 0);
                else
                    canvas->set_clear_color (1, 1, 1);

                canvas->clear ();

                for (auto & sprite : sprites)
                {
                    sprite->render (*canvas);
                }
//score
                //score
                if (font )
                {
                    if(playing)
                    {
                        Text_Layout sample_text(*font, L"Puntos: " + to_wstring(score));
                        canvas->draw_text({10, canvas_height - 100}, sample_text, LEFT);

                        Text_Layout bestScore_text(*font, L"Best Score " + to_wstring(bestScore));
                        canvas->draw_text({10, canvas_height - 200}, bestScore_text, LEFT);
                    }


                    //es necesario que el texto sea wstrign ademas para ponerlo la "L"para declararlo
                     helpText = L"Los controles de juego son simples  \nte mueves al girar a los lados el movil \nsi te pasas sales al lado opuesto de la pantalla  \nsi subes por encima de la mitad de la pantalla \nsumas punto";
                    if(visibleText)
                    {
                        Text_Layout helpLayout(*font, helpText);
                        canvas->draw_text({canvas_width*0.01f, squares[11]->get_position_y() - squares[11]->get_height()}, helpLayout, LEFT);
                    }

                }

            }
        }
    }

    bool Sample_Scene::load_images ()
    {
        Graphics_Context::Accessor context = director.lock_graphics_context ();

        if (context)
        {
            texture = Texture_2D::create (ID(texture), context, "background.png");

            if (texture)
            {
                context->add (texture);

                atlas = make_shared< Atlas > ("atlas.sprites", context);

                status = atlas->good () ? READY : ERROR;
            }
            else
                status = ERROR;
        }

        return status == READY;
    }

    void Sample_Scene::create_sprites ()
    {
        sprites.emplace_back (make_shared< Sprite > (texture.get ()));


        auto background = sprites.back ().get ();

        background->set_size   ({ canvas_width, canvas_height });
        background->set_anchor (BOTTOM | LEFT);

        sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(player)));
        squares[0] = sprites.back ().get ();

        for (int i = 1; i < 9 ; ++i)
        {
            sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(platform)));
            squares[i] = sprites.back ().get ();
        }

        sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(pause)));
        squares[9] = sprites.back ().get ();

        sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(resume)));

        squares[10] = sprites.back ().get ();

    sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(help)));
    squares[11] = sprites.back ().get ();

    sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(game-over)));
    squares[12] = sprites.back ().get ();
    sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(play-menu)));
    squares[13] = sprites.back ().get ();

        Vector2f canvas_center{ canvas_width * .5f, canvas_height * .5f };

        inicio();



        //pause position
        squares[9]->set_position( canvas_center);
        squares[9]->set_scale    (2.00f);

        //menu pause
        squares[10]->set_position( canvas_center + Vector2f(0, squares[10]->get_height()));
        squares[10]->set_scale    (2.00f);


        //Button help
        squares[11]->set_position( canvas_center - Vector2f(0, squares[11]->get_height()));
        squares[11]->set_scale    (2.00f);

        //game-over
        squares[12]->set_position (canvas_center);
        squares[12]->set_scale    (2.00f);

        squares[13]->set_position (canvas_center);
        squares[13]->set_scale    (8.00f);
//posicion de los objetos se dan desde su centro
        //plataformas
        for (int i = 2; i < 9; ++i)
        {
            squares[i]->set_size(Size2f(300,50));
            float random_x = float(0 + rand () % (1080 - 200 ));
            squares[i]->set_position(Vector2f(random_x, 300 * i));
        }

        squares[9]->set_position( Vector2f(canvas_width - (squares[9]->get_width()*0.5f), canvas_height- (squares[9]->get_height()*0.5f)));


        /*

        squares[1]->set_position(Vector2f(canvas_width*0.5f, 100));
        squares[1]->set_anchor   (BOTTOM | LEFT);
        squares[1]->set_scale    (1.00f);
        squares[2]->set_position (canvas_center + Vector2f{ -20.f, -20.f });
        squares[2]->set_anchor   (TOP | RIGHT);
        squares[2]->set_scale    (1.25f);
        squares[3]->set_position (canvas_center + Vector2f{ -500.f, -20.f });
        squares[3]->set_anchor   (TOP | LEFT);
        squares[3]->set_scale    (1.50f);

         */
        // postY         = squares[0]->get_position_y() + 100;

    }
    void Sample_Scene::restart()
    {
            squares[13]-> show();
            timer = 0;
            inicio();
            playing = false;
        squares[12]-> hide ();
    }
    void Sample_Scene::inicio()
    {
        Vector2f canvas_center{ canvas_width * .5f, canvas_height * .5f };
        //player
        squares[0]->set_position (canvas_center);
        squares[1]->set_position (Vector2f(canvas_width*0.5f, 200 ));
        squares[1]->set_size(Size2f(300,50));
        //hace invisible
        if(!playing)
        {
            for (int i = 0; i < 13; ++i)
            {
                squares[i]-> hide ();
            }

        }
    }

}
