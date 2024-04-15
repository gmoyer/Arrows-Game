#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QAbstractTableModel>
#include "player.h"

class LeaderBoard : public QAbstractTableModel
{
    Q_OBJECT
private:
    QVector<Player*> playerData;
    static const QString headerNames[4];
    int set;
    static const int sizeOfSet;
public:
    explicit LeaderBoard(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setPlayerData(QVector<Player*> data);
    void nextSet();
    void prevSet();
    QString status();
    bool prevButton() const;
    bool nextButton() const;
};

#endif // LEADERBOARD_H
