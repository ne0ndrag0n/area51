#ifndef WIDGET_ENGINE_ADAPTER
#define WIDGET_ENGINE_ADAPTER

#include "graphics/gui/widget/widgetengine.hpp"
#include <osg/Drawable>
#include <osg/RenderInfo>
#include <osg/State>
#include <nanovg/nanovg.h>

namespace BlueBear {
  namespace Graphics {
    namespace GUI {
      namespace Widget {

        class WidgetEngine::Adapter : public osg::Drawable {
          WidgetEngine& parent;
          NVGcontext* nvgContext = nullptr;
          unsigned int activeContextID;

          void loadFonts();

        public:
          Adapter( WidgetEngine& parent );

          virtual void drawImplementation( osg::RenderInfo& renderInfo ) const override;
          virtual void releaseGLObjects( osg::State* state = 0 ) const override;
        };

      }
    }
  }
}


#endif
