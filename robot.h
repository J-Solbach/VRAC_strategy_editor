#ifndef ROBOT_H
#define ROBOT_H

#include <QGraphicsPixmapItem>

struct position
{
    QPointF coord;
    double theta;
};
enum mode{Close,Open};
enum type_etat{debut,close,frontOpen,backOpen,open,fin};

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

    void setEtat(enum mode frontStock, enum mode backStock, bool reset=0);

    mode frontStock();

    mode backStock();

private:
    position m_pos;

    QString m_pixmap;

    enum mode m_frontStock=Close;

    enum mode m_backStock=Close;
};

#endif // ROBOT_H
