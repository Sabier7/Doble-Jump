/*
 * SAMPLE SCENE
 * Copyright © 2023+ Jonatan David Vargas Revollo
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 */

#include "Sample_Scene.hpp"  //referencia de encabezado
#include <basics/Application> //biblioteca base para acceder a eventos como inicializar o destruir
#include <basics/Canvas>  // la ventana que refleja el juego
#include <basics/Display>  // Esta clase permite configurar algunas características del display como el apagado o permanencia la iluminacion de la pantalla
#include <basics/Director> //administra eventos el kernel y otros como el run
#include <basics/Log>
#include <basics/Window>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <basics/Accelerometer>  //biblioteca que permite el uso del accelerometro osea si giro el movil eso me indica y puedo accionar eventos en mi caso movimiento del jugador
#include <basics/Text_Layout>
#include <random> //biblioteca que permite el uso de ramdom en mi caso incremento desde el 0 hasta canvas_hight menos ancho de sprite

using namespace basics;
using namespace std;

namespace example
{
//inicializo las variables que necesito y si quiero declarar alguna en el hpp al igual que los eventos
    Sample_Scene::Sample_Scene()
    {
        status        = UNINITIALIZED;
        canvas_width  = 1080;
        canvas_height = 1920;
        speed         = 100;
        score         = 0;
        timer         = 0;
        timer_logo    = 0;
        active_logo   = false;
    }
/**metodo que permite iniciar siertos parametros al igual que un start *
 * @return util para tener una buena organizacion
 */
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

    /**metodo que permite hacer acciones al dejar el juego en segundo plano
    */
     void Sample_Scene::suspend ()
    {
        suspended = true;

        display.set_prevent_sleep (false);

        //accelerometer se toma la instancia para posteriormente apagarlo ya que no se esta usando la app
        Accelerometer* accelerometer = Accelerometer::get_instance();
        if (accelerometer) accelerometer->switch_off();
    }
    /**Evento que permite reanudar la app al pasar por el suspend
    */
    void Sample_Scene::resume ()
    {
        suspended = false;

        display.set_prevent_sleep (true);

        /**al igual que en el evento de suspende se toma la instancia que se genero para posteriormente encenderla
         */
        Accelerometer* accelerometer = Accelerometer::get_instance();
        if (accelerometer) accelerometer->switch_on();
    }

    void Sample_Scene::handle (basics::Event & event)
    {
        if (!suspended && status == READY) switch (event.id)
            {
                case ID(touch-started):
                {
                    /**Forma de acceder a los puntos en los cuales se esta tocando la pantalla muy util para acceder a las contains de los sprites
                     */
                    float x = *event[ID(x) ].as< var::Float > ();
                    float y = *event[ID(y) ].as< var::Float > ();

                    //Help debe tener ademas de la posicion las cordenadas precionadas el ver que sea visible ya
                    // incluso en invisible esto aplicaria y accionaria la mecanica
                    if(squares[13]->contains({x,y}) &&squares[13]->is_visible())
                    {
                        visibleText = !visibleText;
                    }

                    if(menu_help->contains({x,y}) && menu_help->is_visible())
                    { visibleText = !visibleText; } //hace visible en el menu el texto de ayuda
                }

                    // El usuario toca la pantalla y mantiene presionado y se mueve
                case ID(touch-moved):
                {
                    float x = *event[ID(x) ].as< var::Float > ();
                    float y = *event[ID(y) ].as< var::Float > ();

                    //play button
                    if (squares[15]->contains({ x, y }) && !playing && squares[15]->is_visible())
                    {
                        Jump= true;
                        active_logo= true;
                        logo->show();
                        visibleText = false;
                        squares[15]->hide();
                        menu_help->hide();
                    }

                    //active pause
                    if (squares[11]->contains({ x, y }) && squares[11]->is_visible())
                    {
                        for (int i = 0; i < 12; ++i)
                        {
                            squares[i]->hide();
                        }
                        playing = false;

                        squares[12]->show();
                        squares[13]->show();
                    }

                    //reanudar partida
                    if (squares[12]->contains({ x, y }))
                    {
                        for (int i = 0; i < 12; ++i)
                        {
                            squares[i]->show();
                        }
                        squares[13]->hide();
                        squares[12]->hide();
                        visibleText = false;
                        playing = true;
                    }

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
        if (!font)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context();

            if (context) { font.reset(new Raster_Font("fonts/impact.fnt", context)); }
        }
        if(playing)
        {
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
                if (squares[0]->get_position_x() > canvas_width)
                { squares[0]->set_position_x(0); }

                if (squares[0]->get_position_x() < -squares[0]->get_width())
                { squares[0]->set_position_x(canvas_width); }

            }

            Point2f playerLeft = Point2f(  squares[0]->get_left_x(),squares[0]->get_bottom_y());
            Point2f playerRight = Point2f( squares[0]->get_right_x() -70 ,squares[0]->get_bottom_y());

                for (int i = 1; i < 11; ++i)
                {
                    if (squares[i]->contains(playerLeft) || squares[i]->contains(playerRight) && squares[0]->get_position_y() > squares[i]->get_position_y())
                    {
                        Jump = true;

                    }
                }


            if (squares[0]->get_position_y() > canvas_height *0.5f -300){ Jump = false;}

            //Down platform
            if (squares[0]->get_position_y() > 400)
            {
                for (int i = 1; i < 11; ++i)
                {
                    squares[i]->set_position_y(squares[i]->get_position_y() - 5);

                    if (squares[i]->get_position_y() < 0)
                    {
                        //change platform position x with a random number
                        float random_x = float(0 + rand() % (1080 - 200));
                        squares[i]->set_position (Point2f (random_x, canvas_height));
                        if(Jump)
                        {
                            score++;  // Inicializar la puntuación a cero
                            Text_Layout sample_text(*font, L"Puntos: " + to_wstring(score));

                        }
                    }
                }
            }


            //salto
            if (Jump)
            {
                float y = squares[0]->get_position_y() + 15;
                squares[0]->set_position_y(y);
            }

            if(!Jump )
            {
                squares[0]->set_position_y(squares[0]->get_position_y() - 15);
            }

            //best score
            if (score > bestScore)
            {
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
                squares[14]->show();
                score = 0;
                timer++;
                if (timer > 80) { restart(); }
            }
        }

        if (active_logo)
        {
            timer_logo++;

            if (timer_logo >= 70)
            {
                logo->hide();
                playing = true;
                for (int i = 0; i < 12; ++i)
                {
                    squares[i]->show();

                    timer_logo =0;
                }
                playing     = true;
                timer_logo  =0;
                active_logo = false;
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

        for (int i = 1; i < 11 ; ++i)
        {
            sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(platform)));
            squares[i] = sprites.back ().get ();

        }

        sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(pause)));
        squares[11] = sprites.back ().get ();

        sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(resume)));
        squares[12] = sprites.back ().get ();

        sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(help)));
        squares[13] = sprites.back ().get ();//help

        sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(game-over)));
        squares[14] = sprites.back ().get ();

        sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(play-menu)));
        squares[15] = sprites.back ().get ();

        Vector2f canvas_center{ canvas_width * .5f, canvas_height * .5f };

        sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(platform)));
        logo = sprites.back ().get ();

        sprites.emplace_back (make_shared< Sprite > (atlas.get (), ID(help-menu)));
        menu_help = sprites.back ().get ();
        inicio();
        logo->set_position( canvas_center);
        logo->set_scale    (5.00f);

        menu_help->set_position( canvas_center - Vector2f (0.f, 200.f));
        menu_help->set_scale    (4.00f);

        //pause position
        squares[11]->set_position( Vector2f(canvas_width - (squares[12]->get_width()*0.5f), canvas_height- (squares[11]->get_height()*0.5f)));
        squares[11]->set_scale    (2.00f);

        //reanudar
        squares[12]->set_position( canvas_center + Vector2f(0, squares[12]->get_height()));
        squares[12]->set_scale    (2.00f);


        //Button help
        squares[13]->set_position( canvas_center - Vector2f(0, squares[13]->get_height()));
        squares[13]->set_scale    (2.00f);

        //game-over
        squares[14]->set_position (canvas_center);
        squares[14]->set_scale    (2.00f);

        squares[15]->set_position (canvas_center + Vector2f(0, 200));
        squares[15]->set_scale    (7.00f);

        //posicion de los objetos se dan desde su centro
        //plataformas
        for (int i = 1; i < 11; ++i)
        {
            squares[i]->set_size(Size2f(300,50));
            float random_x = float(0 + rand () % (1080 - 200 ));
            squares[i]->set_position(Vector2f(random_x, 192 * i));
        }
    }
    void Sample_Scene::restart()
    {
        squares[15]-> show();
        menu_help->show();
        timer = 0;
        inicio();
        playing = false;
        squares[14]-> hide (); //game-over invisible
    }
    void Sample_Scene::inicio()
    {
        Vector2f canvas_center{ canvas_width * .5f, canvas_height * .5f };
        //player
        squares[0]->set_position (canvas_center);
        for (int i = 1; i < 11; ++i)
        {
            squares[i]->set_size(Size2f(300,50));
            float random_x = float(0 + rand () % (1080 - 200 ));
            squares[i]->set_position(Vector2f(random_x, 192 * i));
        }

        //hace invisible
        if(!playing)
        {
            for (int i = 0; i < 15; ++i)
            {
                squares[i]-> hide ();
            }
            logo->hide();
        }
    }
}