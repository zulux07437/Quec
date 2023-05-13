#ifndef MAINWND_H
#define MAINWND_H

#include <gtkmm.h>
#include <project.h>
#include <string>
#include "../Common/Bass.h"
#include "PlayList.h"

namespace MainWnd {
class MainWnd : public Gtk::Window {
    public:
        MainWnd();
        bool updateTime();
        void onMenuOpenClicked();
        void setAudio(const std::string file);
    private:
        void _initWidget();
        void _setTimeOut(const int milliSeconds);

        const int DEFAULT_WIDTH = 800;
        const int DEFAULT_HEIGHT = 600;
        const std::string DEFAULT_UI = "MainWnd.glade";
        const std::string title = std::string(PROJECT_NAME) + " v" + std::string(PROJECT_VERSION);

        std::unique_ptr<Bass> m_bassMain;
        std::unique_ptr<PlayList> m_playList;
        /*
         * UI
         */
        Gtk::Box* m_boxMain;
        Gtk::Scale* m_scaleTime;
        Gtk::Scale* m_scaleVolume;
        Gtk::MenuItem* m_menuOpen;
        Gtk::MenuItem* m_menuQuit;
        Gtk::Scale* m_scalePitch;
        Gtk::Button* m_buttonPlay;
        Gtk::TreeView* m_treePlayList;
};
}

#endif // MAINWND_H