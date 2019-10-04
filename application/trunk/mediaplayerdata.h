#ifndef MEDIAPLAYERDATA_H
#define MEDIAPLAYERDATA_H

#include "global.h"

#include <QAbstractListModel>
#include <QVector>

class MediaPlayerData : public QAbstractListModel
{
public:
	enum MODEL_ITEM_ID
	{
		MODEL_ITEM_ICON_ID = Qt::UserRole + 1,
		MODEL_ITEM_NAME_ID,

		MODEL_ITEM_MAX_ID
	};

	enum ITEM_TYPE
	{
		ITEM_BACK_TYPE,
		ITEM_FOLDER_TYPE,
		ITEM_MULTIMEDIA_FILE_TYPE,

		ITEM_TYPE_NUMBER
	};

#define ITEM_PLAYING_TYPE          ITEM_TYPE_NUMBER

	struct MODEL_ITEM
	{
		int       id;
		QString   name;
		ITEM_TYPE type;
		QFileInfo info;
	};

	// Signals and Slots
public slots:

signals:

	// Functions
public:
	explicit MediaPlayerData(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;

	void SetRootPath(const QString &path);

	void SetItems(const QFileInfoList &file_list);
	void GetItem(int id, MODEL_ITEM *item);

	void SetItemPlaying(const QFileInfo &item_playing);
private:

	// Variables
private:
	QVector<MODEL_ITEM>        _Items;
	QFileInfo                  _Item_Playing;
	QString                    _Root_Path;
};

#endif // MEDIAPLAYERDATA_H
