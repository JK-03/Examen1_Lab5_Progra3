#include "GuardarReservas.h"
#include <QFile>
#include <QTextStream>

bool GuardarReservas::guardar(const QVector<Reserva>& reservas, const QString& nombreArchivo)
{
    QFile file(nombreArchivo);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    for (const auto& reserva : reservas) {
        out << reserva.getCodigo() << ','
            << reserva.getNombre() << ','
            << reserva.getContacto() << ','
            << reserva.getComensales() << ','
            << reserva.getFecha().toString("dd/MM/yyyy") << ','
            << reserva.getHora().toString("HH:mm") << ','
            << reserva.getMesa() << '\n';
    }

    file.close();
    return true;
}

QVector<Reserva> GuardarReservas::cargar(const QString& nombreArchivo)
{
    QVector<Reserva> reservas;
    QFile file(nombreArchivo);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para lectura:" << nombreArchivo;
        return reservas;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        qDebug() << "Línea leída:" << line;  // Depuración: Ver la línea leída
        if (fields.size() == 7) {
            QString codigo = fields[0];
            QString nombre = fields[1];
            QString contacto = fields[2];
            int comensales = fields[3].toInt();
            QDate fecha = QDate::fromString(fields[4], "dd/MM/yyyy");
            QTime hora = QTime::fromString(fields[5], "HH:mm");
            QString mesa = fields[6];
            Reserva reserva(codigo, nombre, contacto, comensales, fecha, hora, mesa);
            reservas.append(reserva);
        } else {
            qDebug() << "Formato de línea incorrecto:" << line;  // Depuración: Ver formato de línea
        }
    }

    file.close();
    return reservas;
}
