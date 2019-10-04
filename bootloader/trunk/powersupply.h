#ifndef POWERSUPPLY_H
#define POWERSUPPLY_H

#include "global.h"
#include <QObject>
#include <QThread>
#include <QProcess>

class PowerSupply : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(PowerSupply)

	// Signals and Slots
public slots:
	// QML slots

	// Local slots
	void Thread();

signals:
	// QML signals

	// Local signals

	// Functions
public:
	static PowerSupply &GetInstance(void);

	~PowerSupply(void);
	void Initialize(int output);
	void End(void);

private:
	explicit PowerSupply(QObject *parent = nullptr);

	// Variables
private:
	QThread _Thread;
	int     _Output;
};

#endif // POWERSUPPLY_H
