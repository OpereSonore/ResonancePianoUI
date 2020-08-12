

#include "mediaplayer.h"

DEFINE_LOG_MODULE_NAME;

#define MEDIA_DATA_STRING_LENGTH         38
#define MEDIA_INFO_SWITCH                5

/******************************************************************************/
MediaPlayer &MediaPlayer::GetInstance(void)
/******************************************************************************/
{
	static MediaPlayer instance;

	return (instance);
}

/*****************************************************************************/
MediaPlayer::MediaPlayer(QObject *parent) : QObject(parent)
/*****************************************************************************/
{
	_Media_Progress     = 0.0;
	_Media_Duration     = 0;
	_Media_Position     = 0;
	_Media_Info_Counter = 0;

	_Media_Filter.append("*.mp3");
	_Media_Filter.append("*.wav");
	_Media_Filter.append("*.flac");
	_Media_Filter.append("*.ogg");
	_Media_Filter.append("*.m4a");
	_Media_Filter.append("*.aiff");
	_Media_Filter.append("*.aif");
	_Media_Filter.append("*.aifc");

	_Media_Disconnected = true;
	_Media_Progress_Pressed = false;

	_Player_Status  = PLAYER_PAUSE_STATUS;
	_Shuffle_Status = SHUFFLE_OFF_STATUS;
	_Repeat_Status  = REPEAT_OFF_STATUS;
}

/*****************************************************************************/
MediaPlayer::~MediaPlayer(void)
/*****************************************************************************/
{
	if (_Player != NULL)
	{
		Stop();
		_Player->disconnect();
		_Player.clear();
	}
}

/*****************************************************************************/
MediaPlayerData &MediaPlayer::GetMediaPlayerData(void)
/*****************************************************************************/
{
	return (_Media_Player_Data);
}


/*****************************************************************************/
void MediaPlayer::Play(void)
/*****************************************************************************/
{
	if (_Player == NULL)
	{
		_Player_Status = PLAYER_PAUSE_STATUS;
	}
	else
	{
		ASSERT (_Play_List != NULL);

		_Player_Status = PLAYER_PLAY_STATUS;
		_Play_List->setPlaybackMode(GetPlaybackMode());
		_Player->play();
	}

	emit playIconChanged();
	emit isPlayingChanged();
}

/*****************************************************************************/
void MediaPlayer::Pause(void)
/*****************************************************************************/
{
	if (_Player != NULL)
	{
		_Player->pause();
	}

	_Player_Status = PLAYER_PAUSE_STATUS;

	emit playIconChanged();
	emit isPlayingChanged();
}

/*****************************************************************************/
void MediaPlayer::Stop(void)
/*****************************************************************************/
{
	if (_Player != NULL)
	{
		_Player->stop();
	}

	_Player_Status = PLAYER_PAUSE_STATUS;

	emit playIconChanged();
	emit isPlayingChanged();
}

/*****************************************************************************/
void MediaPlayer::onShuffleClicked(void)
/*****************************************************************************/
{
	_Shuffle_Status = (_Shuffle_Status == SHUFFLE_OFF_STATUS) ? SHUFFLE_ON_STATUS : SHUFFLE_OFF_STATUS;
	shuffleIconChanged();

	if (_Play_List != NULL)
	{
		_Play_List->setPlaybackMode(GetPlaybackMode());
	}
}

/*****************************************************************************/
void MediaPlayer::onPreviousClicked(void)
/*****************************************************************************/
{
	if ((_Play_List != NULL) && _Player_Status)
	{
		_Play_List->previous();
	}
}

/*****************************************************************************/
void MediaPlayer::onPlayClicked(void)
/*****************************************************************************/
{
	QString path;

	if (_Player == NULL)
	{
		SetPlaylist(0);
	}
	else
	{
		if (_Player->state() == QMediaPlayer::PlayingState)
		{
			_Media_Player_Data.SetItemPlaying(QFileInfo());
			Pause();
		}
		else
		{
			ASSERT(_Play_List != NULL);

			path = _Play_List->currentMedia().canonicalUrl().toString();
			path.replace("file:///", "");

			_Media_Player_Data.SetItemPlaying(QFileInfo(path));
			Play();
		}
	}
}

/*****************************************************************************/
void MediaPlayer::onNextClicked(void)
/*****************************************************************************/
{
	if ((_Play_List != NULL) && _Player_Status)
	{
		_Play_List->next();
	}
}

/*****************************************************************************/
void MediaPlayer::onRepeatClicked(void)
/*****************************************************************************/
{
	_Repeat_Status = (REPEAT_STATUS)((int)_Repeat_Status + 1);
	if ((int)_Repeat_Status >= (int)REPEAT_STATUS_NUMBER)
	{
		_Repeat_Status = (REPEAT_STATUS)0;
	}

	repeatIconChanged();

	if (_Play_List != NULL)
	{
		_Play_List->setPlaybackMode(GetPlaybackMode());
	}
}

/*****************************************************************************/
void MediaPlayer::onMediaProgressPressed(const bool &pressed)
/*****************************************************************************/
{
	_Media_Progress_Pressed = pressed;
}

/*****************************************************************************/
void MediaPlayer::SetPlaylist(int id)
/*****************************************************************************/
{
	QDir          dir;
	QFileInfoList file_list;
	int           i;

	dir.setPath(_Current_Path);
	dir.setNameFilters(_Media_Filter);

	file_list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst);

	if (file_list.size() > 0)
	{
		_Play_List_Path = dir.path() + "/";

		if ((_Play_List == NULL) && (_Player == NULL))
		{
			_Player = QSharedPointer<QMediaPlayer>(new QMediaPlayer());
			_Play_List = QSharedPointer<QMediaPlaylist>(new QMediaPlaylist());

			_Player->setPlaylist(_Play_List.data());

			connect(_Player.data(),    SIGNAL(durationChanged(qint64)),                       this, SLOT(DurationChanged(qint64)));
			connect(_Player.data(),    SIGNAL(positionChanged(qint64)),                       this, SLOT(PositionChanged(qint64)));
			connect(_Player.data(),    SIGNAL(metaDataChanged()),                             this, SLOT(MetaDataChanged()));
			connect(_Play_List.data(), SIGNAL(currentMediaChanged(const QMediaContent &)),    this, SLOT(PlaylistMediaChanged(const QMediaContent &)));
			connect(_Player.data(),    SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(StatusChanged(QMediaPlayer::MediaStatus)));
			connect(_Player.data(),    SIGNAL(error(QMediaPlayer::Error)),                    this, SLOT(DisplayErrorMessage()));
		}
		else if ((_Play_List == NULL) || (_Player == NULL))
		{
			ASSERT(false);
		}

		_Play_List->clear();
		for (i = 0; i < file_list.size(); i++)
		{
			_Play_List->addMedia(QUrl("file:///" + file_list.at(i).absoluteFilePath()));
		}

		_Play_List->setCurrentIndex(id);

		_Media_Player_Data.SetItemPlaying(QFileInfo(file_list.at(id).absoluteFilePath()));

		_Play_List->setPlaybackMode(GetPlaybackMode());
		Play();
	}
}

/*****************************************************************************/
void MediaPlayer::onMediaPlayerListViewChanged(int id)
/*****************************************************************************/
{
	QDir                        dir;
	QFileInfoList               file_list;
	MediaPlayerData::MODEL_ITEM item;

	_Media_Player_Data.GetItem(id, &item);

	try
	{
		switch (item.type)
		{
			case MediaPlayerData::ITEM_BACK_TYPE:
			case MediaPlayerData::ITEM_FOLDER_TYPE:
				_Current_Path = item.info.absoluteFilePath();

				dir.setPath(_Current_Path);
				dir.setNameFilters(_Media_Filter);

				file_list = dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::NoDot, QDir::DirsFirst);
				_Media_Player_Data.SetItems(file_list);

				emit resetMediaList();
				break;

			case MediaPlayerData::ITEM_MULTIMEDIA_FILE_TYPE:
				if ((_Play_List_Path == _Current_Path) && (_Play_List != NULL) && (_Player != NULL))
				{
					if (_Play_List->currentIndex() == item.id)
					{
						if (_Player->state() == QMediaPlayer::PlayingState)
						{
							_Media_Player_Data.SetItemPlaying(QFileInfo());
							Pause();
						}
						else
						{
							_Media_Player_Data.SetItemPlaying(item.info);
							_Play_List->setPlaybackMode(GetPlaybackMode());
							Play();
						}
					}
					else
					{
						_Play_List->setCurrentIndex(item.id);
						_Media_Player_Data.SetItemPlaying(item.info);
						_Play_List->setPlaybackMode(GetPlaybackMode());
						Play();
					}
				}
				else
				{
					SetPlaylist(item.id);
				}
				break;

			default:
				ASSERT(false);
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}
}

/*****************************************************************************/
void MediaPlayer::UsbMountPoint(bool enable, QString &root)
/*****************************************************************************/
{
	QDir          dir;
	QFileInfoList file_list;

	try
	{
		dir.setPath(root);
		dir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

		if (enable)
		{
			_Media_Player_Data.SetRootPath(root);
			_Current_Path = root;

			dir.setNameFilters(_Media_Filter);

			file_list = dir.entryInfoList(QDir::AllDirs | QDir::AllEntries | QDir::NoDot, QDir::DirsFirst);
			_Media_Player_Data.SetItems(file_list);

			_Media_Disconnected = false;
			emit mediaDisconnectedChanged();
		}
		else
		{
			if (_Player != NULL)
			{
				LOG("Stopping player");
				Stop();

				LOG("Disconnecting player");
				_Player->disconnect();
				if (_Play_List != NULL)
				{
					LOG("Clearinging player");
					_Play_List.clear();
				}
				_Player.clear();
			}

			_Media_Player_Data.SetItems(QFileInfoList());
			_Media_Player_Data.SetItemPlaying(QFileInfo());

			_Media_Disconnected = true;

			emit resetMediaList();

			emit mediaDisconnectedChanged();
			emit mediaInfoChanged();
			emit mediaDurationChanged();
			emit mediaPositionChanged();

			LOG("Player disabled");
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}
}

/*****************************************************************************/
void MediaPlayer::End(void)
/*****************************************************************************/
{
	if (_Player != NULL)
	{
		LOG("Stopping player");
		Stop();

		LOG("Disconnecting player");
		_Player->disconnect();
		if (_Play_List != NULL)
		{
			LOG("Clearinging player");
			_Play_List.clear();
		}
		_Player.clear();
	}
}

/*****************************************************************************/
void MediaPlayer::DurationChanged(qint64 duration)
/*****************************************************************************/
{
	_Media_Duration = (duration / 1000);

	emit mediaDurationChanged();
}

/*****************************************************************************/
void MediaPlayer::PositionChanged(qint64 position)
/*****************************************************************************/
{
	_Media_Position = (position / 1000);

	if ((_Media_Duration > 0) && !_Media_Progress_Pressed)
	{
		_Media_Progress = ((double)_Media_Position / (double)_Media_Duration);

		emit mediaProgressChanged();
	}

	if (_Media_Position == 0)
	{
		_Media_Info_Counter = 0;
	}
	else
	{
		_Media_Info_Counter++;
		switch (_Media_Info_Counter)
		{
			case 1:
				_Media_Info = _Media_Title;
				emit mediaInfoChanged();
				break;

			case MEDIA_INFO_SWITCH + 1:
				_Media_Info = _Media_Album;
				emit mediaInfoChanged();
				break;

			case (MEDIA_INFO_SWITCH * 2) + 1:
				_Media_Info = _Media_Artist;
				emit mediaInfoChanged();
				break;

			case (MEDIA_INFO_SWITCH * 3):
				_Media_Info_Counter = 0;
				break;

			default:
				break;
		}
	}

	emit mediaPositionChanged();
}

/*****************************************************************************/
void MediaPlayer::MetaDataChanged(void)
/*****************************************************************************/
{
	if (_Player == NULL)
	{
		return;
	}

	try
	{
		if (_Player->isMetaDataAvailable())
		{
			_Media_Artist = _Player->metaData(QMediaMetaData::AlbumArtist).toString();
			if (_Media_Artist == "")
			{
				_Media_Artist = _Player->metaData(QMediaMetaData::ContributingArtist).toString();
				if (_Media_Artist == "")
				{
					_Media_Artist = "Unknown Artist";
				}
			}

			_Media_Album  = _Player->metaData(QMediaMetaData::AlbumTitle).toString();
			if (_Media_Album == "")
			{
				_Media_Album  = "Unknown Album";
			}

			_Media_Title  = _Player->metaData(QMediaMetaData::Title).toString();
			if (_Media_Title == "")
			{
				_Media_Title  = "Unknown Title";
			}
		}
		else
		{
			_Media_Artist = "Unknown Artist";
			_Media_Album  = "Unknown Album";
			_Media_Title  = "Unknown Title";
		}

		if (_Media_Artist.length() > MEDIA_DATA_STRING_LENGTH)
		{
			_Media_Artist = _Media_Artist.left(MEDIA_DATA_STRING_LENGTH - 3) + "...";
		}
		if (_Media_Album.length() > MEDIA_DATA_STRING_LENGTH)
		{
			_Media_Album = _Media_Album.left(MEDIA_DATA_STRING_LENGTH - 3) + "...";
		}
		if (_Media_Title.length() > MEDIA_DATA_STRING_LENGTH)
		{
			_Media_Title = _Media_Title.left(MEDIA_DATA_STRING_LENGTH - 3) + "...";
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}
}

/*****************************************************************************/
void MediaPlayer::PlaylistMediaChanged(const QMediaContent &media)
/*****************************************************************************/
{
	QString path;

	try
	{
		if (_Player_Status)
		{
			path = media.canonicalUrl().toString();
			path.replace("file:///", "");
			_Media_Player_Data.SetItemPlaying(QFileInfo(path));
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}
}

/*****************************************************************************/
void MediaPlayer::StatusChanged(QMediaPlayer::MediaStatus status)
/*****************************************************************************/
{
	try
	{
		switch (status)
		{
			case QMediaPlayer::UnknownMediaStatus:
			case QMediaPlayer::LoadedMedia:
			case QMediaPlayer::BufferingMedia:
			case QMediaPlayer::BufferedMedia:
				break;
			case QMediaPlayer::LoadingMedia:
				break;
			case QMediaPlayer::StalledMedia:
				break;
			case QMediaPlayer::EndOfMedia:
			case QMediaPlayer::NoMedia:
				_Media_Player_Data.SetItemPlaying(QFileInfo());
				Stop();
				break;
			case QMediaPlayer::InvalidMedia:
				break;
			default:
				ASSERT(false);
				break;
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}
}

/*****************************************************************************/
void MediaPlayer::DisplayErrorMessage(void)
/*****************************************************************************/
{

}

/*****************************************************************************/
double MediaPlayer::GetMediaProgress(void)
/*****************************************************************************/
{
	if (_Player == NULL)
	{
		return (0);
	}

	return (_Media_Progress);
}

/*****************************************************************************/
void MediaPlayer::SetMediaProgress(double &media_progress)
/*****************************************************************************/
{
	try
	{
		if (_Player == NULL)
		{
			return;
		}

		if (media_progress == _Media_Progress)
		{
			return;
		}

		_Media_Progress = media_progress;

		if (_Media_Duration > 0)
		{
			_Player->setPosition((qint64)((double)(_Media_Duration * 1000) * _Media_Progress));
		}

		emit mediaProgressChanged();
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}
}

/*****************************************************************************/
QString MediaPlayer::GetMediaPosition(void)
/*****************************************************************************/
{
	try
	{
		if (_Player == NULL)
		{
			return ("-");
		}
		else
		{
			QTime time((_Media_Position / 3600) % 60, (_Media_Position / 60) % 60, _Media_Position % 60, (_Media_Position * 1000) % 1000);
			if (_Media_Position > 3600)
			{
				return (time.toString("hh:mm:ss"));
			}
			else
			{
				return (time.toString("mm:ss"));
			}
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}

	return (QString());
}

/*****************************************************************************/
QString MediaPlayer::GetMediaDuration(void)
/*****************************************************************************/
{
	try
	{
		if (_Player == NULL)
		{
			return ("-");
		}
		else
		{
			QTime time((_Media_Duration / 3600) % 60, (_Media_Duration / 60) % 60, _Media_Duration % 60, (_Media_Duration * 1000) % 1000);
			if (_Media_Duration > 3600)
			{
				return (time.toString("hh:mm:ss"));
			}
			else
			{
				return (time.toString("mm:ss"));
			}
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}

	return (QString());
}

/*****************************************************************************/
QString MediaPlayer::GetMediaInfo(void)
/*****************************************************************************/
{
	if (_Player == NULL)
	{
		return ("-");
	}

	return (_Media_Info);
}

/*****************************************************************************/
bool MediaPlayer::GetMediaDisconnected(void)
/*****************************************************************************/
{
	return (_Media_Disconnected);
}

/*****************************************************************************/
bool MediaPlayer::GetIsPlaying(void)
/*****************************************************************************/
{
	return (_Player_Status);
}

/*****************************************************************************/
int MediaPlayer::GetPlayIcon(void)
/*****************************************************************************/
{
	ASSERT(_Player_Status < PLAYER_STATUS_NUMBER);

	return (_Player_Status);
}

/*****************************************************************************/
int MediaPlayer::GetShuffleIcon(void)
/*****************************************************************************/
{
	ASSERT(_Shuffle_Status < SHUFFLE_STATUS_NUMBER);

	return (_Shuffle_Status);
}

/*****************************************************************************/
int MediaPlayer::GetRepeatIcon(void)
/*****************************************************************************/
{
	ASSERT(_Repeat_Status < REPEAT_STATUS_NUMBER);

	return (_Repeat_Status);
}

/*****************************************************************************/
QMediaPlaylist::PlaybackMode MediaPlayer::GetPlaybackMode(void)
/*****************************************************************************/
{
	if (_Shuffle_Status)
	{
		return (QMediaPlaylist::Random);
	}
	else
	{
		switch (_Repeat_Status)
		{
			case REPEAT_OFF_STATUS:
				return (QMediaPlaylist::Sequential);
				break;

			case REPEAT_CURRENT_ITEM_IN_LOOP_STATUS:
				return (QMediaPlaylist::CurrentItemInLoop);
				break;

			case REPEAT_LOOP_STATUS:
				return (QMediaPlaylist::Loop);
				break;

			default:
				ASSERT(false);
		}
	}

	ASSERT(false);
	return (QMediaPlaylist::Sequential);
}
