#pragma once

#include <QObject>

class ui  : public QObject
{
    Q_OBJECT

public:
    ui(QObject *parent);
    ~ui();
};
