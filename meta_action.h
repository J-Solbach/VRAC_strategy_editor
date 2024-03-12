#ifndef META_ACTION_H
#define META_ACTION_H

#include "toolboxscene.h"
#include <QJsonObject>
#include <QJsonDocument>

class meta_action
{
public:
    meta_action(QString fileName);

    void setMetaActionScene(ToolBoxScene *stratBuilder);

private:
    QString m_fileName;
};

#endif // META_ACTION_H
