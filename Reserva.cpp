#include "Reserva.h"

Reserva::Reserva(const QString &codigo, const QString &nombre, const QString &contacto, int comensales, const QDate &fecha, const QTime &hora, const QString &mesa)
    : codigo(codigo), nombre(nombre), contacto(contacto), comensales(comensales), fecha(fecha), hora(hora), mesa(mesa)
{
}

QString Reserva::getCodigo() const
{
    return codigo;
}

QString Reserva::getNombre() const
{
    return nombre;
}

QString Reserva::getContacto() const
{
    return contacto;
}

int Reserva::getComensales() const
{
    return comensales;
}

QDate Reserva::getFecha() const
{
    return fecha;
}

QTime Reserva::getHora() const
{
    return hora;
}

QString Reserva::getMesa() const
{
    return mesa;
}

void Reserva::setNombre(const QString &nombre)
{
    this->nombre = nombre;
}

void Reserva::setContacto(const QString &contacto)
{
    this->contacto = contacto;
}

void Reserva::setComensales(int comensales)
{
    this->comensales = comensales;
}

void Reserva::setFecha(const QDate &fecha)
{
    this->fecha = fecha;
}

void Reserva::setHora(const QTime &hora)
{
    this->hora = hora;
}

void Reserva::setMesa(const QString &mesa)
{
    this->mesa = mesa;
}
