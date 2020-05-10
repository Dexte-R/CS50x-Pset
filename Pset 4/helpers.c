#include "helpers.h"
#include <math.h>
#include <stdio.h>


int root(int g_x, int g_y)
{
    float x = pow(g_x, 2);
    float y = pow(g_y, 2);
    float z = x + y;
    int num = round(sqrt(z));

    if (num > 255)
        num = 255;
    return num;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        RGBTRIPLE dummy[width];
        for (int j = 0; j < width; j++)
        {
            dummy[width - 1 - j].rgbtRed = image[i][j].rgbtRed;
            dummy[width - 1 - j].rgbtGreen = image[i][j].rgbtGreen;
            dummy[width - 1 - j].rgbtBlue = image[i][j].rgbtBlue;
        }
        for (int k = 0; k < width; k++)
        {
            image[i][k].rgbtRed = dummy[k].rgbtRed;
            image[i][k].rgbtGreen = dummy[k].rgbtGreen;
            image[i][k].rgbtBlue = dummy[k].rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // dummy 2-d array to store blurred pixels
    RGBTRIPLE dummy[height][width];
    int r_blur = 0;
    int g_blur = 0;
    int b_blur = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // blur pixels in edge (exlucding corners)
            // pixels contained in 2x3 grid (horizontal edge)
            if ((i == 0 || i == height - 1) && (j > 0) && (j < width - 1))
            {
                int k = 0;
                if (i == 0)
                    k = i;
                if (i == height - 1)
                    k = i - 1;

                r_blur = round((image[k][j - 1].rgbtRed + image[k][j].rgbtRed + image[k][j + 1].rgbtRed
                        + image[k + 1][j - 1].rgbtRed + image[k + 1][j].rgbtRed + image[k + 1][j + 1].rgbtRed) / 6.0);

                g_blur = round((image[k][j - 1].rgbtGreen + image[k][j].rgbtGreen + image[k][j + 1].rgbtGreen
                        + image[k + 1][j - 1].rgbtGreen + image[k + 1][j].rgbtGreen + image[k + 1][j + 1].rgbtGreen) / 6.0);

                b_blur = round((image[k][j - 1].rgbtBlue + image[k][j].rgbtBlue + image[k][j + 1].rgbtBlue
                        + image[k + 1][j - 1].rgbtBlue + image[k + 1][j].rgbtBlue + image[k + 1][j + 1].rgbtBlue) / 6.0);
            }
            // pixels contained in 3x2 grid (vertical edge)
            else if ((i > 0) && (i < height - 1) && (j == 0 || j == width - 1))
            {
                int k = 0;
                if (j == 0)
                    k = j;
                if (j == width - 1)
                    k = j - 1;

                r_blur = round((image[i - 1][k].rgbtRed + image[i - 1][k + 1].rgbtRed
                        + image[i][k].rgbtRed + image[i][k + 1].rgbtRed
                        + image[i + 1][k].rgbtRed + image[i + 1][k + 1].rgbtRed) / 6.0);

                g_blur = round((image[i-1][k].rgbtGreen + image[i - 1][k + 1].rgbtGreen
                        + image[i][k].rgbtGreen + image[i][k + 1].rgbtGreen
                        + image[i + 1][k].rgbtGreen + image[i + 1][k + 1].rgbtGreen) / 6.0);

                b_blur = round((image[i-1][k].rgbtBlue + image[i - 1][k + 1].rgbtBlue
                        + image[i][k].rgbtBlue + image[i][k + 1].rgbtBlue
                        + image[i + 1][k].rgbtBlue + image[i + 1][k + 1].rgbtBlue) / 6.0);
            }
            // pixels contained in 3x3 grid
            else if ((i > 0) && (i < height - 1) && (j > 0) && (j < width - 1))
            {
                r_blur = round((image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed
                    + image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed
                    + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 9.0);

                g_blur = round((image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen
                        + image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen
                        + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 9.0);

                b_blur = round((image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue
                        + image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue
                        + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 9.0);
            }
            else
            {
                r_blur = 0;
                g_blur = 0;
                b_blur = 0;
            }
            // store blurred pixels into dummmy array
            dummy[i][j].rgbtRed = r_blur;
            dummy[i][j].rgbtGreen = g_blur;
            dummy[i][j].rgbtBlue = b_blur;
        }
    }
    // blur the pixels in the 4 corners
    // pixels in 4 corners are contained in 2x2 grid
    // upper left corner
    r_blur = round((image[0][0].rgbtRed + image[0][1].rgbtRed
                    + image[1][0].rgbtRed + image[1][1].rgbtRed) / 4.0);

    g_blur = round((image[0][0].rgbtGreen + image[0][1].rgbtGreen
                    + image[1][0].rgbtGreen + image[1][1].rgbtGreen) / 4.0);

    b_blur = round((image[0][0].rgbtBlue + image[0][1].rgbtBlue
                    + image[1][0].rgbtBlue + image[1][1].rgbtBlue) / 4.0);
    dummy[0][0].rgbtRed = r_blur;
    dummy[0][0].rgbtGreen = g_blur;
    dummy[0][0].rgbtBlue = b_blur;

    // upper right corner
    r_blur = round((image[0][width - 2].rgbtRed + image[0][width - 1].rgbtRed
                    + image[1][width - 2].rgbtRed + image[1][width - 1].rgbtRed) / 4.0);

    g_blur = round((image[0][width - 2].rgbtGreen + image[0][width - 1].rgbtGreen
                    + image[1][width - 2].rgbtGreen + image[1][width - 1].rgbtGreen) / 4.0);

    b_blur = round((image[0][width - 2].rgbtBlue + image[0][width - 1].rgbtBlue
                    + image[1][width - 2].rgbtBlue + image[1][width - 1].rgbtBlue) / 4.0);
    dummy[0][width - 1].rgbtRed = r_blur;
    dummy[0][width - 1].rgbtGreen = g_blur;
    dummy[0][width - 1].rgbtBlue = b_blur;

    // bottom left corner
    r_blur = round((image[height - 2][0].rgbtRed + image[height - 2][1].rgbtRed
                    + image[height - 1][0].rgbtRed + image[height - 1][1].rgbtRed) / 4.0);

    g_blur = round((image[height - 2][0].rgbtGreen + image[height - 2][1].rgbtGreen
                    + image[height - 1][0].rgbtGreen + image[height - 1][1].rgbtGreen) / 4.0);

    b_blur = round((image[height - 2][0].rgbtBlue + image[height - 2][1].rgbtBlue
                    + image[height - 1][0].rgbtBlue + image[height - 1][1].rgbtBlue) / 4.0);
    dummy[height - 1][0].rgbtRed = r_blur;
    dummy[height - 1][0].rgbtGreen = g_blur;
    dummy[height - 1][0].rgbtBlue = b_blur;

    // bottom right corner
    r_blur = round((image[height - 2][width - 2].rgbtRed + image[height - 2][width - 1].rgbtRed
                    + image[height - 1][width - 2].rgbtRed + image[height - 1][width - 1].rgbtRed) / 4.0);

    g_blur = round((image[height - 2][width - 2].rgbtGreen + image[height - 2][width - 1].rgbtGreen
                    + image[height - 1][width - 2].rgbtGreen + image[height - 1][width - 1].rgbtGreen) / 4.0);

    b_blur = round((image[height - 2][width - 2].rgbtBlue + image[height - 2][width - 1].rgbtBlue
                    + image[height - 1][width - 2].rgbtBlue + image[height - 1][width - 1].rgbtBlue) / 4.0);
    dummy[height - 1][width - 1].rgbtRed = r_blur;
    dummy[height - 1][width - 1].rgbtGreen = g_blur;
    dummy[height - 1][width - 1].rgbtBlue = b_blur;

    // update the original pxiel array with blurred pixel values
    for (int m = 0; m < height; m++)
    {
        for (int n = 0; n < width; n++)
        {
            image[m][n].rgbtRed = dummy[m][n].rgbtRed;
            image[m][n].rgbtGreen = dummy[m][n].rgbtGreen;
            image[m][n].rgbtBlue = dummy[m][n].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create dummy array to store convoluted pixels
    RGBTRIPLE dummy[height][width];
    int r_conv = 0;
    int g_conv = 0;
    int b_conv = 0;

    int r_x, r_y, g_x, g_y, b_x, b_y;

    // initialize convolution matrix for g_x and g_y as defined by problem
    int Gx[3][3] = { {-1, 0, 1},
                      {-2, 0, 2},
                      {-1, 0, 1} };

    int Gy[3][3] = { {-1, -2, -1},
                      {0, 0, 0},
                      {1, 2, 1} };

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // convolute pixels in boundaries (exlucding corners)
            // pixels contained in 2x3 grid (horizontal boundary)
            if ((i == 0 || i == height - 1) && (j > 0) && (j < width - 1))
            {
                int k = 0;
                int p;
                if (i == 0)
                {
                    k = i;
                    p = 1;
                }
                if (i == height - 1)
                {
                    k = i - 1;
                    p = 0;
                }

                r_x = Gx[p][0] * image[k][j - 1].rgbtRed + Gx[p][1] * image[k][j].rgbtRed + Gx[p][2] * image[k][j + 1].rgbtRed
                    + Gx[p + 1][0] * image[k + 1][j - 1].rgbtRed + Gx[p + 1][1] * image[k + 1][j].rgbtRed + Gx[p + 1][2] * image[k + 1][j + 1].rgbtRed;

                r_y = Gy[p][0] * image[k][j - 1].rgbtRed + Gy[p][1] * image[k][j].rgbtRed + Gy[p][2] * image[k][j + 1].rgbtRed
                    + Gy[p + 1][0] * image[k + 1][j - 1].rgbtRed + Gy[p + 1][1] * image[k + 1][j].rgbtRed + Gy[p + 1][2] * image[k + 1][j + 1].rgbtRed;

                g_x = Gx[p][0] * image[k][j - 1].rgbtGreen + Gx[p][1] * image[k][j].rgbtGreen + Gx[p][2] * image[k][j + 1].rgbtGreen
                    + Gx[p + 1][0] * image[k + 1][j - 1].rgbtGreen + Gx[p + 1][1] * image[k + 1][j].rgbtGreen + Gx[p + 1][2] * image[k + 1][j + 1].rgbtGreen;

                g_y = Gy[p][0] * image[k][j - 1].rgbtGreen + Gy[p][1] * image[k][j].rgbtGreen + Gy[p][2] * image[k][j + 1].rgbtGreen
                    + Gy[p + 1][0] * image[k + 1][j - 1].rgbtGreen + Gy[p + 1][1] * image[k + 1][j].rgbtGreen + Gy[p + 1][2] * image[k + 1][j + 1].rgbtGreen;

                b_x = Gx[p][0] * image[k][j - 1].rgbtBlue + Gx[p][1] * image[k][j].rgbtBlue + Gx[p][2] * image[k][j + 1].rgbtBlue
                    + Gx[p + 1][0] * image[k + 1][j - 1].rgbtBlue + Gx[p + 1][1] * image[k + 1][j].rgbtBlue + Gx[p + 1][2] * image[k + 1][j + 1].rgbtBlue;

                b_y = Gy[p][0] * image[k][j - 1].rgbtBlue + Gy[p][1] * image[k][j].rgbtBlue + Gy[p][2] * image[k][j + 1].rgbtBlue
                    + Gy[p + 1][0] * image[k + 1][j - 1].rgbtBlue + Gy[p + 1][1] * image[k + 1][j].rgbtBlue + Gy[p + 1][2] * image[k + 1][j + 1].rgbtBlue;

                r_conv = root(r_x, r_y);
                g_conv = root(g_x, g_y);
                b_conv = root(b_x, b_y);
            }
            // pixels contained in 3x2 grid (vertical boundary)
            else if ((i > 0) && (i < height - 1) && (j == 0 || j == width - 1))
            {
                int k = 0;
                int p;
                if (j == 0)
                {
                    k = j;
                    p = 1;
                }
                if (j == width - 1)
                {
                    k = j - 1;
                    p = 0;
                }

                r_x = Gx[0][p] * image[i - 1][k].rgbtRed + Gx[0][p + 1] * image[i - 1][k + 1].rgbtRed
                    + Gx[1][p] * image[i][k].rgbtRed + Gx[1][p + 1] * image[i][k + 1].rgbtRed
                    + Gx[2][p] * image[i + 1][k].rgbtRed + Gx[2][p + 1] * image[i + 1][k + 1].rgbtRed;

                r_y = Gy[0][p] * image[i - 1][k].rgbtRed + Gy[0][p + 1] * image[i - 1][k + 1].rgbtRed
                    + Gy[1][p] * image[i][k].rgbtRed + Gy[1][p + 1] * image[i][k + 1].rgbtRed
                    + Gy[2][p] * image[i + 1][k].rgbtRed + Gy[2][p + 1] * image[i + 1][k + 1].rgbtRed;

                g_x = Gx[0][p] * image[i - 1][k].rgbtGreen + Gx[0][p + 1] * image[i - 1][k + 1].rgbtGreen
                    + Gx[1][p] * image[i][k].rgbtGreen + Gx[1][p + 1] * image[i][k + 1].rgbtGreen
                    + Gx[2][p] * image[i + 1][k].rgbtGreen + Gx[2][p + 1] * image[i + 1][k + 1].rgbtGreen;

                g_y = Gy[0][p] * image[i - 1][k].rgbtGreen + Gy[0][p + 1] * image[i - 1][k + 1].rgbtGreen
                    + Gy[1][p] * image[i][k].rgbtGreen + Gy[1][p + 1] * image[i][k + 1].rgbtGreen
                    + Gy[2][p] * image[i + 1][k].rgbtGreen + Gy[2][p + 1] * image[i + 1][k + 1].rgbtGreen;

                b_x = Gx[0][p] * image[i - 1][k].rgbtBlue + Gx[0][p + 1] * image[i - 1][k + 1].rgbtBlue
                    + Gx[1][p] * image[i][k].rgbtBlue + Gx[1][p + 1] * image[i][k + 1].rgbtBlue
                    + Gx[2][p] * image[i + 1][k].rgbtBlue + Gx[2][p + 1] * image[i + 1][k + 1].rgbtBlue;

                b_y = Gy[0][p] * image[i - 1][k].rgbtBlue + Gy[0][p + 1] * image[i - 1][k + 1].rgbtBlue
                    + Gy[1][p] * image[i][k].rgbtBlue + Gy[1][p + 1] * image[i][k + 1].rgbtBlue
                    + Gy[2][p] * image[i + 1][k].rgbtBlue + Gy[2][p + 1] * image[i + 1][k + 1].rgbtBlue;

                g_conv = root(g_x, g_y);
                r_conv = root(r_x, r_y);
                b_conv = root(b_x, b_y);

            }
            // pixels contained in 3x3 grid
            else if ((i > 0) && (i < height - 1) && (j > 0) && (j < width - 1))
            {
                r_x = Gx[0][0] * image[i - 1][j - 1].rgbtRed + Gx[0][1] * image[i - 1][j].rgbtRed + Gx[0][2] * image[i - 1][j + 1].rgbtRed
                    + Gx[1][0] * image[i][j - 1].rgbtRed + Gx[1][1] * image[i][j].rgbtRed + Gx[1][2] * image[i][j + 1].rgbtRed
                    + Gx[2][0] * image[i + 1][j - 1].rgbtRed + Gx[2][1] * image[i + 1][j].rgbtRed + Gx[2][2] * image[i + 1][j + 1].rgbtRed;

                r_y = Gy[0][0] * image[i - 1][j - 1].rgbtRed + Gy[0][1] * image[i - 1][j].rgbtRed + Gy[0][2] * image[i - 1][j + 1].rgbtRed
                    + Gy[1][0] * image[i][j - 1].rgbtRed + Gy[1][1] * image[i][j].rgbtRed + Gy[1][2] * image[i][j + 1].rgbtRed
                    + Gy[2][0] * image[i + 1][j - 1].rgbtRed + Gy[2][1] * image[i + 1][j].rgbtRed + Gy[2][2] * image[i + 1][j + 1].rgbtRed;

                g_x = Gx[0][0] * image[i - 1][j - 1].rgbtGreen + Gx[0][1] * image[i - 1][j].rgbtGreen + Gx[0][2] * image[i - 1][j + 1].rgbtGreen
                    + Gx[1][0] * image[i][j - 1].rgbtGreen + Gx[1][1] * image[i][j].rgbtGreen + Gx[1][2] * image[i][j + 1].rgbtGreen
                    + Gx[2][0] * image[i + 1][j - 1].rgbtGreen + Gx[2][1] * image[i + 1][j].rgbtGreen + Gx[2][2] * image[i + 1][j + 1].rgbtGreen;

                g_y = Gy[0][0] * image[i - 1][j - 1].rgbtGreen + Gy[0][1] * image[i - 1][j].rgbtGreen + Gy[0][2] * image[i - 1][j + 1].rgbtGreen
                    + Gy[1][0] * image[i][j - 1].rgbtGreen + Gy[1][1] * image[i][j].rgbtGreen + Gy[1][2] * image[i][j + 1].rgbtGreen
                    + Gy[2][0] * image[i + 1][j - 1].rgbtGreen + Gy[2][1] * image[i + 1][j].rgbtGreen + Gy[2][2] * image[i + 1][j + 1].rgbtGreen;

                b_x = Gx[0][0] * image[i - 1][j - 1].rgbtBlue + Gx[0][1] * image[i - 1][j].rgbtBlue + Gx[0][2] * image[i - 1][j + 1].rgbtBlue
                    + Gx[1][0] * image[i][j - 1].rgbtBlue + Gx[1][1] * image[i][j].rgbtBlue + Gx[1][2] * image[i][j + 1].rgbtBlue
                    + Gx[2][0] * image[i + 1][j - 1].rgbtBlue + Gx[2][1] * image[i + 1][j].rgbtBlue + Gx[2][2] * image[i + 1][j + 1].rgbtBlue;

                b_y = Gy[0][0] * image[i - 1][j - 1].rgbtBlue + Gy[0][1] * image[i - 1][j].rgbtBlue + Gy[0][2] * image[i - 1][j + 1].rgbtBlue
                    + Gy[1][0] * image[i][j - 1].rgbtBlue + Gy[1][1] * image[i][j].rgbtBlue + Gy[1][2] * image[i][j + 1].rgbtBlue
                    + Gy[2][0] * image[i + 1][j - 1].rgbtBlue + Gy[2][1] * image[i + 1][j].rgbtBlue + Gy[2][2] * image[i + 1][j + 1].rgbtBlue;

                r_conv = root(r_x, r_y);
                g_conv = root(g_x, g_y);
                b_conv = root(b_x, b_y);
            }
            else
            {
                r_conv = 0;
                g_conv = 0;
                b_conv = 0;
            }
            // store blurred pixels into dummmy array
            dummy[i][j].rgbtRed = r_conv;
            dummy[i][j].rgbtGreen = g_conv;
            dummy[i][j].rgbtBlue = b_conv;
        }
    }
    // convolute the pixels in the 4 corners
    // pixels in 4 corners are contained in 2x2 grid
    // upper left corner
    r_x = Gx[1][1] * image[0][0].rgbtRed + Gx[1][2] * image[0][1].rgbtRed
        + Gx[2][1] * image[1][0].rgbtRed + Gx[2][2] * image[1][1].rgbtRed;

    r_y = Gy[1][1] * image[0][0].rgbtRed + Gy[1][2] * image[0][1].rgbtRed
        + Gy[2][1] * image[1][0].rgbtRed + Gy[2][2] * image[1][1].rgbtRed;

    g_x = Gx[1][1] * image[0][0].rgbtGreen + Gx[1][2] * image[0][1].rgbtGreen
        + Gx[2][1] * image[1][0].rgbtGreen + Gx[2][2] * image[1][1].rgbtGreen;

    g_y = Gy[1][1] * image[0][0].rgbtGreen + Gy[1][2] * image[0][1].rgbtGreen
        + Gy[2][1] * image[1][0].rgbtGreen + Gy[2][2] * image[1][1].rgbtGreen;

    b_x = Gx[1][1] * image[0][0].rgbtBlue + Gx[1][2] * image[0][1].rgbtBlue
        + Gx[2][1] * image[1][0].rgbtBlue + Gx[2][2] * image[1][1].rgbtBlue;

    b_y = Gy[1][1] * image[0][0].rgbtBlue + Gy[1][2] * image[0][1].rgbtBlue
        + Gy[2][1] * image[1][0].rgbtBlue + Gy[2][2] * image[1][1].rgbtBlue;

    r_conv = root(r_x, r_y);
    g_conv = root(g_x, g_y);
    b_conv = root(b_x, b_y);

    dummy[0][0].rgbtRed = r_conv;
    dummy[0][0].rgbtGreen = g_conv;
    dummy[0][0].rgbtBlue = b_conv;

    // upper right corner
    r_x = Gx[1][0] * image[0][width - 2].rgbtRed + Gx[1][1] * image[0][width - 1].rgbtRed
        + Gx[2][0] * image[1][width - 2].rgbtRed + Gx[2][1] * image[1][width - 1].rgbtRed;

    r_y = Gy[1][0] * image[0][width - 2].rgbtRed + Gy[1][1] * image[0][width - 1].rgbtRed
        + Gy[2][0] * image[1][width - 2].rgbtRed + Gy[2][1] * image[1][width - 1].rgbtRed;

    g_x = Gx[1][0] * image[0][width - 2].rgbtGreen + Gx[1][1] * image[0][width - 1].rgbtGreen
        + Gx[2][0] * image[1][width - 2].rgbtGreen + Gx[2][1] * image[1][width - 1].rgbtGreen;

    g_y = Gy[1][0] * image[0][width - 2].rgbtGreen + Gy[1][1] * image[0][width - 1].rgbtGreen
        + Gy[2][0] * image[1][width - 2].rgbtGreen + Gy[2][1] * image[1][width - 1].rgbtGreen;

    b_x = Gx[1][0] * image[0][width - 2].rgbtBlue + Gx[1][1] * image[0][width - 1].rgbtBlue
        + Gx[2][0] * image[1][width - 2].rgbtBlue + Gx[2][1] * image[1][width - 1].rgbtBlue;

    b_y = Gy[1][0] * image[0][width - 2].rgbtBlue + Gy[1][1] * image[0][width - 1].rgbtBlue
        + Gy[2][0] * image[1][width - 2].rgbtBlue + Gy[2][1] * image[1][width - 1].rgbtBlue;

    r_conv = root(r_x, r_y);
    g_conv = root(g_x, g_y);
    b_conv = root(b_x, b_y);

    dummy[0][width - 1].rgbtRed = r_conv;
    dummy[0][width - 1].rgbtGreen = g_conv;
    dummy[0][width - 1].rgbtBlue = b_conv;

    // bottom left corner
    r_x = Gx[0][1] * image[height - 2][0].rgbtRed + Gx[0][2] * image[height - 2][1].rgbtRed
        + Gx[1][1] * image[height - 1][0].rgbtRed + Gx[1][2] * image[height - 1][1].rgbtRed;

    r_y = Gy[0][1] * image[height - 2][0].rgbtRed + Gy[0][2] * image[height - 2][1].rgbtRed
        + Gy[1][1] * image[height - 1][0].rgbtRed + Gy[1][2] * image[height - 1][1].rgbtRed;

    g_x = Gx[0][1] * image[height - 2][0].rgbtGreen + Gx[0][2] * image[height - 2][1].rgbtGreen
        + Gx[1][1] * image[height - 1][0].rgbtGreen + Gx[1][2] * image[height - 1][1].rgbtGreen;

    g_y = Gy[0][1] * image[height - 2][0].rgbtGreen + Gy[0][2] * image[height - 2][1].rgbtGreen
        + Gy[1][1] * image[height - 1][0].rgbtGreen + Gy[1][2] * image[height - 1][1].rgbtGreen;

    b_x = Gx[0][1] * image[height - 2][0].rgbtBlue + Gx[0][2] * image[height - 2][1].rgbtBlue
        + Gx[1][1] * image[height - 1][0].rgbtBlue + Gx[1][2] * image[height - 1][1].rgbtBlue;

    b_y = Gy[0][1] * image[height - 2][0].rgbtBlue + Gy[0][2] * image[height - 2][1].rgbtBlue
        + Gy[1][1] * image[height - 1][0].rgbtBlue + Gy[1][2] * image[height - 1][1].rgbtBlue;

    r_conv = root(r_x, r_y);
    g_conv = root(g_x, g_y);
    b_conv = root(b_x, b_y);

    dummy[height - 1][0].rgbtRed = r_conv;
    dummy[height - 1][0].rgbtGreen = g_conv;
    dummy[height - 1][0].rgbtBlue = b_conv;

    // bottom right corner
    r_x = Gx[0][0] * image[height - 2][width - 2].rgbtRed + Gx[0][1] * image[height - 2][width - 1].rgbtRed
        + Gx[1][0] * image[height - 1][width - 2].rgbtRed + Gx[1][1] * image[height - 1][width - 1].rgbtRed;

    r_y = Gy[0][0] * image[height - 2][width - 2].rgbtRed + Gy[0][1] * image[height - 2][width - 1].rgbtRed
        + Gy[1][0] * image[height - 1][width - 2].rgbtRed + Gy[1][1] * image[height - 1][width - 1].rgbtRed;

    g_x = Gx[0][0] * image[height - 2][width - 2].rgbtGreen + Gx[0][1] * image[height - 2][width - 1].rgbtGreen
        + Gx[1][0] * image[height - 1][width - 2].rgbtGreen + Gx[1][1] * image[height - 1][width - 1].rgbtGreen;

    g_y = Gy[0][0] * image[height - 2][width - 2].rgbtGreen + Gy[0][1] * image[height - 2][width - 1].rgbtGreen
        + Gy[1][0] * image[height - 1][width - 2].rgbtGreen + Gy[1][1] * image[height - 1][width - 1].rgbtGreen;

    b_x = Gx[0][0] * image[height - 2][width - 2].rgbtBlue + Gx[0][1] * image[height - 2][width - 1].rgbtBlue
        + Gx[1][0] * image[height - 1][width - 2].rgbtBlue + Gx[1][1] * image[height - 1][width - 1].rgbtBlue;

    b_y = Gy[0][0] * image[height - 2][width - 2].rgbtBlue + Gy[0][1] * image[height - 2][width - 1].rgbtBlue
        + Gy[1][0] * image[height - 1][width - 2].rgbtBlue + Gy[1][1] * image[height - 1][width - 1].rgbtBlue;

    r_conv = root(r_x, r_y);
    g_conv = root(g_x, g_y);
    b_conv = root(b_x, b_y);

    dummy[height - 1][width - 1].rgbtRed = r_conv;
    dummy[height - 1][width - 1].rgbtGreen = g_conv;
    dummy[height - 1][width - 1].rgbtBlue = b_conv;

    // update the original pxiel array with convoluted pixel values
    for (int m = 0; m < height; m++)
    {
        for (int n = 0; n < width; n++)
        {
            image[m][n].rgbtRed = dummy[m][n].rgbtRed;
            image[m][n].rgbtGreen = dummy[m][n].rgbtGreen;
            image[m][n].rgbtBlue = dummy[m][n].rgbtBlue;
        }
    }
    return;
}
