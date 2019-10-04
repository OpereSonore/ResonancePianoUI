#ifndef UPDATEHANDLER_H
#define UPDATEHANDLER_H

#include "global.h"

#include "updatehandler.h"

class UpdateHandler : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(UpdateHandler)


// Signals and Slots
public slots:
	// QML slots
	void updateDialogResponse(bool ok);

	// Local slots
	void UsbMountPoint(bool enable, QString &root);


signals:
	// QML signals
	void openUpdateDialog(void);

	// Local signals

// Functions
public:
	static UpdateHandler &GetInstance(void);

	~UpdateHandler(void);
	void Enable(bool enable);

private:
	explicit UpdateHandler(QObject *parent = nullptr);

// Variables
private:
	bool        _Enable;
	QStringList _Filter;
	QString     _Update_File;
};

#endif // MEDIAPLAYER_H
