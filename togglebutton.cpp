#include "togglebutton.h"

#include <QLabel>
#include <QPainter>
#include <QBrush>
#include <QRect>
#include <QPropertyAnimation>


#include <QMouseEvent>

ToggleButton::ToggleButton(const QString &activatedTxt, const QString &deactivatedTxt, QWidget *parent) : QAbstractButton(parent),
    m_leftLabel(new QLabel(activatedTxt,this)),
    m_rightLabel(new QLabel(deactivatedTxt,this)),
    m_anim(new QPropertyAnimation(this,"sliderCenterOffset",this))
{
    setMouseTracking(true);    

    m_activatedBrush = QBrush(QColor(10,200,10));   // Green
    m_deactivatedBrush = QBrush(QColor(240,0,32));  // Red
    m_sliderBrush = QBrush(Qt::white);

    QFont defaultBoltFont(this->font());
    defaultBoltFont.setBold(true);

    m_leftLabel->setFont(defaultBoltFont);
    m_rightLabel->setFont(defaultBoltFont);

    m_leftLabel->setAlignment(Qt::AlignCenter);
    m_rightLabel->setAlignment(Qt::AlignCenter);

    // Calling show on labels allow us to use their real width to calculate the proper ToggleButton SizeHint
    m_leftLabel->show();
    m_rightLabel->show();
}

ToggleButton::~ToggleButton()
{
}

QSize ToggleButton::minimumSizeHint() const noexcept {    
    const auto minimumWidthHint = qMax(m_leftLabel->width(), m_rightLabel->width()) + 2*m_sliderRadius + 3*m_edgesMargin;
    return QSize(minimumWidthHint, 2*(m_sliderRadius + m_edgesMargin));
}

QSize ToggleButton::sizeHint() const noexcept {
    const auto widthHint = 1.5*qMax(m_leftLabel->width(), m_rightLabel->width()) + 2*m_sliderRadius + 3*m_edgesMargin;
    return QSize(widthHint, 2*(m_sliderRadius + m_edgesMargin));
}


// Center both labels in the available space
void ToggleButton::updateLabelsPosition() const noexcept
{
    const auto leftLabelPosX = (m_drawingSize.width() - m_leftLabel->width() - m_edgesMargin)/2 - m_sliderRadius;
    const auto leftLabelPosY = (height() - m_leftLabel->height()) / 2;

    const auto rightLabelPosX = (m_drawingSize.width() + m_edgesMargin - m_rightLabel->width()) / 2 + m_sliderRadius;
    const auto rightLabelPosY = leftLabelPosY;

    m_leftLabel->move(leftLabelPosX, leftLabelPosY);
    m_rightLabel->move(rightLabelPosX, rightLabelPosY);
}


// ----  GETTERS ----

bool ToggleButton::isActivated() const noexcept
{
    return m_activated;
}

QString ToggleButton::activatedText() const noexcept
{
    return m_leftLabel->text();
}


QString ToggleButton::deactivatedText() const noexcept
{
    return m_rightLabel->text();
}


// ----  SETTERS ----
void ToggleButton::setActivatedText(const QString &txt) noexcept
{
    m_leftLabel->setText(txt);
    m_leftLabel->resize(m_leftLabel->sizeHint());
    update();
}

void ToggleButton::setDeactivatedText(const QString &txt) noexcept
{
    m_rightLabel->setText(txt);
    m_rightLabel->resize(m_rightLabel->sizeHint());
    update();
}


void ToggleButton::setActivatedBrush(const QBrush &br) noexcept
{
    m_activatedBrush = br;
    update();
}

void ToggleButton::setDeactivatedBrush(const QBrush &br) noexcept
{
    m_deactivatedBrush = br;
    update();
}

void ToggleButton::setActivatedColor(const QColor &col) noexcept
{
    m_activatedBrush.setColor(col);
    update();
}


void ToggleButton::setDeactivatedColor(const QColor &col) noexcept
{
    m_deactivatedBrush.setColor(col);
    update();
}

// This private function with the update is necessary for the good functionning of
// the QPropertyAnimation on the slider
void ToggleButton::setSliderCenterOffset(int val) noexcept {
    m_sliderCenterOffset = val;
    update();
}


///////////     EVENTS DEFINITION     ///////////

void ToggleButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    // If the ToggleButton is activated, left label is visible and right label is hidden.
    m_leftLabel->setVisible(isActivated());
    m_rightLabel->setVisible(!isActivated());

    updateLabelsPosition();

    // Update slider size
    m_sliderRadius = m_drawingSize.height()/2 - m_edgesMargin;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);

    if(isEnabled()) {
        // Draw the body and set its color
        isActivated() ? p.setBrush(m_activatedBrush) : p.setBrush(m_deactivatedBrush);
        p.drawRoundedRect(QRect(0,0, m_drawingSize.width(),m_drawingSize.height()),
                          m_sliderRadius + m_edgesMargin, m_sliderRadius + m_edgesMargin);

        // Draw the slider and set its color
        p.setBrush(m_sliderBrush);
        p.drawEllipse(m_sliderCenterOffset,
                      m_edgesMargin,
                      2*m_sliderRadius,
                      2*m_sliderRadius);
    }
    else {
        p.setBrush(QColor(189,189,189));    // Grey
        p.drawRoundedRect(QRect(0,0, m_drawingSize.width(),m_drawingSize.height()),
                          m_sliderRadius + m_edgesMargin, m_sliderRadius + m_edgesMargin);

        p.setBrush(QColor(110,110,110));    // Darker Grey
        p.drawEllipse(m_sliderCenterOffset,
                      m_edgesMargin,
                      2*m_sliderRadius,
                      2*m_sliderRadius);
    }
}

// Change the cursor when it's on the widget
void ToggleButton::mouseMoveEvent(QMouseEvent *e)
{
    const auto relativePos = e->localPos();

    if(relativePos.x() <= m_drawingSize.width() && relativePos.y() <= m_drawingSize.height()) {
        setCursor(Qt::PointingHandCursor);
        QAbstractButton::mouseMoveEvent(e);
    }
    else
        setCursor(Qt::ArrowCursor);
}

// Run the slider animation on leftClick release and emit signal for button state changed
void ToggleButton::mouseReleaseEvent(QMouseEvent *e)
{    
    if(e->button() == Qt::LeftButton) {
        const auto relativePos = e->localPos();

        if(relativePos.x() <= m_drawingSize.width() && relativePos.y() <= m_drawingSize.height()) {
            m_activated = !m_activated;

            if(m_activated) {
                m_anim->setStartValue(m_edgesMargin);
                m_anim->setEndValue(m_drawingSize.width() - 2*m_sliderRadius - m_edgesMargin);
                m_anim->setDuration(100);
                m_anim->start();
            }
            else {
                m_anim->setStartValue(m_drawingSize.width() - m_sliderRadius - m_edgesMargin);
                m_anim->setEndValue(m_edgesMargin);
                m_anim->setDuration(100);
                m_anim->start();
            }
            emit toggled(m_activated);
            QAbstractButton::mouseReleaseEvent(e);
        }
    }
}

// Allow the slider to keep it's correct activated position in case of resizing the widget
void ToggleButton::resizeEvent(QResizeEvent *e) {
    const auto newSize = e->size();

    m_drawingSize.setWidth(newSize.width());
    m_drawingSize.setHeight(newSize.height());

    if(isActivated())
        setSliderCenterOffset(m_drawingSize.width() - 2*m_sliderRadius - m_edgesMargin);

}
