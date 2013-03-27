// Original author Alex Markin (?)
// Licence LGPL
// http://code.google.com/p/qtgetopt/
//
// Modified by Michael Wiklund
//

#ifndef QGETOPT_H
#define QGETOPT_H

#include <QStringListIterator>
#include <QString>
#include <QChar>
#include <QList>

class QGetOpt
{
public:
	class Option;
	class Error;
	typedef QList<Option> OptionList;

	QGetOpt();
	~QGetOpt();

	void addOption(const Option &opt);
	const OptionList &options() const;

/**
  * returns false if program has no arguments at all
**/
	bool areArgumentsUsed() const;

/**
  * writes argument values in values
  * returns true, if argument used
  * else returns false if unused or argument doesn't exist
**/
	bool optionValues(QChar shortOpt, QStringList &values);
	bool optionValues(const QString &longOpt, QStringList &values);
/**
  * in case of only one possible argument value
**/
	bool optionValues(QChar shortOpt, QString &values);
	bool optionValues(const QString &longOpt, QString &values);

/**
  * searches option by name. Returns pointer to option, or NULL of option
  * with this name does't exists
**/
	const Option *findOpt(QChar shortOpt) const;
	const Option *findOpt(const QString &longOpt) const;

/**
  * enables arguments after "--" string. Default is true.
**/
	void enableArguments(bool b);
	bool areArgumentsEnabled() const;
	const QStringList &arguments() const;

/**
  * when all options are configured, start parse() to get them from command line
**/
	bool parse();

/**
  * you can use check() after parse() for checking correctness of given arguments
**/
	bool check();

    /// Update an option with its corresponding values i GetOpt, typically after a parse()
    Option &update(Option &opt) const;

	const Error *lastError() const;

	QGetOpt &operator << (const Option &opt);

private:
	OptionList _opts;
	QStringList _args;			// arguments after "--"
	bool _enArgs;
	Error *_err;
	bool _argsExist;
};

//========================= Option

class QGetOpt::Option
{
public:
/**
  * Creates option, that can be added to QGetOpt object
  * shortOpt and longOpt are aliases, that can be used as "-shortOpt" or "--longOpt"
  * maxValueNum is number of values that can be taken by this option.
  * If it is -1, there can be any number of values, if 0, zere is no values at all
**/
	Option(const QChar shortOpt, const QString &longOpt,
		   qint32 minValueNum = 0, qint32 maxValueNum = -1);

	QChar shortOption() const;
	const QString &longOption() const;

/**
  * returns const reference to the argument value list.
  * Canges only after calling parse().
**/
    const QStringList &values() const;
    QString value() const;

/**
  * shows if argument used in program. Canges only after calling parse().
  * the default is false
**/
    bool isUsed() const;
    void clearUsed();
	qint32 maxValues() const;
	qint32 minValues() const;

/**
  * gets argument values until the next argument
**/
	void getValues(QStringListIterator &i);
    void addValue(const QString &value);

private:

	QChar _shortOpt;
	QString _longOpt;
	qint32 _minValueNum, _maxValueNum;		// minimal and maximal values of arguments in this option
	QStringList _values;					// list of values, used for this argument
    bool _isUsed;
};

//========================= Error

class QGetOpt::Error
{
public:
	enum ErrorIndex
	{
		none = 0,				// no error
		notExist = 1,			// option doesn't exist
		fewValues = 2,			// too few values
		argsNotEnabled = 3,		// no arguments enabled
	};

	Error();

	void setErrorMessage(const QString &msg, ErrorIndex i, QGetOpt::Option *opt = NULL);

	QString errorMessage() const;
	ErrorIndex index() const;
	const QGetOpt::Option *option() const;

private:
	QString _msg;		// error message
	ErrorIndex _err;
	QGetOpt::Option *_opt;
};

#endif // QGETOPT_H
