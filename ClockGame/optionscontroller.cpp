#include "optionscontroller.h"
#include <QtMultimedia/QMediaPlayer>

extern QMediaPlayer *player;
extern double globalDelay;
extern int globalDifficulty;
int OptionsController::getCurrentGlobalDifficulty()
{
    return globalDifficulty;
}
int OptionsController::getCurrentGlobalMuted()
{
    return (player->isMuted() ? 1 : 0);
}
int OptionsController::getCurrentGlobalSpeed()
{
    if (globalDelay > 2000) return 0;
    else if(globalDelay > 1000) return 1;
    else if(globalDelay > 500) return 2;
    else if(globalDelay > 200) return 3;
    else return 4;
}
void OptionsController::animationSpeedClicked(int index)
{
    m_animationSpeed = index;
    switch(m_animationSpeed)
    {
        case animationSpeeds::VerySlow:
            globalDelay = 3000;
            break;
        case animationSpeeds::Slow:
            globalDelay = 2000;
            break;
        case animationSpeeds::Medium:
            globalDelay = 1000;
            break;
        case animationSpeeds::Fast:
            globalDelay = 500;
            break;
        case animationSpeeds::VeryFast:
            globalDelay = 200;
            break;
        default:
            globalDelay = 1000;
    }

}
void OptionsController::difficultyClicked(int index)
{
    m_difficulty = index;
    globalDifficulty = m_difficulty;
}
void OptionsController::volumeDownClicked()
{
    m_volume = m_volume < 2 ? 0: m_volume - 2;
    player->setVolume(m_volume);
}
void OptionsController::volumeUpClicked()
{
    m_volume = m_volume > 98 ? 0: m_volume + 2;
    player->setVolume(m_volume);
}
void OptionsController::volumeToggleClicked(int index)
{
    if(index == volumeToggle::On)
        m_muted = false;
    else m_muted = true;
    player->setMuted(m_muted);
}

int OptionsController::volume()
{
    return m_volume;
}

bool OptionsController::muted()
{
    return m_muted;
}

int OptionsController::difficulty()
{
    return m_difficulty;
}

int OptionsController::animationSpeed()
{
    return m_animationSpeed;
}

void OptionsController::setVolume(int volume)
{
    m_volume = volume;
}

void OptionsController::setMuted(bool muted)
{
    m_muted = muted;
}

void OptionsController::setDifficulty(int difficulty)
{
    m_difficulty = difficulty;
}

void OptionsController::setAnimationSpeed(int animationSpeed)
{
    m_animationSpeed = animationSpeed;
}
