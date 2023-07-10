#include "RasterizerUtility.hpp"

// ------------------------------------------------------------------------
void RasterizerUtility::setDepthTestState(const bool isEnabled)
{
    if (isEnabled)
    {
        glEnable(GL_DEPTH_TEST);
        setDepthMethod(DepthMethods::DEPTH_LESS);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }

    LOG(INFO) << "Depth test set to " << isEnabled;
}

// ------------------------------------------------------------------------
void RasterizerUtility::setDepthMethod(const DepthMethods depthMethod)
{
    switch (depthMethod)
    {
        case DepthMethods::DEPTH_LESS:
            glDepthFunc(GL_LESS);
            break;
        case DepthMethods::DEPTH_LEQUAL:
            glDepthFunc(GL_LEQUAL);
            break;
        default:
            break;
    }

    LOG(INFO) << "Depth method set to " << static_cast<int>(depthMethod);
}

// ------------------------------------------------------------------------
void RasterizerUtility::setBlendState(const bool blendState)
{
    if (blendState)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LOG(INFO) << "Blend State set to " << blendState;
}

// ------------------------------------------------------------------------
void RasterizerUtility::setSupersampleState(const bool supersampleState)
{
    if (supersampleState)
        glEnable(GL_MULTISAMPLE);
    else
        glDisable(GL_MULTISAMPLE);

    LOG(INFO) << "Supersample state set to " <<  supersampleState;
}

// ------------------------------------------------------------------------
void RasterizerUtility::enableFaceCulling()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    LOG(INFO) << "Face culling enabled";
}

// ------------------------------------------------------------------------
void RasterizerUtility::setWireframeDrawingState(const bool drawWireframe)
{
    if (drawWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    LOG(INFO) << "Wireframe state set to " << drawWireframe;
}
