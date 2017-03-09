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

#include "PINGUI.h"

std::vector<Window*> PINGUI::_ACTIVE_WINDOWS;

std::vector<Window*> PINGUI::_NON_ACTIVE_WINDOWS;

Window* PINGUI::_mainWindow = nullptr;

GUIManager* PINGUI::_mainGUIManager = nullptr;

void PINGUI::destroy(){

    for (std::size_t i = 0; i < _ACTIVE_WINDOWS.size(); i++){

        delete _ACTIVE_WINDOWS[i];
    }

    for (std::size_t i = 0; i < _NON_ACTIVE_WINDOWS.size(); i++){

        delete _NON_ACTIVE_WINDOWS[i];
    }

    delete _mainGUIManager;
}

void PINGUI::addWindow(Window* win){

    _ACTIVE_WINDOWS.push_back(win);

    if (!_mainWindow)
        _mainWindow = win;
}

void PINGUI::render(){

    PinGUI::Shader_Program::use();

    if (_mainGUIManager)
        _mainGUIManager->render();

    for (std::size_t i = 0; i < _ACTIVE_WINDOWS.size(); i++){

        if (_ACTIVE_WINDOWS[i]!=_mainWindow){

            _ACTIVE_WINDOWS[i]->render();
        }
    }

    if (_mainWindow && _mainWindow->getShow()){

        _mainWindow->render();
    }

    PinGUI::Shader_Program::unuse();
}

void PINGUI::update(){

    bool allowUpdate = true;

    checkActiveWindows();

    for (std::size_t i = _ACTIVE_WINDOWS.size(); i > 0; i--){

        _ACTIVE_WINDOWS[i-1]->update(allowUpdate);

        if (allowUpdate && collide(_ACTIVE_WINDOWS[i-1])){
            allowUpdate = false;
        }
    }

    if (_mainGUIManager)
        _mainGUIManager->update(allowUpdate);
}

void PINGUI::setMainWindow(Window* win){
    _mainWindow = win;
}

void PINGUI::checkActiveWindows(){

    for (std::size_t i = 0; i < _ACTIVE_WINDOWS.size(); i++)
        if (!_ACTIVE_WINDOWS[i]->getShow()){

            _NON_ACTIVE_WINDOWS.push_back(_ACTIVE_WINDOWS[i]);
            _ACTIVE_WINDOWS.erase(_ACTIVE_WINDOWS.begin()+i);

        }

    for (std::size_t i = 0; i < _NON_ACTIVE_WINDOWS.size(); i++)
    {

        if (_NON_ACTIVE_WINDOWS[i]->getShow()){

            _ACTIVE_WINDOWS.push_back(_NON_ACTIVE_WINDOWS[i]);
            _NON_ACTIVE_WINDOWS.erase(_NON_ACTIVE_WINDOWS.begin()+i);
        }
    }
}

int PINGUI::getSizeOfActiveWindows(){
    return _ACTIVE_WINDOWS.size();
}

bool PINGUI::collide(Window* win){

    if (GUI_CollisionManager::isColliding(GUI_Cursor::getCollider(),*(win->getCollider())))
        return true;
    else
        return false;
}

void PINGUI::checkMainWindow(){

    if (_mainWindow && !GUI_CollisionManager::isColliding(GUI_Cursor::getCollider(),*(_mainWindow->getCollider()))){

        for (std::size_t i = _ACTIVE_WINDOWS.size(); i >0; i--){

            if (GUI_CollisionManager::isColliding(GUI_Cursor::getCollider(),*(_ACTIVE_WINDOWS[i-1]->getCollider()))){

                _mainWindow = _ACTIVE_WINDOWS[i-1];

                _ACTIVE_WINDOWS[i-1] = _ACTIVE_WINDOWS.back();
                _ACTIVE_WINDOWS.back() = _mainWindow;

                return;
            }
        }
    }
}

PinGUI::basicPointer PINGUI::getFunctionPointer(){

    PinGUI::basicPointer f;
    f._function = boost::bind(&PINGUI::checkMainWindow);

    return f;
}

void PINGUI::initLibrary(int screenWidth, int screenHeight){

    _mainGUIManager = new GUIManager();

    initStorage();

    SheetManager::loadAllTextures();

    GUI_CollisionManager::colliding = false;

    PinGUI::Input_Manager::setScreenSize(screenWidth,screenHeight);

    GUI_ColorManager::initColors();

    PinGUI::CameraManager::init(screenWidth,screenHeight);

    PinGUI::Input_Manager::setClickFunction(PINGUI::getFunctionPointer());

    PinGUI::Shader_Program::initShaders();
}

void PINGUI::processInput(SDL_Event* event){

    PinGUI::Input_Manager::process(event);
}

GUIManager* PINGUI::getGUI(){

    return _mainGUIManager;
}

void PINGUI::initStorage(){

    _ACTIVE_WINDOWS.reserve(WINDOW_STORAGE_SIZE);
    _NON_ACTIVE_WINDOWS.reserve(WINDOW_STORAGE_SIZE);
}
