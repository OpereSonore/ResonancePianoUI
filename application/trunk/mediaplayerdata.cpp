

#include <QDir>

#include "mediaplayerdata.h"

/*****************************************************************************/

DEFINE_LOG_MODULE_NAME;

/*****************************************************************************/
MediaPlayerData::MediaPlayerData(QObject *parent) : QAbstractListModel(parent)
/*****************************************************************************/
{
	_Item_Playing = QFileInfo();
}

/*****************************************************************************/
int MediaPlayerData::rowCount(const QModelIndex &parent) const
/*****************************************************************************/
{
	Q_UNUSED(parent);

	return (_Items.size());
}

/*****************************************************************************/
QVariant MediaPlayerData::data(const QModelIndex &index, int role) const
/*****************************************************************************/
{
	int id;

	try
	{
		id	= index.row();

		if (id < 0 || id >= _Items.size())
		{
			return (QVariant());
		}

		switch (role)
		{
			case MODEL_ITEM_ICON_ID:
				if (_Item_Playing == _Items[id].info)
				{
					return (ITEM_PLAYING_TYPE);
				}
				else
				{
					ASSERT(_Items[id].type < ITEM_TYPE_NUMBER);

					return (_Items[id].type);
				}

			case MODEL_ITEM_NAME_ID:
				return (_Items[id].name);

			default:
				ASSERT(false);
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}

	return (QVariant());
}

/*****************************************************************************/
QHash<int, QByteArray> MediaPlayerData::roleNames() const
/*****************************************************************************/
{
	QHash<int, QByteArray> role_names = QAbstractListModel::roleNames();

	role_names[MODEL_ITEM_ICON_ID] = "icon";
	role_names[MODEL_ITEM_NAME_ID] = "name";

	return (role_names);
}

/*****************************************************************************/
void MediaPlayerData::SetRootPath(const QString &path)
/*****************************************************************************/
{
	_Root_Path = path;
}

/*****************************************************************************/
void MediaPlayerData::SetItems(const QFileInfoList &file_list)
/*****************************************************************************/
{
	QFileInfo file_info;
	QString file_name;
	MODEL_ITEM item;
	int id;
	int file_id;
	int folder_id;
	QVector<MODEL_ITEM> items;

	try
	{
		if (file_list.isEmpty())
		{
			items.clear();
		}
		else
		{
			file_id = 0;
			folder_id = 1;
			for (id = 0; id < file_list.size(); id++)
			{
				file_info = file_list.at(id);
				file_name = file_info.fileName();

				if (file_info.isDir())
				{
					if (file_name == "..")
					{
						if (file_info.path().startsWith(_Root_Path))
						{
							item.id   = 0;
							item.name = file_name;
							item.type = MediaPlayerData::ITEM_BACK_TYPE;
							item.info = file_info;
							items.push_front(item);
						}
					}
					else
					{
						item.id   = folder_id;
						item.name = file_name;
						item.type = MediaPlayerData::ITEM_FOLDER_TYPE;
						item.info = file_info;
						items.push_back(item);

						folder_id++;
					}
				}
				else
				{
					item.id   = file_id;
					item.name = file_name;
					item.type = MediaPlayerData::ITEM_MULTIMEDIA_FILE_TYPE;
					item.info = file_info;
					items.push_back(item);

					file_id++;
				}
			}
		}

		beginResetModel();

		_Items = items;

		endResetModel();
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}

}

/*****************************************************************************/
void MediaPlayerData::GetItem(int id, MODEL_ITEM *item)
/*****************************************************************************/
{
	ASSERT((id >= 0) && (id < _Items.size()));

	*item = _Items[id];
}

/*****************************************************************************/
void MediaPlayerData::SetItemPlaying(const QFileInfo &item_playing)
/*****************************************************************************/
{
	QVector<int> roles;

	_Item_Playing = item_playing;

	roles.push_back(MODEL_ITEM_ICON_ID);
	roles.push_back(Qt::DisplayRole);

	emit dataChanged(createIndex(0, 0), createIndex(_Items.size() - 1, 0), roles);
}
