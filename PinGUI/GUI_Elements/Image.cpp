/**

    PinGUI

    Copyright (c) 2017 Lubomir Barantal <l.pinsius@gmail.com>

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.

**/

#include "Image.h"

Image::Image(SDL_Surface* imageSurface , GUIPos x, GUIPos y)
{
    _position.x = x;
    _position.y = y;

    addSprite(x, y, imageSurface);

    _collidable = false;
}

Image::Image(SDL_Surface* imageSurface, GUIPos x, GUIPos y, int width, int height){

    _position.x = x;
    _position.y = y;

    addSprite(x, y, imageSurface);
    getSprite()->setRH(height);
    getSprite()->setRW(width);

    _collidable = false;
}

void Image::changeImage(SDL_Surface* imageSurface)
{
	deleteSprite(0);
	addSprite(_position.x, _position.y, imageSurface);
}

Image::~Image()
{
    //dtor
}

void Image::normalizeElement(const PinGUI::Vector2<GUIPos>& vect){

    //Have to store the oldPosition because the position will be modified in moveTo function
    PinGUI::Vector2<GUIPos> oldPosition(_position);

    moveTo(vect);

    //Flip Y
    oldPosition.y += getSprite()->getH();
    oldPosition.y *= -1;

    moveElement(oldPosition);
}
