/******************************************************************************/
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser
// General Public License version 2.1 as published by the Free Software
// Foundation and appearing in the file LICENSE.LGPL included in the
// packaging of this file.  Please review the following information to
// ensure the GNU Lesser General Public License version 2.1 requirements
// will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
/******************************************************************************/

#include "updatehandler.h"

#include <QProcess>

DEFINE_LOG_MODULE_NAME;

#define UPDATE_FILE_NAME          APP_MODEL_NAME ".tar.gz"

/******************************************************************************/
UpdateHandler &UpdateHandler::GetInstance(void)
/******************************************************************************/
{
	static UpdateHandler instance;

	return (instance);
}

/*****************************************************************************/
UpdateHandler::UpdateHandler(QObject *parent) : QObject(parent)
/*****************************************************************************/
{
	_Filter.append(UPDATE_FILE_NAME);

	_Enable = true;
}

/*****************************************************************************/
UpdateHandler::~UpdateHandler(void)
/*****************************************************************************/
{
}

/*****************************************************************************/
void UpdateHandler::Enable(bool enable)
/*****************************************************************************/
{
	_Enable = enable;
}

/*****************************************************************************/
void UpdateHandler::UsbMountPoint(bool enable, QString &root)
/*****************************************************************************/
{
	QDir          dir;
	QFileInfoList file_list;

	try
	{
		if (enable && _Enable)
		{
			dir.setPath(root);

			dir.setNameFilters(_Filter);

			file_list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst);
			if (file_list.count() == 1)
			{
				_Update_File = file_list[0].absoluteFilePath();

				emit openUpdateDialog();
			}
		}
		else
		{
			_Update_File = QString();
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}
}

/*****************************************************************************/
void UpdateHandler::updateDialogResponse(bool ok)
/*****************************************************************************/
{
	QString destination_path;
	QString destination_file;
	QString command;

	try
	{
		if (ok && QFileInfo::exists(_Update_File))
		{
			LOG("Update in progress");

			destination_path = QCoreApplication::applicationDirPath() + "/";
			if (!QDir(destination_path).exists())
			{
				QDir().mkdir(destination_path);
			}
			destination_file = destination_path + UPDATE_FILE_NAME;
			QFile::copy(_Update_File, destination_file);

			qApp->quit();
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}
}
