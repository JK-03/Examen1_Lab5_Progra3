#ifndef GUARDARRESERVAS_H
#define GUARDARRESERVAS_H

#include <QString>
#include <QVector>
#include "Reserva.h"

class GuardarReservas
{
public:
    static bool guardar(const QVector<Reserva>& reservas, const QString& nombreArchivo);
    static QVector<Reserva> cargar(const QString& nombreArchivo);
};

#endif // GUARDARRESERVAS_H
