#ifndef RESERVATABLEMANAGER_H
#define RESERVATABLEMANAGER_H

#include <QObject>
#include <QTableWidget>
#include <QVector>
#include "Reserva.h"

class ReservaTableManager : public QObject
{
    Q_OBJECT

public:
    explicit ReservaTableManager(QTableWidget *tableWidget, QObject *parent = nullptr);

    void setReservas(const QVector<Reserva>& reservas);

private:
    QTableWidget *tableWidget;
    void setupTable();
    void fillTable(const QVector<Reserva>& reservas);
};

#endif // RESERVATABLEMANAGER_H
