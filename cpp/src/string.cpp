#include "str.h"

void String::draw () const
{
	driver->pixel(x,y,color);
}
