/******************************************************************************/
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser
// General Public License version 2.1 as published by the Free Software
// Foundation and appearing in the file LICENSE.LGPL included in the
// packaging of this file.  Please review the following information to
// ensure the GNU Lesser General Public License version 2.1 requirements
// will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
/******************************************************************************/

#include "powersupply.h"
#include "gpio.h"

DEFINE_LOG_MODULE_NAME;

#define POWER_SUPPLY_FILTER       5

/******************************************************************************/
PowerSupply &PowerSupply::GetInstance(void)
/******************************************************************************/
{
	static PowerSupply instance;

	return (instance);
}

/*****************************************************************************/
PowerSupply::PowerSupply(QObject *parent) : QObject(parent)
/*****************************************************************************/
{
}

/*****************************************************************************/
PowerSupply::~PowerSupply(void)
/*****************************************************************************/
{
	End();
}

/*****************************************************************************/
void PowerSupply::Initialize(int output)
/*****************************************************************************/
{
	_Output = output;

	Gpio::GetInstance().Export(_Output);

	QThread::usleep(100000);

	Gpio::GetInstance().Direction(_Output, Gpio::OUT_DIR);

	QThread::usleep(100000);

	moveToThread(&_Thread);
	connect(&_Thread, SIGNAL(started()), this, SLOT(Thread()));
	_Thread.start();
}

/*****************************************************************************/
void PowerSupply::End(void)
/*****************************************************************************/
{
	_Thread.requestInterruption();
	_Thread.wait();

	Gpio::GetInstance().Unexport(_Output);
}

/*****************************************************************************/
void PowerSupply::Thread(void)
/*****************************************************************************/
{
	bool state;

	LOG("Thread started");

	state = false;
	while (!_Thread.isInterruptionRequested())
	{
		Gpio::GetInstance().Write(_Output, state);
		state = !state;

		QThread::usleep(100000);
	}

	LOG("Thread stopped");

	_Thread.quit();
}
