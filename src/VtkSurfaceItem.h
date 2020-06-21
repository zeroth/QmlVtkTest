#ifndef VTKSURFACEITEM_H
#define VTKSURFACEITEM_H

#include <QString>
#include <QQuickFramebufferObject>

class VtkSurfaceItem : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    VtkSurfaceItem();
    ~VtkSurfaceItem();

    // QQuickFramebufferObject interface
public:
    Renderer *createRenderer() const;
    QList<QEvent*> pendingEvents() {
        QList<QEvent*> ret = m_pendingEvents;
        m_pendingEvents.clear();
        return ret;
    }
    // QQuickItem interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    const QPoint tx(const QPoint &p) const { return QPoint(p.x(), -1*p.y()); }
    const QPointF tx(const QPointF &p) const { return QPointF(p.x(), -1*p.y()); }

private:
    QList<QEvent*> m_pendingEvents;
};

#endif // VTKSURFACEITEM_H
