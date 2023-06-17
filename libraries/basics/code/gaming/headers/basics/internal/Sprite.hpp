/*
 * SPRITE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef SPRITE_HEADER
#define SPRITE_HEADER

    #include <cassert>
    #include <memory>
    #include <basics/Atlas>
    #include <basics/Canvas>
    #include <basics/Texture_2D>
    #include <basics/Vector>

    namespace basics
    {

        /** Representa una imagen 2D rectangular que ofrece cierta funcionalidad extra para su manipulación
          * en el contexto de videojuegos 2D.
          */
        class Sprite
        {
        protected:

            typedef const Atlas::Slice Slice;

            Texture_2D  * texture;                  ///< Textura en la que está la imagen del sprite (puede ser nullptr).
            Atlas       * atlas;                    ///< Atlas en el que está la imagen del sprite (puede ser nullptr).
            Slice       * slice;                    ///< Slice que se debe usar cuando se tiene un atlas.

            Size2f        size;                     ///< Tamaño del sprite (normalmente en coordenadas virtuales).
            float         scale;                    ///< Escala el tamaño del sprite. Por defecto es 1.

            Point2f       position;                 ///< Posición del sprite (normalmente en coordenadas virtuales).
            int           anchor;                   ///< Indica qué punto de la textura se colocará en 'position' (x,y).

            Vector2f      speed;                    ///< Velocidad a la que se mueve el sprite. Usar el valor por defecto (0,0) para dejarlo quieto.

            bool          visible;                  ///< Indica si el sprite se debe actualizar y dibujar o no. Por defecto es true.

        public:

            /**
             * Inicializa una nueva instancia de Sprite.
             * @param texture Puntero a la textura en la que está su imagen. No debe ser nullptr.
             */
            Sprite(Texture_2D * texture);

            /**
             * Inicializa una nueva instancia de Sprite.
             * @param texture Puntero al atlas en la que está su imagen. No debe ser nullptr.
             * @param slice_id ID del slice perteneciente al atlas que se debe usar.
             */
            Sprite(Atlas * atlas, Id slice_id);

            /**
             * Destructor virtual para facilitar heredar de esta clase si fuese necesario.
             */
            virtual ~Sprite() = default;

        public:

            // Getters (con nombres autoexplicativos):

            const Size2f     get_size       () const { return  size        * scale; }
            const float      get_width      () const { return  size.width  * scale; }
            const float      get_height     () const { return  size.height * scale; }
            const Point2f  & get_position   () const { return  position;    }
            const float    & get_position_x () const { return  position[0]; }
            const float    & get_position_y () const { return  position[1]; }
            const Vector2f & get_speed      () const { return  speed;       }
            const float    & get_speed_x    () const { return  speed[0];    }
            const float    & get_speed_y    () const { return  speed[1];    }

            float get_left_x () const
            {
                return
                    (anchor & 0x3) == basics::LEFT  ? position[0] :
                    (anchor & 0x3) == basics::RIGHT ? position[0] - get_width () :
                     position[0] - get_width () * .5f;
            }

            float get_right_x () const
            {
                return get_left_x () + get_width ();
            }

            float get_bottom_y () const
            {
                return
                    (anchor & 0xC) == basics::BOTTOM ? position[1] :
                    (anchor & 0xC) == basics::TOP    ? position[1] - get_height () :
                     position[1] - get_height () * .5f;
            }

            float get_top_y () const
            {
                return get_bottom_y () + get_height ();
            }

            bool is_visible () const
            {
                return  visible;
            }

            bool is_not_visible () const
            {
                return !visible;
            }

        public:

            // Setters (con nombres autoexplicativos):

            void set_slice (Id slice_id, bool reset_size = true)
            {
                assert(atlas != nullptr);

                slice = atlas->get_slice (slice_id);

                assert(slice != nullptr);

                if (reset_size) size = { slice->width, slice->height };
            }

            void set_size (const Size2f & new_size)
            {
                size = new_size;
            }

            void set_width (const float & new_width)
            {
                size.width = new_width;
            }

            void set_height (const float & new_height)
            {
                size.height = new_height;
            }

            void set_scale (float new_scale)
            {
                scale = new_scale;
            }

            void set_position (const Point2f & new_position)
            {
                position = new_position;
            }

            void set_position_x (const float & new_position_x)
            {
                position.coordinates.x () = new_position_x;
            }

            void set_position_y (const float & new_position_y)
            {
                position.coordinates.y () = new_position_y;
            }

            void set_anchor (int new_anchor)
            {
                anchor = new_anchor;
            }

            void set_speed (const Vector2f & new_speed)
            {
                speed = new_speed;
            }

            void set_speed_x (const float & new_speed_x)
            {
                speed.coordinates.x () = new_speed_x;
            }

            void set_speed_y (const float & new_speed_y)
            {
                speed.coordinates.y () = new_speed_y;
            }

        public:

            /**
             * Hace que el sprite no se actualice ni se dibuje.
             */
            void hide ()
            {
                visible = false;
            }

            /**
             * Hace que el sprite no se actualice ni se dibuje.
             */
            void show ()
            {
                visible = true;
            }

        public:

            /**
             * Comprueba si el área envolvente rectangular de este sprite se solapa con la de otro.
             * @param other Referencia al otro sprite.
             * @return true si las áreas se solapan o false en caso contrario.
             */
            bool intersects (const Sprite & other);

            /**
             * Comprueba si un punto está dentro del sprite.
             * Se puede usar para, por ejemplo, comprobar si el punto donde el usuario ha tocado la
             * pantalla está dentro del sprite.
             * @param point Referencia al punto que se comprobará.
             * @return true si el punto está dentro o false si está fuera.
             */
            bool contains (const Point2f & point);

        public:

            /**
             * Actualiza la posición del sprite automáticamente en función de su velocidad, pero
             * solo cuando es visible.
             * @param time Fracción de tiempo que se debe avanzar.
             */
            virtual void update (float time)
            {
                if (visible)
                {
                    Vector2f displacement = speed * time;

                    position.coordinates.x () += displacement.coordinates.x ();
                    position.coordinates.y () += displacement.coordinates.y ();
                }
            }

            /**
             * Dibuja la imagen del sprite automáticamente, pero solo cuando es visible.
             * @param canvas Referencia al Canvas que se debe usar para dibujar la imagen.
             */
            virtual void render (Canvas & canvas)
            {
                if (visible)
                {
                    if (texture)
                        canvas.fill_rectangle (position, size * scale, texture, anchor);
                    else
                        canvas.fill_rectangle (position, size * scale, slice,   anchor);
                }
            }

        };

    }

#endif
