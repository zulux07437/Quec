#include "Bass.h"
#include "Logger.h"
#include <thread>

Bass::Bass() {
    int result = BASS_Init(-1, 44100, 0, nullptr, nullptr);
    // int result = BASS_Init(-1, 0, 0, nullptr, nullptr);
    if (!result) {
        Logger::Log("BASS_Init failed", Logger::FAIL);
        throw result;
    }
    Logger::Log("BASS_Init successful", Logger::SUCCESS);
    m_mixstream = BASS_Mixer_StreamCreate(44100, 2, 0);
    pitch(0.0f);
    volume(0.5f);
}

Bass::~Bass() {
    int result = BASS_Free();
    if (!result) {
        Logger::Log("BASS_Free failed", Logger::FAIL);
        return;
    }
    Logger::Log("BASS_Free successful", Logger::SUCCESS);
}

bool Bass::play(const std::string file) {
    // Stream refresh
    if (!file.empty()) {
        if (m_stream) {
            BASS_Mixer_ChannelRemove(m_stream);
            BASS_StreamFree(m_stream);
            BASS_StreamFree(m_tempostream);
        }

        m_stream = BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, BASS_STREAM_DECODE);
        m_tempostream = BASS_FX_TempoCreate(m_stream, BASS_STREAM_AUTOFREE);
        // m_tempostream = BASS_FX_TempoCreate(m_stream, BASS_STREAM_DECODE);

        // BASS_Mixer_StreamAddChannel(m_mixstream, m_tempostream, BASS_MIXER_DOWNMIX);
        pitch(pitch());
        Logger::Log("Stream Refreshed", Logger::SUCCESS);
    }
    BASS_ChannelPlay(m_tempostream, FALSE);
    return true;
}

bool Bass::pause() {
    BASS_ChannelPause(m_tempostream);
    return true;
}

double Bass::position() {
    return BASS_ChannelBytes2Seconds(m_tempostream, BASS_ChannelGetPosition(m_tempostream, BASS_POS_BYTE));
}

void Bass::position(const double pos) {
    BASS_ChannelSetPosition(m_tempostream, BASS_ChannelSeconds2Bytes(m_tempostream, pos), BASS_POS_BYTE);
}

float Bass::pitch() {
    return m_pitch;
}

void Bass::pitch(const float value) {
    m_pitch = value;
    BASS_ChannelSetAttribute(m_tempostream, BASS_ATTRIB_TEMPO_PITCH, m_pitch);
}

float Bass::volume() {
    return m_volume;
}

void Bass::volume(const float value) {
    m_volume = value;
    BASS_SetVolume(m_volume);
}




std::string Bass::getTags(const std::string fmt) {
    if (fmt.empty()) return std::string();
    return TAGS_Read(m_stream, fmt.c_str());
}

DWORD Bass::getStatus() {
    return BASS_ChannelIsActive(m_tempostream);
}

double Bass::getLength() {
    return BASS_ChannelBytes2Seconds(m_tempostream, BASS_ChannelGetLength(m_tempostream, BASS_POS_BYTE));
}