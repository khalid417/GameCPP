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
public:
    OptionsController(QQuickItem* parent=0):QQuickItem(parent){}
    int volume();
    bool muted();
    int difficulty();
    int animationSpeed();
    void setVolume(int volume);
    void setMuted(bool muted);
    void setDifficulty(int difficulty);
    void setAnimationSpeed(int animationSpeed);
private:
    enum animationSpeeds{VerySlow = 0, Slow, Medium, Fast, VeryFast};
    enum difficulties{Easy = 0, Regular, Hard};
    enum volumeToggle{On = 0, Off};
    int m_volume;
    bool m_muted;
    int m_difficulty;
    int m_animationSpeed;
public slots:
    void animationSpeedClicked(int index);
    void difficultyClicked(int index);
    void volumeToggleClicked(int index);
    void volumeUpClicked();
    void volumeDownClicked();
};

#endif // OPTIONSCONTROLLER_H
