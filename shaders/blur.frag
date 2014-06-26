#version 330 core

in vec2 UV;
out vec4 colour;

uniform sampler2D texture;
uniform vec2 pixel_size;

void main(){
    colour =  vec4(0, 0, 0, 0);
    // colour = texture2D(texture, UV);

// 5*5 Gaussian Matrix:
/*
    0.002589,  0.0107788, 0.0241466, 0.0107788, 0.002589,
    0.0107788, 0.0448755, 0.10053,   0.0448755, 0.0107788,
    0.0241466, 0.10053,   0.225206,  0.10053,   0.0241466,
    0.0107788, 0.0448755, 0.10053,   0.0448755, 0.0107788,
    0.002589,  0.0107788, 0.0241466, 0.0107788, 0.002589
*/
/*
    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x, -2 * pixel_size.y)) * 0.002589;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x, -2 * pixel_size.y)) * 0.0107788;
    colour += texture2D(texture, UV + vec2(                0, -2 * pixel_size.y)) * 0.0241466;
    colour += texture2D(texture, UV + vec2(     pixel_size.x, -2 * pixel_size.y)) * 0.0107788;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x, -2 * pixel_size.y)) * 0.002589;

    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x,     -pixel_size.y)) * 0.0107788;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x,     -pixel_size.y)) * 0.0448755;
    colour += texture2D(texture, UV + vec2(                0,     -pixel_size.y)) * 0.10053;
    colour += texture2D(texture, UV + vec2(     pixel_size.x,     -pixel_size.y)) * 0.0448755;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x,     -pixel_size.y)) * 0.0107788;

    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x,                 0)) * 0.0241466;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x,                 0)) * 0.10053;
    colour += texture2D(texture, UV                                             ) * 0.225206;
    colour += texture2D(texture, UV + vec2(     pixel_size.x,                 0)) * 0.10053;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x,                 0)) * 0.0241466;

    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x,      pixel_size.y)) * 0.0107788;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x,      pixel_size.y)) * 0.0448755;
    colour += texture2D(texture, UV + vec2(                0,      pixel_size.y)) * 0.10053;
    colour += texture2D(texture, UV + vec2(     pixel_size.x,      pixel_size.y)) * 0.0448755;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x,      pixel_size.y)) * 0.0107788;

    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x,  2 * pixel_size.y)) * 0.002589;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x,  2 * pixel_size.y)) * 0.0107788;
    colour += texture2D(texture, UV + vec2(                0,  2 * pixel_size.y)) * 0.0241466;
    colour += texture2D(texture, UV + vec2(     pixel_size.x,  2 * pixel_size.y)) * 0.0107788;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x,  2 * pixel_size.y)) * 0.002589;
*/

// 7*7 Gaussian Matrix:
/*
0.00121055, 0.00354909, 0.00752005, 0.0102336, 0.00752005, 0.00354909, 0.00121055,
0.00354909, 0.0104052,  0.0220473,  0.0300028, 0.0220473,  0.0104052,  0.00354909,
0.00752005, 0.0220473,  0.0467152,  0.0635719, 0.0467152,  0.0220473,  0.00752005,
0.0102336,  0.0300028,  0.0635719,  0.0865112, 0.0635719,  0.0300028,  0.0102336,
0.00752005, 0.0220473,  0.0467152,  0.0635719, 0.0467152,  0.0220473,  0.00752005,
0.00354909, 0.0104052,  0.0220473,  0.0300028, 0.0220473,  0.0104052,  0.00354909,
0.00121055, 0.00354909, 0.00752005, 0.0102336, 0.00752005, 0.00354909, 0.00121055
*/

    colour += texture2D(texture, UV + vec2(-3 * pixel_size.x, -3 * pixel_size.y)) * 0.00121055;
    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x, -3 * pixel_size.y)) * 0.00354909;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x, -3 * pixel_size.y)) * 0.00752005;
    colour += texture2D(texture, UV + vec2(                0, -3 * pixel_size.y)) * 0.0102336;
    colour += texture2D(texture, UV + vec2(     pixel_size.x, -3 * pixel_size.y)) * 0.00752005;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x, -3 * pixel_size.y)) * 0.00354909;
    colour += texture2D(texture, UV + vec2( 3 * pixel_size.x, -3 * pixel_size.y)) * 0.00121055;

    colour += texture2D(texture, UV + vec2(-3 * pixel_size.x, -2 * pixel_size.y)) * 0.00354909;
    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x, -2 * pixel_size.y)) * 0.0104052;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x, -2 * pixel_size.y)) * 0.0220473;
    colour += texture2D(texture, UV + vec2(                0, -2 * pixel_size.y)) * 0.0300028;
    colour += texture2D(texture, UV + vec2(     pixel_size.x, -2 * pixel_size.y)) * 0.0220473;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x, -2 * pixel_size.y)) * 0.0104052;
    colour += texture2D(texture, UV + vec2( 3 * pixel_size.x, -2 * pixel_size.y)) * 0.00354909;

    colour += texture2D(texture, UV + vec2(-3 * pixel_size.x, -1 * pixel_size.y)) * 0.00752005;
    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x, -1 * pixel_size.y)) * 0.0220473;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x, -1 * pixel_size.y)) * 0.0467152;
    colour += texture2D(texture, UV + vec2(                0, -1 * pixel_size.y)) * 0.0635719;
    colour += texture2D(texture, UV + vec2(     pixel_size.x, -1 * pixel_size.y)) * 0.0467152;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x, -1 * pixel_size.y)) * 0.0220473;
    colour += texture2D(texture, UV + vec2( 3 * pixel_size.x, -1 * pixel_size.y)) * 0.00752005;

    colour += texture2D(texture, UV + vec2(-3 * pixel_size.x,                 0)) * 0.0102336;
    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x,                 0)) * 0.0300028;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x,                 0)) * 0.0635719;
    colour += texture2D(texture, UV + vec2(                0,                 0)) * 0.0865112;
    colour += texture2D(texture, UV + vec2(     pixel_size.x,                 0)) * 0.0635719;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x,                 0)) * 0.0300028;
    colour += texture2D(texture, UV + vec2( 3 * pixel_size.x,                 0)) * 0.0102336;

    colour += texture2D(texture, UV + vec2(-3 * pixel_size.x,  1 * pixel_size.y)) * 0.00752005;
    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x,  1 * pixel_size.y)) * 0.0220473;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x,  1 * pixel_size.y)) * 0.0467152;
    colour += texture2D(texture, UV + vec2(                0,  1 * pixel_size.y)) * 0.0635719;
    colour += texture2D(texture, UV + vec2(     pixel_size.x,  1 * pixel_size.y)) * 0.0467152;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x,  1 * pixel_size.y)) * 0.0220473;
    colour += texture2D(texture, UV + vec2( 3 * pixel_size.x,  1 * pixel_size.y)) * 0.00752005;

    colour += texture2D(texture, UV + vec2(-3 * pixel_size.x,  2 * pixel_size.y)) * 0.00354909;
    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x,  2 * pixel_size.y)) * 0.0104052;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x,  2 * pixel_size.y)) * 0.0220473;
    colour += texture2D(texture, UV + vec2(                0,  2 * pixel_size.y)) * 0.0300028;
    colour += texture2D(texture, UV + vec2(     pixel_size.x,  2 * pixel_size.y)) * 0.0220473;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x,  2 * pixel_size.y)) * 0.0104052;
    colour += texture2D(texture, UV + vec2( 3 * pixel_size.x,  2 * pixel_size.y)) * 0.00354909;

    colour += texture2D(texture, UV + vec2(-3 * pixel_size.x,  3 * pixel_size.y)) * 0.00121055;
    colour += texture2D(texture, UV + vec2(-2 * pixel_size.x,  3 * pixel_size.y)) * 0.00354909;
    colour += texture2D(texture, UV + vec2(    -pixel_size.x,  3 * pixel_size.y)) * 0.00752005;
    colour += texture2D(texture, UV + vec2(                0,  3 * pixel_size.y)) * 0.0102336;
    colour += texture2D(texture, UV + vec2(     pixel_size.x,  3 * pixel_size.y)) * 0.00752005;
    colour += texture2D(texture, UV + vec2( 2 * pixel_size.x,  3 * pixel_size.y)) * 0.00354909;
    colour += texture2D(texture, UV + vec2( 3 * pixel_size.x,  3 * pixel_size.y)) * 0.00121055;


// 9*9 Gaussian Matrix:
/*
0.000709454, 0.00167174, 0.00321706, 0.0048888, 0.00566146, 0.0048888, 0.00321706, 0.00167174, 0.000709454,
0.00167174,  0.00393924, 0.0075806,  0.0115198, 0.0133405,  0.0115198, 0.0075806,  0.00393924, 0.00167174,
0.00321706,  0.0075806,  0.014588,   0.0221686, 0.0256723,  0.0221686, 0.014588,   0.0075806,  0.00321706,
0.0048888,   0.0115198,  0.0221686,  0.0336884, 0.0390128,  0.0336884, 0.0221686,  0.0115198,  0.0048888,
0.00566146,  0.0133405,  0.0256723,  0.0390128, 0.0451786,  0.0390128, 0.0256723,  0.0133405,  0.00566146,
0.0048888,   0.0115198,  0.0221686,  0.0336884, 0.0390128,  0.0336884, 0.0221686,  0.0115198,  0.0048888,
0.00321706,  0.0075806,  0.014588,   0.0221686, 0.0256723,  0.0221686, 0.014588,   0.0075806,  0.00321706,
0.00167174,  0.00393924, 0.0075806,  0.0115198, 0.0133405,  0.0115198, 0.0075806,  0.00393924, 0.00167174,
0.000709454, 0.00167174, 0.00321706, 0.0048888, 0.00566146, 0.0048888, 0.00321706, 0.00167174, 0.000709454
*/
}