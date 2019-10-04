/******************************************************************************/
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser
// General Public License version 2.1 as published by the Free Software
// Foundation and appearing in the file LICENSE.LGPL included in the
// packaging of this file.  Please review the following information to
// ensure the GNU Lesser General Public License version 2.1 requirements
// will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
/******************************************************************************/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#if defined Q_OS_UNIX
#include <signal.h>
#endif

#include "global.h"
#include "usbmountpoint.h"
#include "mediaplayer.h"
#include "mediaplayerdata.h"
#include "equalizer.h"
#include "musicstand.h"
#include "updatehandler.h"
#include "plnet.h"
#include "powersupply.h"

DEFINE_LOG_MODULE_NAME;

#if defined Q_OS_UNIX
/******************************************************************************/
void signal_handler(int sig)
/******************************************************************************/
{
	switch (sig)
	{
		case SIGHUP:
			LOG_DEBUG("Application quit by SIGHUP");
			break;
		case SIGINT:
			LOG_DEBUG("Application quit by SIGINT");
			break;
		case SIGPIPE:
			LOG_DEBUG("Application quit by SIGPIPE");
			break;
		case SIGTERM:
			LOG_DEBUG("Application quit by SIGTERM");
			break;
		case SIGALRM:
			LOG_DEBUG("Application quit by SIGALRM");
			break;
		case SIGPOLL:
			LOG_DEBUG("Application quit by SIGPOLL");
			break;
		case SIGPROF:
			LOG_DEBUG("Application quit by SIGPROF");
			break;
		case SIGUSR1:
			LOG_DEBUG("Application quit by SIGUSR1");
			break;
		case SIGUSR2:
			LOG_DEBUG("Application quit by SIGUSR2");
			break;

		default:
			LOG_DEBUG("Application quit");

	}

	qApp->quit();
}
#endif

/******************************************************************************/
int main(int argc, char *argv[])
/******************************************************************************/
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication       app(argc, argv);
	QQmlApplicationEngine engine;
	int                   return_value;
	Logger::LOG_LEVEL     log_level;
	QString               path;
	QString               buffer;
	bool                  ok;
	Equalizer::EQ_ID      eq_id;

	app.setApplicationName("RESONANCE piano");
	app.setApplicationVersion(QString(APP_VERSION));

#if defined Q_OS_UNIX
	QSettings             ini("/home/pi/pianoLight/pianoLight.ini", QSettings::IniFormat);
#else
	QSettings             ini("pianoLight.ini", QSettings::IniFormat);
#endif

#if defined Q_OS_UNIX
	signal(SIGHUP,   signal_handler);
	signal(SIGINT,   signal_handler);
	signal(SIGPIPE,  signal_handler);
	signal(SIGTERM,  signal_handler);
	signal(SIGALRM,  signal_handler);
	signal(SIGPOLL,  signal_handler);
	signal(SIGPROF,  signal_handler);
	signal(SIGUSR1,  signal_handler);
	signal(SIGUSR1,  signal_handler);
#endif

	ini.beginGroup("Log");
	log_level = (Logger::LOG_LEVEL)ini.value("Level").toInt(&ok);
	if (!ok)
	{
		log_level = Logger::LOG;
		ini.setValue("Level", log_level);
		ini.sync();
	}
	else if (log_level > Logger::LOG_LEVELS_NUMBER)
	{
		log_level = Logger::LOG_DEBUG;
		ini.setValue("Level", log_level);
		ini.sync();
	}
	ini.endGroup();

	ini.beginGroup("Selection");
	eq_id = (Equalizer::EQ_ID)ini.value("Eq").toInt(&ok);
	if (!ok)
	{
		eq_id = Equalizer::EQ_ID::NO_EQ_ID;
		ini.setValue("Eq", eq_id);
		ini.sync();
	}
	else if (eq_id >= Equalizer::EQ_ID::EQ_ID_NUMBER)
	{
		eq_id = Equalizer::EQ_ID::NO_EQ_ID;
		ini.setValue("Eq", eq_id);
		ini.sync();
	}
	ini.endGroup();

	Logger::GetInstance().Initialize(log_level);
	PowerSupply::GetInstance().Initialize(17, 27);
	Equalizer::GetInstance().SetEqId(eq_id);

	buffer = QString("Application started  - %1 %2").arg(app.applicationName(), app.applicationVersion());
	LOG(buffer);

#if defined Q_OS_UNIX
	PlNet::GetInstance().Initialize("/dev/ttyAMA0");
	UsbMountPoint::GetInstance().Initialize(QLatin1String("/media/usb/"), QLatin1String("/media/ram_disk/usb_mounted.info"));
#else
	ini.beginGroup("Player");
	path = ini.value("Path").toString();

	if (path == "")
	{
		path = "I:/";
		ini.setValue("Path", path);
		ini.sync();
	}
	ini.endGroup();

	PlNet::GetInstance().Initialize("COM1");
	UsbMountPoint::GetInstance().Initialize(path, QLatin1String(""));
#endif

	QObject::connect(&PlNet::GetInstance(), SIGNAL(EqChanged(void)), &Equalizer::GetInstance(), SLOT(EqChanged(void)), Qt::QueuedConnection);

	QObject::connect(&UsbMountPoint::GetInstance(), SIGNAL(SetState(bool, QString &)), &MediaPlayer::GetInstance(), SLOT(UsbMountPoint(bool, QString &)), Qt::BlockingQueuedConnection);
	QObject::connect(&UsbMountPoint::GetInstance(), SIGNAL(SetState(bool, QString &)), &UpdateHandler::GetInstance(), SLOT(UsbMountPoint(bool, QString &)), Qt::BlockingQueuedConnection);

	engine.rootContext()->setContextProperty("media_player", &MediaPlayer::GetInstance());
	engine.rootContext()->setContextProperty("media_player_data", &MediaPlayer::GetInstance().GetMediaPlayerData());
	engine.rootContext()->setContextProperty("assert", &Assert::GetInstance());
	engine.rootContext()->setContextProperty("equalizer", &Equalizer::GetInstance());
	engine.rootContext()->setContextProperty("music_stand", &MusicStand::GetInstance());
	engine.rootContext()->setContextProperty("update_handler", &UpdateHandler::GetInstance());
	engine.rootContext()->setContextProperty("pl_net", &PlNet::GetInstance());


	engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

	return_value = app.exec();

	ini.beginGroup("Selection");
	ini.setValue("Eq", Equalizer::GetInstance().GetEqId());
	ini.sync();
	ini.endGroup();

	MediaPlayer::GetInstance().End();
	PlNet::GetInstance().End();
	UsbMountPoint::GetInstance().End();
	Logger::GetInstance().End();
	PowerSupply::GetInstance().End();

	LOG("Application stopped");

	return (return_value);
}
