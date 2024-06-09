//
// Created by Scott Kirila on 5/15/2024.
//

#include "RenderingBase.h"

using namespace Rendering;

Base::Base(Window *window, const char *shaderVersion)
        : m_ShaderVersion(shaderVersion),
          m_FramebufferSize(window->m_ViewWidth, window->m_ViewHeight),
          m_Window(window) {}
