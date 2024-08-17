#include "Mesa.h"

Mesa::Mesa(int id, bool disponible)
    : id(id), disponible(disponible) {}

int Mesa::getId() const {
    return id;
}

bool Mesa::isDisponible() const {
    return disponible;
}

void Mesa::setDisponible(bool disponible) {
    this->disponible = disponible;
}
