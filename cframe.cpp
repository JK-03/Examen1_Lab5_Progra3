#include "cframe.h"
#include "ui_cframe.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "ReservaTableWidget.h"
#include "MesaSelectorDialog.h"
#include <QRegularExpression>
#include <QTimer>
#include <QDebug>

cframe::cframe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cframe)
{
    ui->setupUi(this);

    //Cargar automáticamente
    cargarReservasDesdeArchivo();

    this->setWindowTitle("Examen 1");
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabEnabled(1 ,false);
    ui->tabWidget->setTabEnabled(2 ,false);
    ui->tabWidget->setTabEnabled(3 ,false);
    ui->tabWidget->setTabEnabled(4 ,false);
    ui->tabWidget->setTabEnabled(5 ,false);
    actualizarComboBoxMesas();
}

cframe::~cframe()
{
    delete ui;
}

void cframe::Accesos() {
    ui->tabWidget->setTabEnabled(0 ,true);
    ui->tabWidget->setTabEnabled(1 ,false);
    ui->tabWidget->setTabEnabled(2 ,false);
    ui->tabWidget->setTabEnabled(3 ,false);
    ui->tabWidget->setTabEnabled(4 ,false);
    ui->tabWidget->setTabEnabled(5 ,false);
}

void cframe::cargarReservasDesdeArchivo() {
    QString nombreArchivo = "reservas.txt";
    QString directorio = QDir::currentPath();
    QString rutaArchivo = QDir(directorio).filePath(nombreArchivo);

    if (QFile::exists(rutaArchivo)) {
        QVector<Reserva> reservasCargadas = GuardarReservas::cargar(rutaArchivo);
        if (!reservasCargadas.isEmpty()) {
            reservasLista = reservasCargadas;
            ReservaTableManager tableManager(ui->tableWidget);
            tableManager.setReservas(reservasLista);

            QString mensaje = QString("Número de reservas cargadas: %1").arg(reservasLista.size());
        }
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
    ui->tabWidget->setTabEnabled(0 ,false);
    ui->tabWidget->setTabEnabled(1 ,true);
    ui->tabWidget->setTabEnabled(2 ,true);
    ui->tabWidget->setTabEnabled(3 ,true);
    ui->tabWidget->setTabEnabled(4 ,true);

    ui->pushButton_7->setVisible(false);
}


void cframe::on_pushButton_2_clicked()
{
    TipoUsuario = "admin";
    ui->tabWidget->setCurrentIndex(1);
    ui->tabWidget->setTabEnabled(0 ,false);
    ui->tabWidget->setTabEnabled(1 ,true);
    ui->tabWidget->setTabEnabled(2 ,true);
    ui->tabWidget->setTabEnabled(4 ,true);
    ui->tabWidget->setTabEnabled(3 ,true);
    ui->tabWidget->setTabEnabled(5 ,true);

    ui->pushButton_7->setVisible(true);

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
            ui->tabWidget->setCurrentIndex(4);
        } else if (msgBox.clickedButton() == opcion3) {
            ui->tabWidget->setCurrentIndex(3);
        } else if (msgBox.clickedButton() == opcion4) {
            ui->tabWidget->setCurrentIndex(4);
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
    if (TipoUsuario == "cliente") {
        if (modificarActivo) {
            ui->botonReserva->setText("Modificar Reserva");
            ReservaModificacion();
        } else {
            ui->botonReserva->setText("Crear Reserva");
            CrearNuevaReserva();
        }
    } else if (TipoUsuario == "admin") {
        if (modificarActivo) {
            ui->botonReserva->setText("Modificar Reserva");
            ReservaModificacion();
        } else {
            QMessageBox::warning(this, "Acceso Denegado", "Como admin, solo puedes modificar reservas.");
        }
    } else {
        QMessageBox::warning(this, "Acceso Denegado", "Tipo de usuario no reconocido.");
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

    for (auto& reserva : reservasLista) {
        if (reserva.getCodigo() == codigoReserva) {
            // Compara si la fecha, hora y mesa no han cambiado
            bool mismaMesaFechaHora = (reserva.getFecha() == fecha && reserva.getHora() == hora && reserva.getMesa() == mesa);

            // Solo verifica la disponibilidad si no es la misma mesa, fecha y hora
            if (!mismaMesaFechaHora && !verificarDisponibilidadMesa(mesa, fecha, hora)) {
                QMessageBox::warning(this, tr("Mesa Ocupada"),
                                     QString("La mesa seleccionada ya está reservada en el día y hora seleccionados.\n\n"
                                             "Puedes consultar la disponibilidad de mesas en la sección correspondiente o "
                                             "intentar reservar en otra fecha u hora.\n\n")
                                         + tr("Si deseas ver las alternativas, consulta el siguiente mensaje."));
                QVector<QDateTime> alternativas = sugerirAlternativas(fecha, hora);
                QString mensaje = "Alternativas disponibles:\n";
                for (const auto& alternativa : alternativas) {
                    mensaje += alternativa.toString("dd/MM/yyyy hh:mm") + "\n";
                }
                QMessageBox::information(this, tr("Disponibilidad Alternativa"), mensaje);
                return;
            }

            // Actualiza la reserva
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
        QMessageBox::warning(this, tr("Mesa Ocupada"),
                             QString("La mesa seleccionada ya está reservada en el día y hora seleccionados.\n\n"
                                     "Puedes consultar la disponibilidad de mesas en la sección correspondiente o "
                                     "intentar reservar en otra fecha u hora.\n\n")
                                 + tr("Si deseas ver las alternativas, consulta el siguiente mensaje."));
        QVector<QDateTime> alternativas = sugerirAlternativas(fecha, hora);
        QString mensaje = "Alternativas disponibles:\n";
        for (const auto& alternativa : alternativas) {
            mensaje += alternativa.toString("dd/MM/yyyy hh:mm") + "\n";
        }
        QMessageBox::information(this, tr("Disponibilidad Alternativa"), mensaje);
        return;
    }

    QString codigo = generarCodigoReserva();
    Reserva nuevaReserva(codigo, nombre, contacto, comensales, fecha, hora, mesa);
    reservasLista.append(nuevaReserva);

    QMessageBox::information(this, tr("Reserva Confirmada"), QString("Reserva confirmada.\nCódigo: %1").arg(codigo));
    LimpiarCampos();
}

bool cframe::verificarDisponibilidadMesa(const QString& mesa, const QDate& fecha, const QTime& hora) const
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

    QString mesasDisponibles[20];
    for (int i = 0; i < 20; ++i) {
        mesasDisponibles[i] = QString("Mesa %1 - Disponible").arg(i + 1);
    }

    for (const auto& reserva : reservasLista) {
        if (reserva.getFecha() == fecha && reserva.getHora() == hora) {
            int mesaNumero = reserva.getMesa().toInt();

            if (mesaNumero >= 1 && mesaNumero <= 20) {
                mesasDisponibles[mesaNumero - 1] = QString("Mesa %1 - Ocupada").arg(mesaNumero);
            }
        }
    }

    disponibilidad = "Disponibilidad de mesas para la fecha y hora solicitadas:\n";
    for (int i = 0; i < 20; ++i) {
        disponibilidad += mesasDisponibles[i] + "\n";
    }

    ui->PTE_ResultadosDisponibilidad->setPlainText(disponibilidad);
}

void cframe::on_botonReserva_3_clicked()
{
    QDate fecha = ui->DE_FechaDisponibilidad->date();
    QTime hora = ui->TE_HoraDisponibilidad->time();

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

    QMessageBox::StandardButton respuesta;
    respuesta = QMessageBox::question(this, "Confirmar Cancelación",
                                      QString("¿Está seguro que desea cancelar la reservación con el código: %1?").arg(codigoReserva),
                                      QMessageBox::Yes | QMessageBox::No);

    if (respuesta == QMessageBox::Yes) {
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

        ui->PTE_ResultadosCodigo->clear();
    }
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
            ui->botonReserva->setText("Modificar Reserva");
            ui->tabWidget->setCurrentIndex(2);
            return;
        }
    }

    QMessageBox::warning(this, "Reserva no encontrada", "No se encontró ninguna reserva con el código ingresado.");
}

void cframe::on_pushButton_7_clicked()
{
    QString nombreArchivo = "reservas.txt";
    QString directorio = QDir::currentPath();
    QString rutaArchivo = QDir(directorio).filePath(nombreArchivo);

    QFile file(rutaArchivo);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo guardar las reservas en el archivo %1.").arg(rutaArchivo));
        return;
    }

    QTextStream out(&file);
    for (const auto& reserva : reservasLista) {
        out << reserva.getCodigo() << ','
            << reserva.getNombre() << ','
            << reserva.getContacto() << ','
            << reserva.getComensales() << ','
            << reserva.getFecha().toString("dd/MM/yyyy") << ','
            << reserva.getHora().toString("HH:mm") << ','
            << reserva.getMesa() << '\n';
    }

    file.close();

    QMessageBox::information(this, tr("Guardar Reservaciones"), tr("Las reservas han sido guardadas en el archivo."));
}


void cframe::on_botonUser_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
    Accesos();
}

