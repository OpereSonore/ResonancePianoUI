#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QtMultimedia>
#include <QSharedPointer>

#include "mediaplayerdata.h"

class MediaPlayer : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(MediaPlayer)
	Q_PROPERTY(double  MediaProgress         READ GetMediaProgress WRITE SetMediaProgress NOTIFY mediaProgressChanged)
	Q_PROPERTY(QString MediaPosition         READ GetMediaPosition                        NOTIFY mediaPositionChanged)
	Q_PROPERTY(QString MediaDuration         READ GetMediaDuration                        NOTIFY mediaDurationChanged)
	Q_PROPERTY(QString MediaInfo             READ GetMediaInfo                            NOTIFY mediaInfoChanged)
	Q_PROPERTY(bool    MediaDisconnected     READ GetMediaDisconnected                    NOTIFY mediaDisconnectedChanged)
	Q_PROPERTY(bool    IsPlaying             READ GetIsPlaying                            NOTIFY isPlayingChanged)
	Q_PROPERTY(int     PlayIcon              READ GetPlayIcon                             NOTIFY playIconChanged)
	Q_PROPERTY(int     ShuffleIcon           READ GetShuffleIcon                          NOTIFY shuffleIconChanged)
	Q_PROPERTY(int     RepeatIcon            READ GetRepeatIcon                           NOTIFY repeatIconChanged)

	typedef enum
	{
		PLAYER_PAUSE_STATUS,
		PLAYER_PLAY_STATUS,

		PLAYER_STATUS_NUMBER
	} PLAYER_STATUS;

	typedef enum
	{
		SHUFFLE_OFF_STATUS,
		SHUFFLE_ON_STATUS,

		SHUFFLE_STATUS_NUMBER
	} SHUFFLE_STATUS;

	typedef enum
	{
		REPEAT_OFF_STATUS,
		REPEAT_LOOP_STATUS,
		REPEAT_CURRENT_ITEM_IN_LOOP_STATUS,

		REPEAT_STATUS_NUMBER
	} REPEAT_STATUS;

// Signals and Slots
public slots:
	// QML slots
	void onShuffleClicked(void);
	void onPreviousClicked(void);
	void onPlayClicked(void);
	void onNextClicked(void);
	void onMediaPlayerListViewChanged(int id);
	void onRepeatClicked(void);
	void onMediaProgressPressed(const bool &pressed);

	// Local slots
	void DurationChanged(qint64 duration);
	void PositionChanged(qint64 position);
	void MetaDataChanged();
	void PlaylistMediaChanged(const QMediaContent &media);
	void StatusChanged(QMediaPlayer::MediaStatus status);
	void DisplayErrorMessage();
	void UsbMountPoint(bool enable, QString &root);


signals:
	// QML signals
	void mediaProgressChanged(void);
	void resetMediaList(void);
	void mediaPositionChanged(void);
	void mediaDurationChanged(void);
	void mediaInfoChanged(void);
	void mediaDisconnectedChanged(void);
	void isPlayingChanged(void);
	void playIconChanged(void);
	void shuffleIconChanged(void);
	void repeatIconChanged(void);

	// Local signals

// Functions
public:
	static MediaPlayer &GetInstance(void);

	MediaPlayerData &GetMediaPlayerData(void);

	~MediaPlayer(void);

	double GetMediaProgress(void);
	void SetMediaProgress(double &media_progress);

	QString GetMediaPosition(void);
	QString GetMediaDuration(void);
	QString GetMediaInfo(void);
	bool GetMediaDisconnected(void);
	bool GetIsPlaying(void);
	int GetPlayIcon(void);
	int GetShuffleIcon(void);
	int GetRepeatIcon(void);

	void End(void);

private:
	explicit MediaPlayer(QObject *parent = nullptr);

	void SetState(bool enable);
	void SetPlaylist(int id);

	void Play();
	void Pause();
	void Stop();

	QMediaPlaylist::PlaybackMode GetPlaybackMode(void);

// Variables
private:
	double                               _Media_Progress;
	uint                                 _Media_Position;
	uint                                 _Media_Duration;
	QString                              _Media_Artist;
	QString                              _Media_Album;
	QString                              _Media_Title;
	QString                              _Media_Info;
	int                                  _Media_Info_Counter;
	bool                                 _Media_Disconnected;
	bool                                 _Media_Progress_Pressed;
	PLAYER_STATUS                        _Player_Status;
	SHUFFLE_STATUS                       _Shuffle_Status;
	REPEAT_STATUS                        _Repeat_Status;
	MediaPlayerData                      _Media_Player_Data;
	QStringList                          _Media_Filter;

	int                                  _Usb_Storage_State;
	QTimer                               _Timer;

	QSharedPointer<QMediaPlayer>         _Player;
	QSharedPointer<QMediaPlaylist>       _Play_List;
	QString                              _Play_List_Path;
	QString                              _Current_Path;
};

#endif // MEDIAPLAYER_H
