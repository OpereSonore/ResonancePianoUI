

#include "gpio.h"

/******************************************************************************/
Gpio &Gpio::GetInstance(void)
/******************************************************************************/
{
	static Gpio instance;

	return (instance);
}

/*****************************************************************************/
Gpio::~Gpio(void)
/*****************************************************************************/
{
}

/*****************************************************************************/
bool Gpio::Export(int pin)
/*****************************************************************************/
{
#if defined Q_OS_UNIX
	char buffer[32 + 1];
	int bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);

	if (fd == -1)
	{
		return (false);
	}

	bytes_written = snprintf(buffer, sizeof(buffer), "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
#else
	UNREFERENCED_PAR(pin);
#endif

	return (true);
}

/*****************************************************************************/
bool Gpio::Unexport(int pin)
/*****************************************************************************/
{
#if defined Q_OS_UNIX
	char buffer[32 + 1];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);

	if (fd == -1)
	{
		return (false);
	}

	bytes_written = snprintf(buffer, sizeof(buffer), "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
#else
	UNREFERENCED_PAR(pin);
#endif

	return (true);
}

/*****************************************************************************/
bool Gpio::Direction(int pin, DIRECTION dir)
/*****************************************************************************/
{
#if defined Q_OS_UNIX
	const char *dir_str;
	int        dir_length;
	char       path[64 + 1];
	int        fd;

	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);

	if (fd == -1)
	{
		return (false);
	}

	dir_str    = "";
	dir_length = 0;
	switch (dir)
	{
		case IN_DIR:
			dir_str    = "in";
			dir_length = 2;
			break;

		case OUT_DIR:
			dir_str = "out";
			dir_length = 3;
			break;

		default:
			ASSERT(false);
	}

	if (write(fd, dir_str, dir_length) == -1)
	{
		close(fd);
		return (false);
	}

	close(fd);
#else
	UNREFERENCED_PAR(pin);
	UNREFERENCED_PAR(dir);
#endif

	return (true);
}

/*****************************************************************************/
bool Gpio::Read(int pin)
/*****************************************************************************/
{
#if defined Q_OS_UNIX
	char path[64 + 1];
	char value_str[2 + 1];
	int  value;
	int  fd;


	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);

	if (fd == -1)
	{
		return (false);
	}

	if (read(fd, value_str, sizeof(value_str)) == -1)
	{
		close(fd);
		return (false);
	}

	close(fd);

	value = atoi(value_str);

	return (value != 0);
#else
	UNREFERENCED_PAR(pin);

	return (false);
#endif
}

/*****************************************************************************/
bool Gpio::Write(int pin, bool value)
/*****************************************************************************/
{
#if defined Q_OS_UNIX
	const char *value_str;

	char path[64 + 1];
	int fd;

	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);

	if (fd == -1)
	{
		return (false);
	}

	if (value)
	{
		value_str = "1";
	}
	else
	{
		value_str = "0";
	}

	if (write(fd, value_str, 1) == -1)
	{
		close(fd);
		return (false);
	}

	close(fd);
#else
	UNREFERENCED_PAR(pin);
	UNREFERENCED_PAR(value);
#endif

	return (true);
}

