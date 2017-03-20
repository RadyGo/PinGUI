#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

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

#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <memory>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../TextManager/Text.h"
#include "../TextManager/IntegerText.h"
#include "../TextManager/FloatText.h"
#include "../TextManager/StringText.h"
#include "../VBO_Manager.h"
#include "../PinGUI_Vector.hpp"
#include "../CropManager.hpp"
#include "../clFunction.hpp"
#include "../CameraManager.h"

/**
    Main class
**/
class TextManager
{
    private:

        //Storage variables
        std::vector<std::shared_ptr<Text>> _TEXTS;

        std::vector<vboData> _mainDataStorage;

        std::shared_ptr<VBO_Manager> _textVBOManager;

        //Font stuff
        int _fontSize;

        textInfo _mainTextInfo;

        bool _needUpdate;

        bool _moved;

        //Function to be called everytime when i update text ( useful for dynamic cropping )
        PinGUI::basicPointer _function;

        //Font stuff
        void loadTextInfo();

        //Functions for adding new text
        void checkForUpdate();

        void update();

        void createText();

        void replaceText(int& position);

        void removeData(std::size_t& position);

        /**
            Manipulation with VBOs etc.
        **/
        void loadVBO();
        void updateVBOData();

    public:
        TextManager();
        TextManager(const TextManager& txt);
        ~TextManager();

        //I have 2 cases, so i need to overload these functions
        std::shared_ptr<Text> writeText(const std::string& text,int x, int y);
        std::shared_ptr<Text> writeText(int x, int y, int* var);
        std::shared_ptr<Text> writeText(int x, int y, float* var);
        std::shared_ptr<Text> writeText(int x, int y, std::string* var);

        TTF_Font* loadFont(const std::string& fontPath, int size);

        void setColor(GLubyte R, GLubyte G, GLubyte B, GLubyte A);

        void setFontSize(int size);

        void setUpdate(bool state);

        void renderText();

        void updateText();

        void cropText(PinGUI::Rect& cropRect);

        void normalizeText(const PinGUI::Vector2<GUIPos>& normalizationVector);

        std::vector<std::shared_ptr<Text>>* getMainTextStorage();

        int getFontSize();

        std::shared_ptr<Text> getLastText();

        void moveTextManager(const PinGUI::Vector2<GUIPos>& vect, bool croppedMovement);

        textInfo* getTextInfo();

        void setFunction(PinGUI::basicPointer f);
};

#endif // TEXTMANAGER_H
