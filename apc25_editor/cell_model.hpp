#ifndef CELL_MODEL_H
#define CELL_MODEL_H

#include <common/button_state.hpp>

#include <QAbstractListModel>
#include <QColor>

struct Cell {
    apc25::ButtonState state;
};

class CellModel : public QAbstractListModel {
    Q_OBJECT;
public:
    enum Roles { ColorRole = Qt::UserRole + 1 };

    explicit CellModel(int rows, int cols, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setColor(int index, int color);
    apc25::ButtonState colorAt(int index) const;  // call freely from C++

private:
    QList<Cell> m_cells;
    int m_cols;
};

#endif // CELL_MODEL_H

