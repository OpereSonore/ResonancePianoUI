#ifndef UPDATEHANDLER_H
#define UPDATEHANDLER_H

#include "global.h"

#include "updatehandler.h"

class UpdateHandler : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(UpdateHandler)

	Q_PROPERTY(QString StadndardOutput      READ GetStandardOutput          NOTIFY standardOutputChanged)

	Q_PROPERTY(bool    RebootEnable         MEMBER _RebootButtonEnable      NOTIFY rebootButtonChanged)
	Q_PROPERTY(bool    CancelEnable         MEMBER _CancelButtonEnable      NOTIFY cancelButtonChanged)
	Q_PROPERTY(bool    UpdateEnable         MEMBER _UpdateButtonEnable      NOTIFY updateButtonChanged)
	Q_PROPERTY(bool    ScrollEnable         MEMBER _ScrollEnable)

#define	NO_BUTTON_ENABLE             0
#define	REBOOT_BUTTON_ENABLE         (1 << 0)
#define	CANCEL_BUTTON_ENABLE         (1 << 1)
#define	UPDATE_BUTTON_ENABLE         (1 << 2)

// Signals and Slots
public slots:
	// QML slots
	void cancelButtonClick(void);
	void updateButtonClick(void);
	void rebootButtonClick(void);

	// Local slots
	void ReadStandardOutput(void);
	void ReadStandardError(void);

signals:
	// QML signals
	void standardOutputChanged(void);
	void rebootButtonChanged(void);
	void cancelButtonChanged(void);
	void updateButtonChanged(void);

	// Local signals

// Functions
public:
	static UpdateHandler &GetInstance(void);

	~UpdateHandler(void);

	void Initialize(void);
	QString GetStdOutput(void);

private:
	explicit UpdateHandler(QObject *parent = nullptr);

	void ClearStandardOutput(void);
	void AppendStandardOutput(QString &buffer);
	void SetStandardOutput(QString &buffer);
	QString GetStandardOutput(void);
	void ExecuteCommand (QString &command);
	void SetButtonEnable(int enable);

// Variables
private:
	QString     _Standard_Output;
	bool        _Ok;

	bool        _RebootButtonEnable;
	bool        _CancelButtonEnable;
	bool        _UpdateButtonEnable;
	bool        _ScrollEnable;
};

#endif // MEDIAPLAYER_H
