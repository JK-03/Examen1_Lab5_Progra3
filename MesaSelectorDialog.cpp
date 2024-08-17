#include "MesaSelectorDialog.h"
#include <QVBoxLayout>
#include <QPushButton>

MesaSelectorDialog::MesaSelectorDialog(const QVector<int>& mesasDisponibles, QWidget* parent)
    : QDialog(parent), seleccion(-1) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    for (int mesa : mesasDisponibles) {
        QPushButton* boton = new QPushButton(QString("Mesa %1").arg(mesa), this);
        layout->addWidget(boton);

        connect(boton, &QPushButton::clicked, [this, mesa]() {
            seleccion = mesa;
            accept();  // Cierra el diálogo y acepta la selección
        });
    }

    setLayout(layout);
    setWindowTitle("Seleccionar Mesa");
}

int MesaSelectorDialog::mesaSeleccionada() const {
    return seleccion;
}
