#ifndef PANNEAU_H
#define PANNEAU_H


#include <QGraphicsPixmapItem>

class Panneau : public QGraphicsPixmapItem
{
public:
    Panneau(QPointF pos);
    virtual ~Panneau(){};

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QRectF boundingRect() const override;

    void setTheta(int theta);

    int theta() const;

    void setNewPixmap(QString file){m_pixmap = file;}

private:
    QPointF m_pos;

    QString m_pixmap;

    int m_theta=0;
};

#endif // PANNEAU_H
