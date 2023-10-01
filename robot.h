#ifndef ROBOT_H
#define ROBOT_H

#include <QGraphicsPixmapItem>
#include "shared_types.hpp"

struct position
{
    QPointF coord;
    double theta;
};

class Robot : public QGraphicsPixmapItem
{
public:
    Robot();
    virtual ~Robot(){};

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QRectF boundingRect() const override;

    void setPosition(position pos);

    const position &pos() const;

    void setNewPixmap(QString file){m_pixmap = file;}

private:
    position m_pos;

    QString m_pixmap;
};

#endif // ROBOT_H
