#ifndef MESASELECTORDIALOG_H
#define MESASELECTORDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>

class MesaSelectorDialog : public QDialog {
    Q_OBJECT

public:
    explicit MesaSelectorDialog(const QVector<int>& mesasDisponibles, QWidget* parent = nullptr);
    int mesaSeleccionada() const;

private:
    int seleccion;
};

#endif // MESASELECTORDIALOG_H
