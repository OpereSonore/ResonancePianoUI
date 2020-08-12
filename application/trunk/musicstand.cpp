

#include "musicstand.h"
#include "updatehandler.h"

#include <QProcess>

DEFINE_LOG_MODULE_NAME;

#define FOCUS_ON_TIMEOUT           10000
#define DOUBLE_CLICK_TIMEOUT        2000
#define BRIGHTNESS_CHANGE_TIME         2

#define MAX_BRIGHTNESS               255
#define MIN_BRIGHTNESS                10
#define BRIGHTNESS_STEP               30

/******************************************************************************/
MusicStand &MusicStand::GetInstance(void)
/******************************************************************************/
{
	static MusicStand instance;

	return (instance);
}

/*****************************************************************************/
MusicStand::MusicStand(QObject *parent) : QObject(parent)
/*****************************************************************************/
{
	_Swipe_Enable = true;

	_State  = 0;

	QObject::connect(&_Timer, SIGNAL(timeout()), this, SLOT(Timeout()));

	_Brightness = MAX_BRIGHTNESS;
	SetBrightness(_Brightness);
}

/*****************************************************************************/
MusicStand::~MusicStand(void)
/*****************************************************************************/
{
}

/*****************************************************************************/
bool MusicStand::GetSwipeEnable(void)
/*****************************************************************************/
{
	return (_Swipe_Enable);
}

/*****************************************************************************/
void MusicStand::onFocusChanged(bool focus)
/*****************************************************************************/
{
	if (focus)
	{
		Handler(FOCUS_ON_EVENT_ID);
		UpdateHandler::GetInstance().Enable(false);
	}
	else
	{
		Handler(FOCUS_OFF_EVENT_ID);
		UpdateHandler::GetInstance().Enable(true);
	}
}

/*****************************************************************************/
void MusicStand::onDoubleClickEvent(void)
/*****************************************************************************/
{
	Handler(DOUBLE_CLICK_EVENT_ID);
}

/*****************************************************************************/
void MusicStand::Timeout(void)
/*****************************************************************************/
{
	Handler(TIMEOUT_EVENT_ID);
}

/*****************************************************************************/
void MusicStand::SetBrightness(int brightness)
/*****************************************************************************/
{
	QProcess process;
	QString command;

	if (brightness > MAX_BRIGHTNESS)
	{
		brightness = MAX_BRIGHTNESS;
	}
	else if (brightness < MIN_BRIGHTNESS)
	{
		brightness = MIN_BRIGHTNESS;
	}

	command = QString("/bin/bash -c \"echo %1 | sudo tee /sys/class/backlight/rpi_backlight/brightness\"").arg(brightness);

	process.start(command);
	process.waitForFinished();
}

/*****************************************************************************/
void  MusicStand::Handler(EVENT_ID event_id)
/*****************************************************************************/
{
	_Timer.stop();

	switch (_State)
	{
		case 0:
			switch (event_id)
			{
				case FOCUS_ON_EVENT_ID:
					_Timer.start(FOCUS_ON_TIMEOUT);
					_State = 1;
					break;

				default:
					ASSERT(false);
			}
			break;

		case 1:
			switch (event_id)
			{
				case FOCUS_OFF_EVENT_ID:
					_Brightness = MAX_BRIGHTNESS;
					SetBrightness(_Brightness);

					_State = 0;
					break;

				case TIMEOUT_EVENT_ID:
				case DOUBLE_CLICK_EVENT_ID:
					_Swipe_Enable = false;
					emit swipeEnableChanged();

					_Timer.start(BRIGHTNESS_CHANGE_TIME);
					_State = 2;
					break;

				default:
					ASSERT(false);
			}
			break;

		case 2:
			switch (event_id)
			{
				case FOCUS_OFF_EVENT_ID:
					_Brightness = MAX_BRIGHTNESS;
					SetBrightness(_Brightness);

					_Swipe_Enable = true;
					emit swipeEnableChanged();

					_State = 0;
					break;

				default:
					SetBrightness(_Brightness);
					if (_Brightness > MIN_BRIGHTNESS)
					{
						_Brightness -= BRIGHTNESS_STEP;
						_Timer.start(BRIGHTNESS_CHANGE_TIME);
					}
					else
					{
						_Brightness = MIN_BRIGHTNESS;

						_State = 3;
					}
					break;
			}
			break;

		case 3:
			switch (event_id)
			{
				case FOCUS_OFF_EVENT_ID:
					_Brightness = MAX_BRIGHTNESS;
					SetBrightness(_Brightness);

					_Swipe_Enable = true;
					emit swipeEnableChanged();

					_State = 0;
					break;

				case DOUBLE_CLICK_EVENT_ID:
					_Timer.start(BRIGHTNESS_CHANGE_TIME);

					_State = 4;
					break;

				default:
					ASSERT(false);
			}
			break;

		case 4:
			switch (event_id)
			{
				case FOCUS_OFF_EVENT_ID:
					_Brightness = MAX_BRIGHTNESS;
					SetBrightness(_Brightness);

					_Swipe_Enable = true;
					emit swipeEnableChanged();

					_State = 0;
					break;

				case DOUBLE_CLICK_EVENT_ID:
					_Timer.start(BRIGHTNESS_CHANGE_TIME);
					_State = 2;
					break;

				default:
					SetBrightness(_Brightness);
					if (_Brightness < MAX_BRIGHTNESS)
					{
						_Brightness += BRIGHTNESS_STEP;
						_Timer.start(BRIGHTNESS_CHANGE_TIME);
					}
					else
					{
						_Swipe_Enable = true;
						emit swipeEnableChanged();

						_Brightness = MAX_BRIGHTNESS;

						_Timer.start(DOUBLE_CLICK_TIMEOUT);
						_State = 5;
					}
					break;
			}
			break;

		case 5:
			switch (event_id)
			{
				case FOCUS_OFF_EVENT_ID:
					_State = 0;
					break;

				case DOUBLE_CLICK_EVENT_ID:
				case TIMEOUT_EVENT_ID:
					_Swipe_Enable = false;
					emit swipeEnableChanged();

					_Timer.start(BRIGHTNESS_CHANGE_TIME);
					_State = 2;
					break;

				default:
					ASSERT(false);
			}
			break;

		default:
			ASSERT(false);
	}
}
