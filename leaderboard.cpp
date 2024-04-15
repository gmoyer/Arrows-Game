#include "leaderboard.h"
#include <QAbstractTableModel>
#include <QVector>

const QString LeaderBoard::headerNames[4] = {
    "Name",
    "Wins",
    "Games",
    "Win rate"
};
const int LeaderBoard::sizeOfSet = 10;

LeaderBoard::LeaderBoard(QObject *parent)
    : QAbstractTableModel{parent}
{
    set = 0;
}


int LeaderBoard::rowCount(const QModelIndex &parent = QModelIndex()) const {
    Q_UNUSED(parent);
    return nextButton() ? sizeOfSet : playerData.size() % sizeOfSet;
}
int LeaderBoard::columnCount(const QModelIndex &parent = QModelIndex()) const {
    Q_UNUSED(parent);
    return 4;
}
QVariant LeaderBoard::data(const QModelIndex &index, int role = Qt::DisplayRole) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        Player* player = playerData[index.row()+(set * sizeOfSet)];
        switch (index.column()) {
        case 0:
            return player->getName();
        case 1:
            return player->getWinCount();
        case 2:
            return player->getGameCount();
        case 3:
            return player->getWinPercentageString();

        }
    }

    return QVariant();
}
QVariant LeaderBoard::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            return headerNames[section];
        }
    }
    return QVariant();
}
void LeaderBoard::setPlayerData(QVector<Player *> data) {
    beginResetModel();
    playerData = data;
    std::sort(playerData.begin(), playerData.end(), [](const Player* a, const Player* b) {return *a > *b;});
    endResetModel();
}

void LeaderBoard::nextSet() {
    if (nextButton()) set++;
    //reset();
}
void LeaderBoard::prevSet() {
    if (prevButton()) set--;
    //reset();
}
QString LeaderBoard::status() {
    return QString("Showing results %1 through %2 of %3")
        .arg(QString::number((set*sizeOfSet)+1),
             QString::number(qMin((set+1)*sizeOfSet, playerData.size())),
             QString::number(playerData.size()));
}
bool LeaderBoard::nextButton() const {
    return set < playerData.size() / sizeOfSet;
}
bool LeaderBoard::prevButton() const {
    return set > 0;
}
