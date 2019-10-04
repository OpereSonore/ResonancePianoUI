#ifndef MUSICSTAND_H
#define MUSICSTAND_H

#include "global.h"

#include <QObject>
#include <QTimer>
#include <QQmlApplicationEngine>

class MusicStand : public QObject
{
	Q_OBJECT

	Q_DISABLE_COPY(MusicStand)
	Q_PROPERTY(bool SwipeEnable  READ GetSwipeEnable NOTIFY swipeEnableChanged)

	typedef enum
	{
		TIMEOUT_EVENT_ID,
		FOCUS_ON_EVENT_ID,
		FOCUS_OFF_EVENT_ID,
		DOUBLE_CLICK_EVENT_ID,
	} EVENT_ID;

// Signals and Slots
public slots:
	// QML slots
	void onFocusChanged(bool focus);
	void onDoubleClickEvent(void);

	// Local slots
	void Timeout(void);

signals:
	// QML signals
	void swipeEnableChanged(void);

	// Local signals

// Functions
public:
	static MusicStand &GetInstance(void);

	~MusicStand(void);

	bool GetSwipeEnable(void);

private:
	explicit MusicStand(QObject *parent = nullptr);

	void Handler(EVENT_ID event_id);
	void SetBrightness(int brightness);

// Variables
private:
	bool         _Swipe_Enable;
	int          _State;
	QTimer       _Timer;
	int          _Brightness;
};

#endif // EQUALIZER_H
