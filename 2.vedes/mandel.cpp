// Mandelbrot png
// Programozó Páternoszter
// 
// Bátfai Norbert, nbatfai@inf.unideb.hu, nbatfai@gmail.com
// http://progpater.blog.hu/2011/03/26/kepes_egypercesek
//
// Fordítás:
// g++ mandelpng.c++ `libpng-config --ldflags` -o mandelpng

#include <iostream>
#include "png++/png.hpp"

int main (int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Hasznalat: ./mandelpng fajlnev";
        return -1;
    }

    // számítás adatai
    double a = -2.0, b = .7,  c = -1.35, d = 1.35;
    int szelesseg = 600, magassag = 600, iteraciosHatar = 1000;

    // png-t készítünk a png++ csomaggal
    png::image <png::rgb_pixel> kep (szelesseg, magassag);

    // a számítás
    double dx = (b-a)/szelesseg;
    double dy = (d-c)/magassag;
    double reC, imC, reZ, imZ, ujreZ, ujimZ;
    // Hány iterációt csináltunk?
    int iteracio = 0;
    std::cout << "Szamitas";
    // Végigzongorázzuk a szélesség x magasság rácsot:
    for (int j=0; j<magassag; ++j) {
        //sor = j;
        for (int k=0; k<szelesseg; ++k) {
            // c = (reC, imC) a rács csomópontjainak
            // megfelelő komplex szám
            reC = a+k*dx;
            imC = d-j*dy;
            // z_0 = 0 = (reZ, imZ)
            reZ = 0;
            imZ = 0;
            iteracio = 0;
            // z_{n+1} = z_n * z_n + c iterációk
            // számítása, amíg |z_n| < 2 vagy még
            // nem értük el a 255 iterációt, ha
            // viszont elértük, akkor úgy vesszük,
            // hogy a kiinduláci c komplex számra
            // az iteráció konvergens, azaz a c a
            // Mandelbrot halmaz eleme
            while (reZ*reZ + imZ*imZ < 4 && iteracio < iteraciosHatar) {
                // z_{n+1} = z_n * z_n + c
                ujreZ = reZ*reZ - imZ*imZ + reC;
                ujimZ = 2*reZ*imZ + imC;
                reZ = ujreZ;
                imZ = ujimZ;

                ++iteracio;

            }

            kep.set_pixel(k, j, png::rgb_pixel(255-iteracio%256,
                                               255-iteracio%256, 255-iteracio%256));
        }
        std::cout << "." << std::flush;
    }

    kep.write (argv[1]);
    std::cout << argv[1] << " mentve" << std::endl;
}

