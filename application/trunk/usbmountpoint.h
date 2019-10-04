#ifndef USBMOUNTPOINT_H
#define USBMOUNTPOINT_H

#include "global.h"
#include <QObject>
#include <QThread>
#include <QProcess>

class UsbMountPoint : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(UsbMountPoint)

	// Signals and Slots
public slots:
	// QML slots

	// Local slots
	void Thread();

signals:
	// QML signals

	// Local signals
	void SetState(bool enable, QString &root);

	// Functions
public:
	static UsbMountPoint &GetInstance(void);

	~UsbMountPoint(void);
	void Initialize(const QString &media_path, const QString &mount_info_file);
	QString &GetMediaRoot(void);
	void End(void);

private:
	explicit UsbMountPoint(QObject *parent = nullptr);
	bool MediaMounted(QDir &dir);
	bool MediaUnmounted(QDir &dir);

	// Variables
private:
	QThread _Thread;
	QString _Media_Path;
	QString _Mount_Info_File;
	int     _Usb_State;
};

#endif // USBMOUNTPOINT_H
