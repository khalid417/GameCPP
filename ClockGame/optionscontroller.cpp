#include "optionscontroller.h"

void OptionsController::animationSpeedClicked(int index){}
void OptionsController::difficultyClicked(int index){}
void OptionsController::volumeDownClicked(){}
void OptionsController::volumeUpClicked(){}
void OptionsController::volumeToggleClicked(int index){}

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
