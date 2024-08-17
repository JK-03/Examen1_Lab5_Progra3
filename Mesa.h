#ifndef MESA_H
#define MESA_H

#include <QString>

class Mesa {
public:
    Mesa(int id, bool disponible = true);

    int getId() const;
    bool isDisponible() const;
    void setDisponible(bool disponible);

private:
    int id;                // Identificador de la mesa
    bool disponible;       // Estado de disponibilidad
};

#endif // MESA_H
