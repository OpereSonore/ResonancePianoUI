/******************************************************************************/
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser
// General Public License version 2.1 as published by the Free Software
// Foundation and appearing in the file LICENSE.LGPL included in the
// packaging of this file.  Please review the following information to
// ensure the GNU Lesser General Public License version 2.1 requirements
// will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
/******************************************************************************/

#include "usbmountpoint.h"

DEFINE_LOG_MODULE_NAME;

/******************************************************************************/
UsbMountPoint &UsbMountPoint::GetInstance(void)
/******************************************************************************/
{
	static UsbMountPoint instance;

	return (instance);
}

/*****************************************************************************/
UsbMountPoint::UsbMountPoint(QObject *parent) : QObject(parent)
/*****************************************************************************/
{
	_Usb_State = 0;

	moveToThread(&_Thread);
	connect(&_Thread, SIGNAL(started()), this, SLOT(Thread()));
	_Thread.start();

}

/*****************************************************************************/
UsbMountPoint::~UsbMountPoint(void)
/*****************************************************************************/
{
}

/*****************************************************************************/
void UsbMountPoint::End(void)
/*****************************************************************************/
{
	_Thread.requestInterruption();
	// Non si attende il termine del thread perchè in fase di
	// spengnimento S.O. potrebbe bloccare l'esecuzione dei comandi di unmount
}

/*****************************************************************************/
void UsbMountPoint::Initialize(const QString &media_path, const QString &mount_info_file)
/*****************************************************************************/
{
	_Media_Path = media_path;
	_Mount_Info_File = mount_info_file;
}

/*****************************************************************************/
bool UsbMountPoint::MediaMounted(QDir &dir)
/*****************************************************************************/
{
	if (_Mount_Info_File == "")
	{
		return (dir.count() > 0);
	}

	QFile file(_Mount_Info_File);

	return (file.exists() && dir.exists());
}

/*****************************************************************************/
bool UsbMountPoint::MediaUnmounted(QDir &dir)
/*****************************************************************************/
{
	if (_Mount_Info_File == "")
	{
		return (dir.count() == 0);
	}

	QFile file(_Mount_Info_File);

	return (!file.exists());
}

/*****************************************************************************/
void UsbMountPoint::Thread(void)
/*****************************************************************************/
{
	QProcess process;

	LOG("Thread started");

	while (!_Thread.isInterruptionRequested())
	{
		QDir dir;

		dir.setPath(_Media_Path);
		dir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

		switch (_Usb_State)
		{
			case 0:
				try
				{
					if (MediaUnmounted(dir))
					{
						process.start("sudo umount -l " + _Media_Path);
						process.waitForFinished();
						process.start("sudo rmdir " + _Media_Path);
						process.waitForFinished();

						LOG("Unmount at start");
					}

					_Usb_State = 1;
				}
				catch (std::exception &e)
				{
					ASSERT_EXCEPTION(e);
				}
				break;

			case 1:
				try
				{
					if (MediaMounted(dir))
					{
						LOG("Usb plugged");

						emit SetState(true, _Media_Path);

						LOG("Usb mounted");

						_Usb_State = 2;
					}
				}
				catch (std::exception &e)
				{
					ASSERT_EXCEPTION(e);
				}
				break;

			case 2:
				try
				{
					if (MediaUnmounted(dir))
					{
						LOG("Usb unplugged");

						emit SetState(false, _Media_Path);

						QThread::usleep(2000000);

						process.start("sudo umount -l " + _Media_Path);
						process.waitForFinished();
						process.start("sudo rmdir " + _Media_Path);
						process.waitForFinished();

						LOG("Usb unmount");

						_Usb_State = 1;
					}
				}
				catch (std::exception &e)
				{
					ASSERT_EXCEPTION(e);
				}
				break;

			default:
				ASSERT(false);
		}

		QThread::usleep(100000);
	}

	LOG("Thread stopped");

	_Thread.quit();
}
