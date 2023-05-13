#include "MainWnd.h"
#include <project.h>
#include <thread>
#include <memory>
#include "../Common/Logger.h"
#include <filesystem>
#include "PlayList.h"

namespace MainWnd {
MainWnd::MainWnd() {
    set_default_size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    Logger::Log(title, Logger::MESSAGE);
    Logger::Log(std::string(), Logger::MESSAGE);

    auto builder = Gtk::Builder::create_from_file(DEFAULT_UI);
    builder->get_widget("boxMain", m_boxMain);
    builder->get_widget("scaleTime", m_scaleTime);
    builder->get_widget("scalePitch", m_scalePitch);
    builder->get_widget("scaleVolume", m_scaleVolume);
    builder->get_widget("menuOpen", m_menuOpen);
    builder->get_widget("menuQuit", m_menuQuit);
    builder->get_widget("buttonPlay", m_buttonPlay);
    builder->get_widget("treePlayList", m_treePlayList);

    m_bassMain = std::make_unique<Bass>();
    _initWidget();
}

void MainWnd::setAudio(const std::string file) {
    m_bassMain->play(file);
    m_buttonPlay->set_label("Playing");
    m_scaleTime->set_range(0, gdouble(m_bassMain->getLength()));
    set_title(m_bassMain->getTags("%TITL") + " (" + title + ")");
    Logger::Log(std::to_string(m_bassMain->volume()), Logger::MESSAGE);
}

void MainWnd::_initWidget() {
    add(*m_boxMain);
    _setTimeOut(500);
    // _setAudio("../sound.mp3"); // For Debug

    //////////
    /// Pitch scroll
    ///
    m_scalePitch->set_round_digits(1);
    m_scalePitch->set_digits(1);
    m_scalePitch->set_increments(0, 0.5);
    m_scalePitch->set_range(-12, 24);
    m_scalePitch->set_value(m_bassMain->pitch());
    // Pitch scroll changed
    m_scalePitch->signal_change_value().connect(
        [&](const Gtk::ScrollType scrollType, const double value) {
            m_bassMain->pitch(floor(m_scalePitch->get_value() * 10) / 10.0); // Increments of 0.1
            return true;
        }
    );

    //////////
    /// Volume scroll
    ///
    m_scaleVolume->set_round_digits(2);
    m_scaleVolume->set_digits(2);
    m_scaleVolume->set_increments(0.01, 0.01);
    m_scaleVolume->set_range(0, 1);
    m_scaleVolume->set_value(m_bassMain->volume());
    // Volume scroll changed
    m_scaleVolume->signal_change_value().connect(
        [&](const Gtk::ScrollType scrollType, const double value) {
            m_bassMain->volume(m_scaleVolume->get_value());
            return true;
        }
    );

    //////////
    /// Time scroll
    ///
    m_scaleTime->set_round_digits(0);
    m_scaleTime->set_digits(0);
    m_scaleTime->set_increments(0, 5);

    // Time scroll changed
    m_scaleTime->signal_change_value().connect(
        [&](const Gtk::ScrollType scrollType, const double value) {
            // m_bassMain->pause();
            if (m_bassMain->getLength() < m_scaleTime->get_value()) return true;
            Logger::Log("onTimeScaleChanged: " + std::to_string(m_scaleTime->get_value()), Logger::CALL);
            m_bassMain->position(m_scaleTime->get_value() + 1); 
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            return true;
        }
    );

    //////////
    /// Menu item
    ///
    m_menuOpen->signal_activate().connect(sigc::mem_fun(*this, &MainWnd::onMenuOpenClicked));
    m_menuQuit->signal_activate().connect([]() {Gtk::Main::quit();});

    //////////
    /// Button play and pause
    ///
    m_buttonPlay->set_label("Init");
    m_buttonPlay->signal_clicked().connect(
        [&]() {
            DWORD status = m_bassMain->getStatus();
            if (status == BASS_ACTIVE_PLAYING) {
                m_buttonPlay->set_label("Paused");
                m_bassMain->pause();
            }
            else if (status == BASS_ACTIVE_PAUSED) {
                m_buttonPlay->set_label("Playing");
                m_bassMain->play(std::string());
            }
        }
    );

    //////////
    /// ListBox playlist
    ///
    m_playList = std::make_unique<PlayList>(m_treePlayList);
    m_playList->setSelectionChanged(
        [&](const std::string filePath) {
            setAudio(filePath);
        }
    );
    m_playList->addFromFolder("/mnt/c/Users/mefor/Music"); 

    show_all_children();
}

void MainWnd::_setTimeOut(const int milliSeconds) {
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &MainWnd::updateTime), milliSeconds);
    // Logger::Log({"TimeOut updated: ", milliSeconds}, Logger::MESSAGE);
}

bool MainWnd::updateTime() {
    double playingTime = m_bassMain->position();
    Logger::Log("updateTime: " + std::to_string(playingTime), Logger::CALL);

    DWORD status = m_bassMain->getStatus();
    if (status != BASS_ACTIVE_PLAYING) return true;

    m_scaleTime->set_value(playingTime);
    return true;
}

void MainWnd::onMenuOpenClicked() {
    Logger::Log("onMenuOpenClicked", Logger::CALL);

    Gtk::FileChooserDialog dialog("Choose an audio", Gtk::FILE_CHOOSER_ACTION_OPEN);
    auto filter = Gtk::FileFilter::create();
    filter->add_mime_type("audio/mpeg");
    filter->set_name("MP3 files (*.mp3)");
    dialog.add_filter(filter);
    
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);
    switch (dialog.run()) {
        case Gtk::RESPONSE_OK: setAudio(dialog.get_filename()); break;
        case Gtk::RESPONSE_CANCEL: Logger::Log("Cancel", Logger::CALL); break;
        default: return;
    }
}
}