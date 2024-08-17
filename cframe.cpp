#include "cframe.h"
#include "ui_cframe.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDebug>
#include "ReservaTableWidget.h"
#include "MesaSelectorDialog.h"
#include <QRegularExpression>


cframe::cframe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cframe)
{
    ui->setupUi(this);

    //Guardado Automático - 15 min
    backupTimer = new QTimer(this);
    connect(backupTimer, &QTimer::timeout, this, &cframe::realizarRespaldo);
    backupTimer->start(900000);

    this->setWindowTitle("Examen 1");
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabEnabled(1 ,false);
    actualizarComboBoxMesas();
}

cframe::~cframe()
{
    delete ui;
}

void cframe::realizarRespaldo()
{
    QString nombreArchivoBackup = "respaldo_reservas.txt";
    bool exito = GuardarReservas::guardar(reservasLista, nombreArchivoBackup);
    if (exito) {
        qDebug() << "Copia de seguridad realizada con éxito.";
    } else {
        qWarning() << "Error al realizar la copia de seguridad.";
    }
}

void cframe::on_botonRespaldoManual_clicked()
{
    QString nombreArchivoBackup = "respaldo_reservas.txt";
    bool exito = GuardarReservas::guardar(reservasLista, nombreArchivoBackup);
    if (exito) {
        QMessageBox::information(this, "Respaldo Manual", "Copia de seguridad realizada con éxito.");
    } else {
        QMessageBox::warning(this, "Respaldo Manual", "Error al realizar la copia de seguridad.");
    }
}

void cframe::actualizarComboBoxMesas() {
    ui->comboBoxMesas->clear();

    for (int i = 1; i <= 20; ++i) {
        QString mesa = QString::number(i);
        ui->comboBoxMesas->addItem(mesa);
    }
}

void cframe::on_pushButton_clicked()
{
    TipoUsuario = "cliente";
    ui->tabWidget->setCurrentIndex(1);
    ui->tabWidget->setTabEnabled(1 ,true);
}


void cframe::on_pushButton_2_clicked()
{
    TipoUsuario = "admin";
    ui->tabWidget->setCurrentIndex(1);
    ui->tabWidget->setTabEnabled(1 ,true);
}


void cframe::on_pushButton_3_clicked()
{
    if (TipoUsuario == "cliente") {

        std::string opcion = "";

        QMessageBox msgBox;
        msgBox.setText("Por favor, seleccione una opción:");

        QPushButton *opcion6 = msgBox.addButton("Salir", QMessageBox::ActionRole);
        QPushButton *opcion5 = msgBox.addButton("Cancelar una Reserva", QMessageBox::ActionRole);
        QPushButton *opcion4 = msgBox.addButton("Modificar una Reserva", QMessageBox::ActionRole);
        QPushButton *opcion3 = msgBox.addButton("Consultar Disponibilidad de Mesas", QMessageBox::ActionRole);
        QPushButton *opcion2 = msgBox.addButton("Nueva Reserva", QMessageBox::ActionRole);

        msgBox.exec();

        if (msgBox.clickedButton() == opcion2) {
            ui->tabWidget->setCurrentIndex(2);
        } else if (msgBox.clickedButton() == opcion3) {
            ui->tabWidget->setCurrentIndex(3);
        } else if (msgBox.clickedButton() == opcion4) {
            ui->tabWidget->setCurrentIndex(4);
        } else if (msgBox.clickedButton() == opcion5) {
            ui->tabWidget->setCurrentIndex(4);
        } else if (msgBox.clickedButton() == opcion6) {
            this->close();
        }

    } else if (TipoUsuario == "admin") {

        std::string opcion = "";

        QMessageBox msgBox;
        msgBox.setText("Por favor, seleccione una opción:");

        QPushButton *opcion6 = msgBox.addButton("Salir", QMessageBox::ActionRole);
         QPushButton *opcion7 = msgBox.addButton("Lista de Todas las Reservas", QMessageBox::ActionRole);
        QPushButton *opcion5 = msgBox.addButton("Cancelar una Reserva", QMessageBox::ActionRole);
        QPushButton *opcion4 = msgBox.addButton("Modificar una Reserva", QMessageBox::ActionRole);
        QPushButton *opcion3 = msgBox.addButton("Consultar Disponibilidad de Mesas", QMessageBox::ActionRole);
        QPushButton *opcion2 = msgBox.addButton("Consultar Información de Reserva", QMessageBox::ActionRole);

        msgBox.exec();

        if (msgBox.clickedButton() == opcion2) {
            ui->tabWidget->setCurrentIndex(0);
        } else if (msgBox.clickedButton() == opcion3) {
            ui->tabWidget->setCurrentIndex(3);
        } else if (msgBox.clickedButton() == opcion4) {
            ui->tabWidget->setCurrentIndex(3);
        } else if (msgBox.clickedButton() == opcion5) {
            ui->tabWidget->setCurrentIndex(4);
        } else if (msgBox.clickedButton() == opcion6) {
            this->close();
        } else if (msgBox.clickedButton() == opcion7) {
            ui->tabWidget->setCurrentIndex(5);

            ReservaTableManager *tableManager = new ReservaTableManager(ui->tableWidget, this);
            tableManager->setReservas(reservasLista);
        }
    }
}

void cframe::on_botonReserva_clicked()
{
    if (modificarActivo) {
        ReservaModificacion();
    } else {
        CrearNuevaReserva();
    }
}

void cframe::ReservaModificacion()
{
    QString nombre = ui->LE_Nombre->text();
    QString contacto = ui->LE_Contacto->text();
    int comensales = ui->spb_Comensales->value();
    QDate fecha = ui->DE_Fecha->date();
    QTime hora = ui->TE_Hora->time();
    QString mesa = ui->comboBoxMesas->currentText();

    QString codigoReserva = ui->LE_CodigoReserva->text();

    if (!verificarDisponibilidadMesa(mesa, fecha, hora)) {
        QMessageBox::warning(this, tr("Mesa Ocupada"), "La mesa seleccionada ya está reservada en el día y hora seleccionados.");
        return;
    }

    if (!verificarDisponibilidad(fecha, hora)) {
        QVector<QDateTime> alternativas = sugerirAlternativas(fecha, hora);
        QString mensaje = "La fecha y hora seleccionadas no están disponibles.\n\nAlternativas:\n";
        for (const auto& alternativa : alternativas) {
            mensaje += alternativa.toString("dd/MM/yyyy hh:mm") + "\n";
        }
        QMessageBox::warning(this, tr("Disponibilidad"), mensaje);
        return;
    }

    for (auto& reserva : reservasLista) {
        if (reserva.getCodigo() == codigoReserva) {
            reserva.setNombre(nombre);
            reserva.setContacto(contacto);
            reserva.setComensales(comensales);
            reserva.setFecha(fecha);
            reserva.setHora(hora);
            reserva.setMesa(mesa);

            QMessageBox::information(this, "Reserva Modificada", QString("Reserva modificada.\nCódigo: %1").arg(codigoReserva));
            LimpiarCampos();
            modificarActivo = false;
            ui->botonReserva->setText("Crear Reserva");
            return;
        }
    }

    QMessageBox::warning(this, "Reserva no encontrada", "No se encontró ninguna reserva con el código ingresado.");
}

void cframe::CrearNuevaReserva()
{
    QString nombre = ui->LE_Nombre->text();
    QString contacto = ui->LE_Contacto->text();
    int comensales = ui->spb_Comensales->value();
    QDate fecha = ui->DE_Fecha->date();
    QTime hora = ui->TE_Hora->time();
    QString mesa = ui->comboBoxMesas->currentText();

    if (!verificarDisponibilidadMesa(mesa, fecha, hora)) {
        QMessageBox::warning(this, tr("Mesa Ocupada"), "La mesa seleccionada ya está reservada en el día y hora seleccionados.");
        return;
    }

    if (!verificarDisponibilidad(fecha, hora)) {
        QVector<QDateTime> alternativas = sugerirAlternativas(fecha, hora);
        QString mensaje = "La fecha y hora seleccionadas no están disponibles.\n\nAlternativas:\n";
        for (const auto& alternativa : alternativas) {
            mensaje += alternativa.toString("dd/MM/yyyy hh:mm") + "\n";
        }
        QMessageBox::warning(this, tr("Disponibilidad"), mensaje);
        return;
    }

    QString codigo = generarCodigoReserva();
    Reserva nuevaReserva(codigo, nombre, contacto, comensales, fecha, hora, mesa);
    reservasLista.append(nuevaReserva);

    QMessageBox::information(this, tr("Reserva Confirmada"), QString("Reserva confirmada.\nCódigo: %1").arg(codigo));
    LimpiarCampos();
}


bool cframe::verificarDisponibilidadMesa(const QString &mesa, const QDate &fecha, const QTime &hora) const
{
    for (const auto& reserva : reservasLista) {
        if (reserva.getMesa() == mesa && reserva.getFecha() == fecha && reserva.getHora() == hora) {
            return false;
        }
    }
    return true;
}


bool cframe::verificarDisponibilidad(const QDate &fecha, const QTime &hora) const
{
    for (const auto& reserva : reservasLista) {
        if (reserva.getFecha() == fecha && reserva.getHora() == hora) {
            return false;
        }
    }
    return true;
}

QVector<QDateTime> cframe::sugerirAlternativas(const QDate &fecha, const QTime &hora)
{
    QVector<QDateTime> alternativas;
    for (int i = 1; i <= 3; ++i) {
        QTime alternativaHora = hora.addSecs(3600 * i);
        if (verificarDisponibilidad(fecha, alternativaHora)) {
            alternativas.append(QDateTime(fecha, alternativaHora));
        }
    }
    return alternativas;
}

QString cframe::generarCodigoReserva()
{
    QString prefijo = QString::fromUtf8("CD");
    QString numeros = QString::number(QRandomGenerator::global()->bounded(10000), 10).rightJustified(4, '0');
    return prefijo + numeros;
}

void cframe::consultarDisponibilidad(const QDate &fecha, const QTime &hora)
{
    QString disponibilidad;
    QVector<QString> mesasDisponibles;

    for (int i = 1; i <= 20; ++i) {
        mesasDisponibles.append(QString("Mesa %1 - Disponible").arg(i));
    }

    qDebug() << "Consultando disponibilidad para la fecha:" << fecha.toString()
             << "y la hora:" << hora.toString();

    for (const auto& reserva : reservasLista) {
        qDebug() << "Reserva encontrada - Fecha:" << reserva.getFecha().toString()
                 << "Hora:" << reserva.getHora().toString()
                 << "Mesa:" << reserva.getMesa();

        if (reserva.getFecha() == fecha && reserva.getHora() == hora) {
            QString mesaReserva = reserva.getMesa();

            // Usar expresión regular para extraer el número de la mesa
            QRegularExpression regex("Mesa (\\d+)");
            QRegularExpressionMatch match = regex.match(mesaReserva);
            if (match.hasMatch()) {
                int mesaNumero = match.captured(1).toInt();
                QString mesaFormato = QString("Mesa %1 - Ocupada").arg(mesaNumero);

                // Actualizar la lista de mesas disponibles
                for (int i = 0; i < mesasDisponibles.size(); ++i) {
                    if (mesasDisponibles[i].startsWith(QString("Mesa %1").arg(mesaNumero))) {
                        mesasDisponibles[i] = mesaFormato;
                        qDebug() << "Mesa " << mesaNumero << " marcada como ocupada.";
                        break;
                    }
                }
            }
        }
    }

    disponibilidad = "Disponibilidad de mesas para la fecha y hora solicitadas:\n";
    for (const auto& mesa : mesasDisponibles) {
        disponibilidad += mesa + "\n";
    }

    if (disponibilidad == "Disponibilidad de mesas para la fecha y hora solicitadas:\n") {
        disponibilidad = "No hay mesas disponibles para la fecha y hora solicitadas.";
    }

    qDebug() << "Mensaje final de disponibilidad:\n" << disponibilidad;

    ui->PTE_ResultadosDisponibilidad->setPlainText(disponibilidad);
}

void cframe::on_botonReserva_3_clicked()
{
    QDate fecha = ui->DE_Fecha->date();
    QTime hora = ui->TE_Hora->time();

    consultarDisponibilidad(fecha, hora);
}

void cframe::on_botonReserva_4_clicked()
{
    QString codigoReserva = ui->LE_CodigoReserva->text();

    QString resultados;
    bool reservaEncontrada = false;

    for (const auto& reserva : reservasLista) {
        if (reserva.getCodigo() == codigoReserva) {
            reservaEncontrada = true;
            resultados += QString("Reserva encontrada:\nNombre: %1\nContacto: %2\nComensales: %3\nFecha: %4\nHora: %5\nMesa: %6\nCódigo: %7\n")
                              .arg(reserva.getNombre())
                              .arg(reserva.getContacto())
                              .arg(reserva.getComensales())
                              .arg(reserva.getFecha().toString("dd/MM/yyyy"))
                              .arg(reserva.getHora().toString("HH:mm"))
                              .arg(reserva.getMesa())
                              .arg(reserva.getCodigo());
            break;
        }
    }

    if (!reservaEncontrada) {
        resultados = "No se encontró ninguna reserva con el código ingresado.";
    }

    ui->PTE_ResultadosCodigo->setPlainText(resultados);
}

void cframe::on_botonCancelarReserva_clicked()
{
    QString codigoReserva = ui->LE_CodigoReserva->text();

    if (codigoReserva.isEmpty()) {
        QMessageBox::warning(this, "Código de reserva vacío", "Por favor, ingrese un código de reserva.");
        return;
    }

    auto it = std::remove_if(reservasLista.begin(), reservasLista.end(),
                             [&codigoReserva](const Reserva& reserva) {
                                 return reserva.getCodigo() == codigoReserva;
                             });

    if (it != reservasLista.end()) {
        reservasLista.erase(it, reservasLista.end());

        QMessageBox::information(this, "Reserva Cancelada", QString("Reserva cancelada.\nCódigo: %1").arg(codigoReserva));
    } else {
        QMessageBox::warning(this, "Reserva no encontrada", "No se encontró ninguna reserva con el código ingresado.");
    }

    ui->LE_CodigoReserva->clear();
    ui->LE_Nombre->clear();
    ui->LE_Contacto->clear();
    ui->spb_Comensales->setValue(0);
    ui->DE_Fecha->setDate(QDate::currentDate());
    ui->TE_Hora->setTime(QTime::currentTime());
}

void cframe::LimpiarCampos() {
    ui->LE_CodigoReserva->clear();
    ui->LE_Nombre->clear();
    ui->LE_Contacto->clear();
    ui->spb_Comensales->setValue(0);
    ui->DE_Fecha->setDate(QDate::currentDate());
    ui->TE_Hora->setTime(QTime::currentTime());
}

void cframe::on_botonModificar_clicked()
{
    QString codigoReserva = ui->LE_CodigoReserva->text();

    for (const auto& reserva : reservasLista) {
        if (reserva.getCodigo() == codigoReserva) {
            ui->LE_Nombre->setText(reserva.getNombre());
            ui->LE_Contacto->setText(reserva.getContacto());
            ui->spb_Comensales->setValue(reserva.getComensales());
            ui->DE_Fecha->setDate(reserva.getFecha());
            ui->TE_Hora->setTime(reserva.getHora());
            ui->comboBoxMesas->setCurrentText(reserva.getMesa());

            modificarActivo = true;
            ui->tabWidget->setCurrentIndex(2);
            return;
        }
    }

    QMessageBox::warning(this, "Reserva no encontrada", "No se encontró ninguna reserva con el código ingresado.");
}

void cframe::on_pushButton_7_clicked()
{
    QString codigoReserva = ui->LE_CodigoReserva->text();

    auto it = std::remove_if(reservasLista.begin(), reservasLista.end(),
                             [&codigoReserva](const Reserva& reserva) {
                                 return reserva.getCodigo() == codigoReserva;
                             });

    if (it != reservasLista.end()) {
        reservasLista.erase(it, reservasLista.end());

        QMessageBox::information(this, "Reserva Cancelada", QString("Reserva cancelada.\nCódigo: %1").arg(codigoReserva));

        ui->LE_CodigoReserva->clear();
        ui->PTE_ResultadosCodigo->clear();

        ReservaTableManager tableManager(ui->tableWidget);
        tableManager.setReservas(reservasLista);
    } else {
        QMessageBox::warning(this, "Reserva no encontrada", "No se encontró ninguna reserva con el código ingresado.");
    }
}

