#include "getDegrees.hpp"

float getDegrees(float i_degrees)
{
    return (fmod(360 + fmod(i_degrees, 360), 360));;
}
