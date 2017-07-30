#ifndef INPUT_DEVICE
#define INPUT_DEVICE

#include "device/eventtype/mouse.hpp"
#include "containers/rect.hpp"
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <map>
#include <vector>
#include <memory>
#include <functional>

namespace BlueBear {
  namespace Device {

    class Input {
      using LibraryKeyEnum = osgGA::GUIEventAdapter::KeySymbol;

    public:
      class InternalAdapter : public osgGA::GUIEventHandler {
        Input& input;

      public:
        InternalAdapter( Input& input );
        virtual bool handle(
          const osgGA::GUIEventAdapter& eventAdapter,
          osgGA::GUIActionAdapter& actionAdapter
        ) override;
      };
      using Adapter = osg::ref_ptr< InternalAdapter >;
      enum class Key : int {
        ANY = -1,

        ESCAPE = LibraryKeyEnum::KEY_Escape,
        F1 = LibraryKeyEnum::KEY_F1,
        F2 = LibraryKeyEnum::KEY_F2,
        F3 = LibraryKeyEnum::KEY_F3,
        F4 = LibraryKeyEnum::KEY_F4,
        F5 = LibraryKeyEnum::KEY_F5,
        F6 = LibraryKeyEnum::KEY_F6,
        F7 = LibraryKeyEnum::KEY_F7,
        F8 = LibraryKeyEnum::KEY_F8,
        F9 = LibraryKeyEnum::KEY_F9,
        F10 = LibraryKeyEnum::KEY_F10,
        F11 = LibraryKeyEnum::KEY_F11,
        F12 = LibraryKeyEnum::KEY_F12,

        TILDE = LibraryKeyEnum::KEY_Backquote,
        NUM1 = LibraryKeyEnum::KEY_1,
        NUM2 = LibraryKeyEnum::KEY_2,
        NUM3 = LibraryKeyEnum::KEY_3,
        NUM4 = LibraryKeyEnum::KEY_4,
        NUM5 = LibraryKeyEnum::KEY_5,
        NUM6 = LibraryKeyEnum::KEY_6,
        NUM7 = LibraryKeyEnum::KEY_7,
        NUM8 = LibraryKeyEnum::KEY_8,
        NUM9 = LibraryKeyEnum::KEY_9,
        NUM0 = LibraryKeyEnum::KEY_0,
        UNDERSCORE = LibraryKeyEnum::KEY_Underscore,
        EQUALS = LibraryKeyEnum::KEY_Equals,
        BACKSPACE = LibraryKeyEnum::KEY_BackSpace,


        TAB = LibraryKeyEnum::KEY_Tab,
        Q = LibraryKeyEnum::KEY_Q,
        W = LibraryKeyEnum::KEY_W,
        E = LibraryKeyEnum::KEY_E,
        R = LibraryKeyEnum::KEY_R,
        T = LibraryKeyEnum::KEY_T,
        Y = LibraryKeyEnum::KEY_Y,
        U = LibraryKeyEnum::KEY_U,
        I = LibraryKeyEnum::KEY_I,
        O = LibraryKeyEnum::KEY_O,
        P = LibraryKeyEnum::KEY_P,
        LEFT_BRACKET = LibraryKeyEnum::KEY_Leftbracket,
        RIGHT_BRACKET = LibraryKeyEnum::KEY_Rightbracket,
        BACKSLASH = LibraryKeyEnum::KEY_Backslash,

        CAPS_LOCK = LibraryKeyEnum::KEY_Caps_Lock,
        A = LibraryKeyEnum::KEY_A,
        S = LibraryKeyEnum::KEY_S,
        D = LibraryKeyEnum::KEY_D,
        F = LibraryKeyEnum::KEY_F,
        G = LibraryKeyEnum::KEY_G,
        H = LibraryKeyEnum::KEY_H,
        J = LibraryKeyEnum::KEY_J,
        K = LibraryKeyEnum::KEY_K,
        L = LibraryKeyEnum::KEY_L,
        SEMICOLON = LibraryKeyEnum::KEY_Semicolon,
        QUOTE = LibraryKeyEnum::KEY_Quote,
        ENTER = LibraryKeyEnum::KEY_Return,


        LEFT_SHIFT = LibraryKeyEnum::KEY_Shift_L,
        Z = LibraryKeyEnum::KEY_Z,
        X = LibraryKeyEnum::KEY_X,
        C = LibraryKeyEnum::KEY_C,
        V = LibraryKeyEnum::KEY_V,
        B = LibraryKeyEnum::KEY_B,
        N = LibraryKeyEnum::KEY_N,
        M = LibraryKeyEnum::KEY_M,
        COMMA = LibraryKeyEnum::KEY_Comma,
        PERIOD = LibraryKeyEnum::KEY_Period,
        SLASH = LibraryKeyEnum::KEY_Slash,
        RIGHT_SHIFT = LibraryKeyEnum::KEY_Shift_R,


        LEFT_CTRL = LibraryKeyEnum::KEY_Control_L,
        LEFT_META = LibraryKeyEnum::KEY_Meta_L,
        LEFT_ALT = LibraryKeyEnum::KEY_Alt_L,
        SPACE = LibraryKeyEnum::KEY_Space,
        RIGHT_ALT = LibraryKeyEnum::KEY_Alt_R,
        RIGHT_META = LibraryKeyEnum::KEY_Meta_R,
        RIGHT_MENU = LibraryKeyEnum::KEY_Menu,
        RIGHT_CTRL = LibraryKeyEnum::KEY_Control_R,

        PRINT_SCREEN = LibraryKeyEnum::KEY_Print,
        SCROLL_LOCK = LibraryKeyEnum::KEY_Scroll_Lock,
        PAUSE = LibraryKeyEnum::KEY_Pause,

        INSERT = LibraryKeyEnum::KEY_Insert,
        HOME = LibraryKeyEnum::KEY_Home,
        PAGE_UP = LibraryKeyEnum::KEY_Page_Up,

        DELETE = LibraryKeyEnum::KEY_Delete,
        END = LibraryKeyEnum::KEY_End,
        PAGE_DOWN = LibraryKeyEnum::KEY_Page_Down,

        UP = LibraryKeyEnum::KEY_Up,

        LEFT = LibraryKeyEnum::KEY_Left,
        DOWN = LibraryKeyEnum::KEY_Down,
        RIGHT = LibraryKeyEnum::KEY_Right,

        NUM_LOCK = LibraryKeyEnum::KEY_Num_Lock
      };

    private:
      Adapter inputAdapter;

    public:
      std::unique_ptr< EventType::BasicEvent > frameMouseDown;
      std::unique_ptr< EventType::BasicEvent > frameMouseUp;

      Input();

      Adapter getAdapter() const;
    };

  }
}


#endif
