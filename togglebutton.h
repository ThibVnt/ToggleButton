#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QAbstractButton>
#include <QSize>

class QLabel;
class QBrush;
class QPixmap;
class QPropertyAnimation;

class ToggleButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(int sliderCenterOffset MEMBER m_sliderCenterOffset WRITE setSliderCenterOffset);

public:
    ToggleButton(const QString &activatedTxt, const QString &deactivatedTxt, QWidget *parent = nullptr);
    ~ToggleButton();

    QSize minimumSizeHint() const noexcept override;
    QSize sizeHint() const noexcept override;

    bool isActivated() const noexcept;
    QString activatedText() const noexcept;
    QString deactivatedText() const noexcept;

    // ----  SETTERS ----
    void setActivatedText(const QString &txt) noexcept;
    void setDeactivatedText(const QString &txt) noexcept;

    void setActivatedBrush(const QBrush &br) noexcept;
    void setDeactivatedBrush(const QBrush &br) noexcept;
    void setActivatedColor(const QColor &col) noexcept;
    void setDeactivatedColor(const QColor &col) noexcept;


protected:
    void paintEvent(QPaintEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;


private:
    QSize m_drawingSize { QSize() };
    QBrush m_activatedBrush;
    QBrush m_deactivatedBrush;
    QBrush m_sliderBrush;

    int m_edgesMargin { 3 };
    int m_sliderRadius { 10 };
    int m_sliderCenterOffset { m_edgesMargin };

    bool m_activated { false };

    QLabel *m_leftLabel { nullptr };
    QLabel *m_rightLabel { nullptr };
    QPropertyAnimation *m_anim { nullptr };


    // ----  PRIVATE FUNCTIONS ----
    void updateLabelsPosition() const noexcept;
    void setSliderCenterOffset(int val) noexcept;

};
#endif // TOGGLEBUTTON_H
