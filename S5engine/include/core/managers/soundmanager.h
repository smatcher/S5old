#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

class SoundEmitter;
class SoundManager : public Manager<SoundEmitter>
{
    friend class Singleton<SoundManager>;
protected:
    SoundManager();

public:
    virtual ~SoundManager();

};

typedef Singleton<SoundManager> SingletonSoundManager;
#define SOUND_MANAGER SingletonSoundManager::getInstance()

#endif // SOUNDMANAGER_H
