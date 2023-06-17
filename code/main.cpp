/*
 * MAIN
 * Copyright © 2023 Jonatan David Vargas Revollo
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/Director>
#include <basics/enable>
#include <basics/opengles/OpenGL_ES2>
#include <basics/Window>

#include "Sample_Scene.hpp"

#include <basics/Log>

using namespace basics;
using namespace example;
using namespace std;

int main ()
{
    // Es necesario habilitar un backend gráfico antes de nada:

    enable< basics::OpenGL_ES2 > ();

    // Se crea una escena y se inicia mediante el Director:

    director.run_scene (shared_ptr< Scene >(new Sample_Scene));

    return 0;
}
