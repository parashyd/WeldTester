#ifndef OPEN_H
#define OPEN_H

#include <QAbstractListModel>

class open : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit open(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // OPEN_H
