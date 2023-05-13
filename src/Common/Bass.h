#ifndef COMMON_BASS_H
#define COMMON_BASS_H

#include <gtkmm.h>
#include <iostream>
#include <bass.h>
#include <tags.h>
#include <bass_fx.h>
#include <bassmix.h>

class Bass : public Gtk::Window {
    public:
        Bass();
        ~Bass();
        bool play(const std::string file);
        bool pause();
        
        double position();
        void position(const double pos);
        
        float pitch();
        void pitch(const float value);
        
        float volume();
        void volume(const float vol);

        double getLength();
        DWORD getStatus();
        std::string getTags(const std::string fmt);
    private:
        float m_pitch;
        float m_volume;
        HSTREAM m_stream;
        HSTREAM m_tempostream;
        HSTREAM m_mixstream;
};

#endif // COMMON_BASS_H