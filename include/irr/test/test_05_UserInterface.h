/** Example 005 User Interface
This tutorial shows how to use the built in User Interface of
the Irrlicht Engine. It will give a brief overview and show
how to create and use windows, buttons, scroll bars, static
texts, and list boxes.
As always, we include the header files, and use the irrlicht
namespaces. We also store a pointer to the Irrlicht device,
a counter variable for changing the creation position of a window,
and a pointer to a listbox.
*/
//#include "driverChoice.h"
#ifdef _IRR_WINDOWS_
//#pragma comment(lib, "Irrlicht.lib")
#endif
// Declare a structure to hold some context for the event receiver so that it
// has it available inside its OnEvent() method.
struct SAppContext {
  IrrlichtDevice* device;
  s32       counter;
  IGUIListBox*  listbox;
};
// Define some values that we'll use to identify individual GUI controls.
enum {
  GUI_ID_QUIT_BUTTON = 101,
  GUI_ID_NEW_WINDOW_BUTTON,
  GUI_ID_FILE_OPEN_BUTTON,
  GUI_ID_TRANSPARENCY_SCROLL_BAR
};
/*
  Set the skin transparency by changing the alpha values of all skin-colors
*/
void setSkinTransparency(s32 alpha, IGUISkin* skin)
{
  for (s32 i = 0; i < EGDC_COUNT ; ++i) {
    SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
    col.setAlpha(alpha);
    skin->setColor((EGUI_DEFAULT_COLOR)i, col);
  }
}
/*
The Event Receiver is not only capable of getting keyboard and
mouse input events, but also events of the graphical user interface
(gui). There are events for almost everything: Button click,
Listbox selection change, events that say that a element was hovered
and so on. To be able to react to some of these events, we create
an event receiver.
We only react to gui events, and if it's such an event, we get the
id of the caller (the gui element which caused the event) and get
the pointer to the gui environment.
*/
class MyEventReceiver : public IEventReceiver
{
public:
  MyEventReceiver(SAppContext& context) : Context(context) { }
  bool OnEvent(const SEvent& event) {
    if (event.EventType == EET_GUI_EVENT) {
      s32 id = event.GUIEvent.Caller->getID();
      IGUIEnvironment* env = Context.device->getGUIEnvironment();
      switch (event.GUIEvent.EventType) {
        /*
        If a scrollbar changed its scroll position, and it is
        'our' scrollbar (the one with id GUI_ID_TRANSPARENCY_SCROLL_BAR), then we change
        the transparency of all gui elements. This is a very
        easy task: There is a skin object, in which all color
        settings are stored. We simply go through all colors
        stored in the skin and change their alpha value.
        */
      case EGET_SCROLL_BAR_CHANGED:
        if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR) {
          s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
          setSkinTransparency(pos, env->getSkin());
        }
        break;
        /*
        If a button was clicked, it could be one of 'our'
        three buttons. If it is the first, we shut down the engine.
        If it is the second, we create a little window with some
        text on it. We also add a string to the list box to log
        what happened. And if it is the third button, we create
        a file open dialog, and add also this as string to the list box.
        That's all for the event receiver.
        */
      case EGET_BUTTON_CLICKED:
        switch (id) {
        case GUI_ID_QUIT_BUTTON:
          Context.device->closeDevice();
          return true;
        case GUI_ID_NEW_WINDOW_BUTTON: {
          Context.listbox->addItem("Window created");
          Context.counter += 30;
          if (Context.counter > 200) {
            Context.counter = 0;
          }
          IGUIWindow* window = env->addWindow(
              IRECT(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter),
              false, // modal?
              "Test window");
          env->addStaticText("Please close me",
              IRECT(35, 35, 140, 50),
              true, // border?
              false, // wordwrap?
              window);
        }
        return true;
        case GUI_ID_FILE_OPEN_BUTTON:
          Context.listbox->addItem("File open");
          // There are some options for the file open dialog
          // We set the title, make it a modal window, and make sure
          // that the working directory is restored after the dialog
          // is finished.
          env->addFileOpenDialog("Please choose a file.", true, 0, -1, true);
          return true;
        default:
          return false;
        }
        break;
      case EGET_FILE_SELECTED: {
        // show the model filename, selected in the file dialog
        IGUIFileOpenDialog* dialog =
            (IGUIFileOpenDialog*)event.GUIEvent.Caller;
        Context.listbox->addItem(dialog->getFileName());
      }
      break;
      default:
        break;
      }
    }
    return false;
  }
private:
  SAppContext& Context;
};
/*
Ok, now for the more interesting part. First, create the Irrlicht device. As in
some examples before, we ask the user which driver he wants to use for this
example:
*/
int main()
{
  // ask user for driver
  E_DRIVER_TYPE driverType = driverChoiceConsole();
  if (driverType == EDT_COUNT) {
    return 1;
  }
  // create device and exit if creation failed
  IrrlichtDevice* device = createDevice(driverType, dimension2du(640, 480));
  if (device == 0) {
    return 1;  // could not create selected driver.
  }
  /* The creation was successful, now we set the event receiver and
    store pointers to the driver and to the gui environment. */
  device->setWindowCaption("Irrlicht Engine - User Interface Demo");
  device->setResizable(true);
  IVideoDriver* driver = device->getVideoDriver();
  IGUIEnvironment* env = device->getGUIEnvironment();
  /*
  To make the font a little bit nicer, we load an external font
  and set it as the new default font in the skin.
  To keep the standard font for tool tip text, we set it to
  the built-in font.
  */
  IGUISkin* skin = env->getSkin();
  IGUIFont* font = env->getFont("../../media/fonthaettenschweiler.bmp");
  if (font) {
    skin->setFont(font);
  }
  skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);
  /*
  We add three buttons. The first one closes the engine. The second
  creates a window and the third opens a file open dialog. The third
  parameter is the id of the button, with which we can easily identify
  the button in the event receiver.
  */
  env->addButton(IRECT(10, 240, 110, 240 + 32), 0, GUI_ID_QUIT_BUTTON,
      "Quit", "Exits Program");
  env->addButton(IRECT(10, 280, 110, 280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON,
      "New Window", "Launches a new Window");
  env->addButton(IRECT(10, 320, 110, 320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON,
      "File Open", "Opens a file");
  /*
  Now, we add a static text and a scrollbar, which modifies the
  transparency of all gui elements. We set the maximum value of
  the scrollbar to 255, because that's the maximal value for
  a color value.
  Then we create an other static text and a list box.
  */
  env->addStaticText("Transparent Control:", IRECT(150, 20, 350, 40), true);
  IGUIScrollBar* scrollbar = env->addScrollBar(true,
      IRECT(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
  scrollbar->setMax(255);
  scrollbar->setPos(255);
  setSkinTransparency(scrollbar->getPos(), env->getSkin());
  // set scrollbar position to alpha value of an arbitrary element
  scrollbar->setPos(env->getSkin()->getColor(EGDC_WINDOW).getAlpha());
  env->addStaticText("Logging ListBox:", IRECT(50, 110, 250, 130), true);
  IGUIListBox* listbox = env->addListBox(IRECT(50, 140, 250, 210));
  env->addEditBox("Editable Text", IRECT(350, 80, 550, 100));
  // Store the appropriate data in a context structure.
  SAppContext context;
  context.device = device;
  context.counter = 0;
  context.listbox = listbox;
  // Then create the event receiver, giving it that context structure.
  MyEventReceiver receiver(context);
  // And tell the device to use our custom event receiver.
  device->setEventReceiver(&receiver);
  /*
  And at last, we create a nice Irrlicht Engine logo in the top left corner.
  */
  env->addImage(driver->getTexture("../../media/irrlichtlogo2.png"),
      position2di(10, 10));
  /*
  That's all, we only have to draw everything.
  */
  while (device->run() && driver)
    if (device->isWindowActive()) {
      driver->beginScene(true, true, _ARGB(0, 200, 200, 200));
      env->drawAll();
      driver->endScene();
    }
  device->drop();
  return 0;
}
/*
**/

