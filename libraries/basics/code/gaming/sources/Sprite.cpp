/*
 * SPRITE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/Sprite>

namespace basics
{

    Sprite::Sprite(Texture_2D * texture)
    :
        texture (texture),
        atlas   (nullptr),
        slice   (nullptr)
    {
        assert(texture != nullptr);

        size     = { texture->get_width (), texture->get_height () };
        scale    = 1.f;
        position = { 0.f, 0.f };
        anchor   = basics::CENTER;
        speed    = { 0.f, 0.f };
        visible  = true;
    }

    Sprite::Sprite(Atlas * atlas, Id slice_id)
    :
        texture (nullptr),
        atlas   (atlas  )
    {
        assert(atlas != nullptr);

        set_slice (slice_id);

        scale    = 1.f;
        position = { 0.f, 0.f };
        anchor   = basics::CENTER;
        speed    = { 0.f, 0.f };
        visible  = true;
    }

    bool Sprite::intersects (const Sprite & other)
    {
        // Se determinan las coordenadas de la esquina inferior izquierda y de la superior derecha
        // de este sprite:

        float this_left    = this->get_left_x   ();
        float this_bottom  = this->get_bottom_y ();
        float this_right   = this_left   + this->get_width  ();
        float this_top     = this_bottom + this->get_height ();

        // Se determinan las coordenadas de la esquina inferior izquierda y de la superior derecha
        // del otro sprite:

        float other_left   = other.get_left_x   ();
        float other_bottom = other.get_bottom_y ();
        float other_right  = other_left   + other.get_width  ();
        float other_top    = other_bottom + other.get_height ();

        // Se determina si los rectángulos envolventes de ambos sprites se solapan:

        return !(other_left >= this_right || other_right <= this_left || other_bottom >= this_top || other_top <= this_bottom);
    }

    bool Sprite::contains (const Point2f & point)
    {
        float left_x = get_left_x ();

        if (point.coordinates.x () > left_x)
        {
            float bottom_y = get_bottom_y ();

            if (point.coordinates.y () > bottom_y)
            {
                if (point.coordinates.x () < left_x + get_width ())
                {
                    if (point.coordinates.y () < bottom_y + get_height ())
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

}
