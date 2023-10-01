#ifndef JSONHELPERFUNCTIONS_H
#define JSONHELPERFUNCTIONS_H
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

void modifyJsonValue(QJsonValue& destValue, const QString& path, const QJsonValue& newValue);

void modifyJsonValue(QJsonDocument& doc, const QString& path, const QJsonValue& newValue);

#endif // JSONHELPERFUNCTIONS_H
