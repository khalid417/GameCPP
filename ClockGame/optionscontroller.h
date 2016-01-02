#ifndef OPTIONSCONTROLLER_H
#define OPTIONSCONTROLLER_H

#include <QQuickItem>

class OptionsController : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int volume READ volume WRITE setVolume)
    Q_PROPERTY(bool muted READ muted WRITE setMuted)
    Q_PROPERTY(int difficulty READ difficulty WRITE setDifficulty)
    Q_PROPERTY(int animationSpeed READ animationSpeed WRITE setAnimationSpeed)
    Q_PROPERTY(int mode READ mode WRITE setMode)
public:
    OptionsController(QQuickItem* parent=0):QQuickItem(parent){}
    int volume();
    bool muted();
    int difficulty();
    int animationSpeed();
    int mode();
    void setVolume(int volume);
    void setMuted(bool muted);
    void setDifficulty(int difficulty);
    void setAnimationSpeed(int animationSpeed);
    void setMode(int mode);
    Q_INVOKABLE int getCurrentGlobalSpeed();
    Q_INVOKABLE int getCurrentGlobalDifficulty();
    Q_INVOKABLE int getCurrentGlobalMuted();
    Q_INVOKABLE int getCurrentGlobalMode();
private:
    enum animationSpeeds{VerySlow = 0, Slow, Medium, Fast, VeryFast};
    enum volumeToggle{On = 0, Off};
    int m_volume;
    bool m_muted;
    int m_difficulty;
    int m_animationSpeed;
    int m_mode;
public slots:
    void animationSpeedClicked(int index);
    void difficultyClicked(int index);
    void volumeToggleClicked(int index);
    void volumeUpClicked();
    void volumeDownClicked();
    void modeClicked(int index);
};

#endif // OPTIONSCONTROLLER_H
