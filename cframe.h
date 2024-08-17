#ifndef CFRAME_H
#define CFRAME_H

#include <QMainWindow>
#include <QVector>
#include <Reserva.h>
#include <Mesa.h>
#include "GuardarReservas.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class cframe;
}
QT_END_NAMESPACE

class cframe : public QMainWindow
{
    Q_OBJECT

public:
    cframe(QWidget *parent = nullptr);
    ~cframe();

    bool verificarDisponibilidad(const QDate &fecha, const QTime &hora) const;
    QVector<QDateTime> sugerirAlternativas(const QDate &fecha, const QTime &hora);
    bool verificarDisponibilidadMesa(const QString &mesa, const QDate &fecha, const QTime &hora) const;
    void cargarReservasDesdeArchivo();

    void consultarDisponibilidad(const QDate &fecha, const QTime &hora);
    void ReservaModificacion();
    void CrearNuevaReserva();
    void LimpiarCampos();
    void inicializarMesas();
    void actualizarComboBoxMesas();
    void realizarRespaldo();
    void Accesos();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_botonReserva_clicked();

    void on_botonReserva_3_clicked();

    void on_botonReserva_4_clicked();

    void on_botonModificar_clicked();

    void on_botonCancelarReserva_clicked();

    void on_pushButton_7_clicked();

    void on_botonUser_clicked();

private:
    Ui::cframe *ui;
    QString TipoUsuario;

    QVector<Reserva> reservasLista;
    QString generarCodigoReserva();
    QString codigoReserva = "";
    bool modificarActivo = false;

    QVector<Mesa> mesas;
};
#endif // CFRAME_H
