#ifndef RESERVA_H
#define RESERVA_H

#include <QString>
#include <QDate>
#include <QTime>

class Reserva
{
public:
    Reserva(const QString &codigo, const QString &nombre, const QString &contacto, int comensales, const QDate &fecha, const QTime &hora, const QString &mesa);

    QString getCodigo() const;
    QString getNombre() const;
    QString getContacto() const;
    int getComensales() const;
    QDate getFecha() const;
    QTime getHora() const;
    QString getMesa() const;

    void setNombre(const QString &nombre);
    void setContacto(const QString &contacto);
    void setComensales(int comensales);
    void setFecha(const QDate &fecha);
    void setHora(const QTime &hora);
    void setMesa(const QString &mesa);

private:
    QString codigo;
    QString nombre;
    QString contacto;
    int comensales;
    QDate fecha;
    QTime hora;
    QString mesa;
};

#endif // RESERVA_H
