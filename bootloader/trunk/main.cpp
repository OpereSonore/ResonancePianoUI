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
#include "updatehandler.h"
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
	QProcess process;
	QString  buffer;

	QGuiApplication       app(argc, argv);
	QQmlApplicationEngine engine;
	int                   return_value;
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

	Logger::GetInstance().Initialize(Logger::LOG_DEBUG);
	PowerSupply::GetInstance().Initialize(27);

	buffer = QString("Application started  - %1").arg(APP_MODEL_NAME);
	LOG(buffer);

	engine.rootContext()->setContextProperty("update_handler", &UpdateHandler::GetInstance());

	engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

	UpdateHandler::GetInstance().Initialize();

	return_value = app.exec();

	PowerSupply::GetInstance().End();
	Logger::GetInstance().End();

	LOG("Application stopped");

	process.start("sudo halt");
	process.waitForFinished();

	return (return_value);
}
