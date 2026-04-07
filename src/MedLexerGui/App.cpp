#include <wx/wx.h>
#include "MainFrame.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MainFrame* frame = new MainFrame("MedLexer - GUI");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
