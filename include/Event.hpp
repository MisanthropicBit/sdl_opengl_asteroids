#ifndef EVENT_HPP
#define EVENT_HPP

#include "SDL.h"

class Event {
   public:
        Event();

        virtual void OnEvent(SDL_Event& event);
        virtual void OnInputFocus();
        virtual void OnInputBlur();
        virtual void OnKeyDown(SDLKey key, SDLMod modifier, Uint16 unicode);
        virtual void OnKeyUp(SDLKey key, SDLMod modifier, Uint16 unicode);
        virtual void OnMouseFocus();
        virtual void OnMouseBlur();
        virtual void OnMouseMove(int mx, int my, int relx, int rely, Uint8 state);
        virtual void OnLeftMouseButtonDown(int mx, int my);
        virtual void OnLeftMouseButtonUp(int mx, int my);
        virtual void OnRightMouseButtonDown(int mx, int my);
        virtual void OnRightMouseButtonUp(int mx, int my);
        virtual void OnMiddleMouseButtonDown(int mx, int my);
        virtual void OnMiddleMouseButtonUp(int mx, int my);
        virtual void OnMinimize();
        virtual void OnRestore();
        virtual void OnResize(int w,int h);
        virtual void OnExpose();
        virtual void OnExit();
};

#endif