#pragma once

#include <QString>
#include <QVector>

struct Package {
    int id;
    QString name;
    QVector<int> themeIds;
};
