//////////////////////////////////////////////////////////////////////
/// @file tsd/sds/android/app/audio/SyncAudioOut.cpp
/// @brief Implementation of class SyncAudioOut
/// @author Christian Loew (christian.loew@preh.de)
///
///
/// Copyright (c) Preh Car Connect GmbH
/// CONFIDENTIAL
//////////////////////////////////////////////////////////////////////

#include "tsd/sds/android/app/audio/SyncAudioOut.hpp"

#include <tsd/sds/android/common/NullCheck.hpp>

namespace tsd { namespace sds { namespace android { namespace app { namespace audio {

DEF_LOGGER(audio::SyncAudioOut);

SyncAudioOut::SyncAudioOut(const std::string &channelId,
                           tsd::common::types::SmartPtr<tsd::audio::audiorouter::client::ISession>& session)
   : IAudioOut()
   , m_ChannelConfiguration(tsd::audio::audiorouter::client::SampleFormat::s16le, 1, 22050)
   , m_Session(session)
   , m_Playback(nullptr)
   , m_ChannelId(channelId)
   , m_Started(false)
{
   ANDROID_THROW_IF_NULL(m_Session, SyncAudioOut);
   m_Playback = m_Session->requestPlayback(m_ChannelId,m_ChannelConfiguration, true).release();//Creates a synchronous audio sink in the Client's process which can be used to dispatch audio data for playback.
   m_Log.logInfo(__func__) << "created playback instance for audio out: " << m_ChannelId;
}


SyncAudioOut::~SyncAudioOut()
{
}


size_t SyncAudioOut::writeSamples(void *samples, size_t len)
{
   size_t result = 0;
   if (m_Playback)
   {
      result = len;
      m_Playback->write(samples, len/ sizeof(uint16_t));
   }
   return result;
}


void SyncAudioOut::finalize()
{
   if(nullptr != m_SourceControl.get())
   {
      m_Started = !m_SourceControl->changePlayStatus(rsiapi::PlayStatus_idle);
      m_Log.logInfo(__func__) << "Change to playstatus 'idle':" << m_Started;
   }
   else
   {
      m_Log.logInfo(__func__) << "No source control set. Setting to started = false";
      m_Started = false;
   }
}


void SyncAudioOut::start()
{
   {
      m_Log.logInfo(__func__);
   }
   if(nullptr != m_SourceControl.get())
   {
      m_Started = m_SourceControl->changePlayStatus(rsiapi::PlayStatus_playing);
      m_Log.logInfo(__func__) << "Change to playstatus 'playing':" << m_Started;
   }
   else
   {
      m_Log.logInfo(__func__) << "No source control set. Setting to started = true";
      m_Started = true;
   }
}


bool SyncAudioOut::isStarted() const
{
   return nullptr != m_Playback.get() && m_Started;
}


void SyncAudioOut::initialize(tsd::common::types::SmartPtr<rsiapi::ISourceControl> sourceControl)
{
   m_SourceControl = sourceControl;
   if(nullptr != m_SourceControl.get())
   {
      m_Started = !m_SourceControl->changePlayStatus(rsiapi::PlayStatus_idle);
      m_Log.logInfo(__func__) << "m_Started: " << (m_Started ? "true" : "false");
   }
}

void SyncAudioOut::initialize()
{
   if(nullptr != m_SourceControl.get())
   {
      m_Started = !m_SourceControl->changePlayStatus(rsiapi::PlayStatus_idle);
      m_Log.logInfo(__func__) << "m_Started: " << (m_Started ? "true" : "false");
   }
}


} // audio
} // app
} // android
} // sds
} // tsd

