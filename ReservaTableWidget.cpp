#include "ReservaTableWidget.h"

ReservaTableManager::ReservaTableManager(QTableWidget *tableWidget, QObject *parent)
    : QObject(parent), tableWidget(tableWidget)
{
    setupTable();
}

void ReservaTableManager::setupTable()
{
    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Código" << "Nombre" << "Contacto" << "Comensales" << "Fecha" << "Hora" << "Mesa");
}

void ReservaTableManager::fillTable(const QVector<Reserva>& reservas)
{
    tableWidget->setRowCount(reservas.size());

    for (int row = 0; row < reservas.size(); ++row) {
        const Reserva& reserva = reservas[row];
        tableWidget->setItem(row, 0, new QTableWidgetItem(reserva.getCodigo()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(reserva.getNombre()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(reserva.getContacto()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(reserva.getComensales())));
        tableWidget->setItem(row, 4, new QTableWidgetItem(reserva.getFecha().toString("dd/MM/yyyy")));
        tableWidget->setItem(row, 5, new QTableWidgetItem(reserva.getHora().toString("HH:mm")));
        tableWidget->setItem(row, 6, new QTableWidgetItem(reserva.getMesa()));
    }
}

void ReservaTableManager::setReservas(const QVector<Reserva>& reservas)
{
    fillTable(reservas);
}
