#include "cell_model.hpp"
#include "common/button_state.hpp"

#include <QByteArray>
#include <QColor>
#include <QModelIndex>
#include <QVariant>

#include <iostream>


CellModel::CellModel(int rows, int cols, QObject* parent)
    : QAbstractListModel(parent), m_cols(cols)
{
    m_cells.resize(rows * cols);
}

int CellModel::rowCount(const QModelIndex&) const {
    return m_cells.size();
}

QVariant CellModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_cells.size()) {
        return {};
    }
    if (role == ColorRole) {
        return QVariant::fromValue(m_cells[index.row()].state);
    }
    return {};
}

QHash<int, QByteArray> CellModel::roleNames() const {
    return { { ColorRole, "cellColor" } };
}

void CellModel::setColor(int index, int color) {
    if (index < 0 || index >= m_cells.size()) return;

    switch (color) {
    case 0:
      m_cells[index].state = apc25::ButtonState::Off;
      break;
    case 1:
      m_cells[index].state = apc25::ButtonState::Green;
      break;
    case 2:
      m_cells[index].state = apc25::ButtonState::GreenBlink;
      break;
    case 3:
      m_cells[index].state = apc25::ButtonState::Red;
      break;
    case 4:
      m_cells[index].state = apc25::ButtonState::RedBlink;
      break;
    case 5:
      m_cells[index].state = apc25::ButtonState::Yellow;
      break;
    case 6:
      m_cells[index].state = apc25::ButtonState::YellowBlink;
      break;
    }

    std::cout << "setting a value" << std::endl;

    emit dataChanged(createIndex(index, 0), createIndex(index, 0), {ColorRole});
}

apc25::ButtonState CellModel::colorAt(int index) const {
    if (index < 0 || index >= m_cells.size()) return apc25::ButtonState::Off;
    return m_cells[index].state;
}
