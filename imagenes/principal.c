#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagen.h"

#define DMASK 3

unsigned char *reservar_memoria(uint32_t width, uint32_t height);
void RGBtoGray(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void GraytoRGB(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void brilloImagen(unsigned char *imagenGray, uint32_t width, uint32_t height);
void filtroPB(unsigned char *imagenGray, unsigned char *imagenF, uint32_t width, uint32_t height);
void Sobel(unsigned char *imagenGray, unsigned char *imagenS, uint32_t width, uint32_t height);

int main()
{
    bmpInfoHeader info;
    unsigned char *imagenRGB, *imagenGray, *imagenFiltrada;

    imagenRGB = abrirBMP("linux.bmp", &info);
    displayInfo(&info);

    imagenGray = reservar_memoria(info.width, info.height);
    imagenFiltrada = reservar_memoria(info.width, info.height);
    memset(imagenFiltrada, 255, info.width * info.height);

    RGBtoGray(imagenRGB, imagenGray, info.width, info.height);

    //brilloImagen(imagenGray, info.width, info.height);
    //filtroPB(imagenGray, imagenFiltrada, info.width, info.height);
    Sobel(imagenGray, imagenFiltrada, info.width, info.height);

    GraytoRGB(imagenRGB, imagenFiltrada, info.width, info.height);

    guardarBMP("linuxSobel.bmp", &info, imagenRGB);

    free(imagenRGB);
    free(imagenGray);
    free(imagenFiltrada);

    return 0;
}

void filtroPB(unsigned char *imagenGray, unsigned char *imagenF, uint32_t width, uint32_t height)
{
    register int x, y, xm, ym;
    int indicem, indicei, conv;
    int mascara[DMASK * DMASK] = {
        1, 1, 1,
        1, 1, 1,
        1, 1, 1};
    for (y = 0; y <= height - DMASK; y++)
    {
        for (x = 0; x <= width - DMASK; x++)
        {
            indicem = 0;
            conv = 0;
            for (ym = 0; ym < DMASK; ym++)
            {
                for (xm = 0; xm < DMASK; xm++)
                {
                    indicei = (y + ym) * width + (x + xm);
                    conv += mascara[indicem++] * imagenGray[indicei];
                }
            }
            conv = conv / 9;
            indicei = (y + 1) * width + (x + 1);
            imagenF[indicei] = conv;
        }
    }
}

void Sobel(unsigned char *imagenGray, unsigned char *imagenS, uint32_t width, uint32_t height)
{
    register int x, y, xm, ym;
    int indicem, indicei, convFil, convCol;
    int gradienteFila[DMASK * DMASK] = {
        1, 0, -1,
        2, 0, -2,
        1, 0, -1};
    int gradienteColumna[DMASK * DMASK] = {
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1};
    for (y = 0; y <= height - DMASK; y++)
    {
        for (x = 0; x <= width - DMASK; x++)
        {
            indicem = 0;
            convFil = 0;
            convCol = 0;
            for (ym = 0; ym < DMASK; ym++)
            {
                for (xm = 0; xm < DMASK; xm++)
                {
                    indicei = (y + ym) * width + (x + xm);
                    convFil += gradienteFila[indicem] * imagenGray[indicei];
                    convCol += gradienteColumna[indicem++] * imagenGray[indicei];
                }
            }
            convFil = convFil >> 2;
            convCol = convCol >> 2;
            indicei = (y + 2) * width + (x + 2);
            imagenS[indicei] = (unsigned char)sqrt((convFil * convFil) + (convCol * convCol));
        }
    }
}

void brilloImagen(unsigned char *imagenGray, uint32_t width, uint32_t height)
{
    register int indiceGray;
    int pixel;
    for (indiceGray = 0; indiceGray < (height * width); indiceGray++)
    {
        pixel = imagenGray[indiceGray] + 70;
        imagenGray[indiceGray] = (pixel > 255) ? 255 : (unsigned char)pixel;
    }
}

unsigned char *reservar_memoria(uint32_t width, uint32_t height)
{
    unsigned char *imagen;
    imagen = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    if (imagen == NULL)
    {
        perror("Error en la memoria de la imagen");
        exit(EXIT_FAILURE);
    }
    return imagen;
}

void RGBtoGray(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height)
{
    unsigned char nivelGris;
    int indiceGray, indiceRGB;
    for (indiceGray = 0, indiceRGB = 0; indiceGray < (height * width); indiceGray++, indiceRGB += 3)
    {
        nivelGris = (imagenRGB[indiceRGB] * 30 + imagenRGB[indiceRGB + 1] * 59 + imagenRGB[indiceRGB + 2] * 11) / 100;
        imagenGray[indiceGray] = nivelGris;
    }
}

void GraytoRGB(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height)
{
    int indiceGray, indiceRGB;
    for (indiceGray = 0, indiceRGB = 0; indiceGray < (height * width); indiceGray++, indiceRGB += 3)
    {
        imagenRGB[indiceRGB] = imagenGray[indiceGray];
        imagenRGB[indiceRGB + 1] = imagenGray[indiceGray];
        imagenRGB[indiceRGB + 2] = imagenGray[indiceGray];
    }
}

/*
void RGBtoGray(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height)
{
    unsigned char nivelGris;
    int indiceGray, indiceRGB;
    // for (y = 0; y < height; y++)
    // {
    for (indiceGray = 0, indiceRGB = 0; indiceGray < (height * width); indiceGray++, indiceRGB += 3)
    {
        //nivelGris = (imagenRGB[indiceRGB] + imagenRGB[indiceRGB + 1] + imagenRGB[indiceRGB + 2]) / 3;
        nivelGris = (imagenRGB[indiceRGB] * 30 + imagenRGB[indiceRGB + 1] * 59 + imagenRGB[indiceRGB + 2] * 11) / 100;
        imagenGray[indiceGray] = nivelGris;
    }
    // }
}

void GraytoRGB(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height)
{
    register int x, y;
    int indiceGray, indiceRGB;
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            indiceGray = (y * width + x);
            indiceRGB = indiceGray * 3;
            imagenRGB[indiceRGB] = imagenGray[indiceGray];
            imagenRGB[indiceRGB + 1] = imagenGray[indiceGray];
            imagenRGB[indiceRGB + 2] = imagenGray[indiceGray];
        }
    }
}
*/