#ifndef POT_H
#define POT_H


#include <QGraphicsPixmapItem>

class Pot : public QGraphicsPixmapItem
{
public:
    Pot(int type,QPointF pos);
    virtual ~Pot(){};

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QRectF boundingRect() const override;

    void setPosition(QPointF pos);

    const QPointF &pos() const;

    void setNewPixmap(QString file){m_pixmap = file;}

    int getType();

private:
    QPointF m_pos;

    QString m_pixmap;

    int m_type=0;
};

#endif // POT_H
