#include <iostream>
#include <gtkmm.h>
#include "MainWnd/MainWnd.h"
#include <tags.h>

int main(int argc, char* argv[]) {
    // 一ヶ月以内に形にする (2023/4/28)
    Gtk::Main kit(argc, argv);

    /* BASS_Init(-1, 44100, 0, nullptr, nullptr);
    HSTREAM stream = BASS_StreamCreateFile(FALSE, "../sound.mp3", 0, 0, BASS_STREAM_DECODE);
    HSTREAM tempo = BASS_FX_TempoCreate(stream, BASS_STREAM_AUTOFREE);
    BASS_ChannelSetAttribute(tempo, BASS_ATTRIB_TEMPO_PITCH, 1.0f);
    BASS_ChannelPlay(tempo, FALSE);
    while (true) ;
    return 0; */

    MainWnd::MainWnd wnd;
    Gtk::Main::run(wnd);

    return 0;
}