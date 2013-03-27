// Original author Alex Markin (?)
// Licence LGPL
// http://code.google.com/p/qtgetopt/
//
// Modified by Michael Wiklund
//

#include "qgetopt.h"

#include <QCoreApplication>
#include <QListIterator>

#include <QDebug>

QGetOpt::QGetOpt()
	:_enArgs(true),
	 _argsExist(false)
{
	_err = new Error;
}

QGetOpt::~QGetOpt()
{

}

void QGetOpt::addOption(const Option &opt)
{
	_opts << opt;
}

const QGetOpt::OptionList &QGetOpt::options() const
{
	return _opts;
}

bool QGetOpt::areArgumentsUsed() const
{
	return _argsExist;
}

bool QGetOpt::optionValues(QChar shortOpt, QStringList &values)
{
	const Option *o = findOpt(shortOpt);

    if( o == NULL || !o->isUsed()) return false;

	values = o->values();
	return true;
}

bool QGetOpt::optionValues(const QString &longOpt, QStringList &values)
{
	const Option *o = findOpt(longOpt);

    if( o == NULL || !o->isUsed() ) return false;

	values = o->values();
	return true;
}

bool QGetOpt::optionValues(QChar shortOpt, QString &values)
{
	QStringList tmp;
	bool res = optionValues(shortOpt, tmp);
	values = tmp[0];
	return res;
}

bool QGetOpt::optionValues(const QString &longOpt, QString &values)
{
	QStringList tmp;
	bool res = optionValues(longOpt, tmp);
	values = tmp[0];
	return res;
}

const QGetOpt::Option *QGetOpt::findOpt(QChar shortOpt) const
{
	QListIterator<Option> i(_opts);

	while( i.hasNext() )
	{
		const Option &o = i.next();
		if( o.shortOption() == shortOpt )
			return &o;
	}

	return NULL;
}

const QGetOpt::Option *QGetOpt::findOpt(const QString &longOpt) const
{
	QListIterator<Option> i(_opts);

	while( i.hasNext() )
	{
		const Option &o = i.next();
		if( o.longOption() == longOpt )
			return &o;
	}

	return NULL;
}

void QGetOpt::enableArguments(bool b)
{
	_enArgs = b;
}

bool QGetOpt::areArgumentsEnabled() const
{
	return _enArgs;
}

const QStringList &QGetOpt::arguments() const
{
	return _args;
}


bool QGetOpt::parse()
{
	QStringList args = QCoreApplication::arguments();

	QStringListIterator arg(args);
	arg.next();						// skipping first argument (program name)
	if( arg.hasNext() ) _argsExist = true;

// loop for all arguments
	while( arg.hasNext() )
	{

		QString cur(arg.next());

// checking for arguments without any options
		if( cur.compare("--") == 0 || !args[1].startsWith('-') ) // if first argument is not option
		{
			if( !_enArgs )
			{
				_err->setErrorMessage("No arguments enabled", Error::argsNotEnabled);
				return false;
			}

			while( arg.hasNext() )
				_args << arg.next();
			return true;
		}

// searching for option
        int asignPos = cur.indexOf("=");
        QString curArg = cur.left( asignPos);
		QListIterator<Option> opts(_opts);
		bool exist = false;
		while( opts.hasNext() )
		{
			Option &o = const_cast<Option &>(opts.next());

            if( curArg.compare("-" + QString(o.shortOption())) == 0 || curArg.compare("--" + o.longOption()) == 0)
			{
                if (asignPos >= 0)
                    o.addValue( cur.mid( asignPos + 1));
				o.getValues(arg);
				exist = true;
			}
		}

		if( !exist )
		{
			_err->setErrorMessage("Option doesn't exist", Error::notExist);
			return false;
		}
	}

	return true;
}

bool QGetOpt::check()
{
	QListIterator<Option> opts(_opts);
	while( opts.hasNext() )
	{
		Option &o = const_cast<Option &>(opts.next());

        if( !o.isUsed() ) continue;

        if( ( o.values().size() < o.minValues() ) )
        {
            _err->setErrorMessage("Too few values", Error::fewValues, &o);
            return false;
        }
        if( ( o.values().size() > o.maxValues() && o.maxValues() != -1 ) )
        {
            _err->setErrorMessage("Too many values", Error::fewValues, &o);
            return false;
        }
    }

	return true;
}

QGetOpt::Option &QGetOpt::update(Option &opt) const
{
    const Option *optFound = findOpt(opt.longOption());  // Try to find long option first
    if (!optFound)
        optFound = findOpt(opt.shortOption());  // Otherwise try short option
    if (optFound) {
        opt = *optFound;
    }
    else {
        opt.clearUsed();
    }

    return opt;
}

const QGetOpt::Error *QGetOpt::lastError() const
{
	return _err;
}

QGetOpt &QGetOpt::operator << (const Option &opt)
{
	addOption(opt);
	return *this;
}

//======================================= Option

QGetOpt::Option::Option(const QChar shortOpt,
						const QString &longOpt,
						qint32 minValueNum,
						qint32 maxValueNum)
							:_shortOpt(shortOpt),
							 _longOpt(longOpt),
							 _minValueNum(minValueNum),
							 _maxValueNum(maxValueNum),
                             _isUsed(false)
{
}

QChar QGetOpt::Option::shortOption() const
{
	return _shortOpt;
}

const QString &QGetOpt::Option::longOption() const
{
	return _longOpt;
}

const QStringList &QGetOpt::Option::values() const
{
	return _values;
}

QString QGetOpt::Option::value()  const
{
    if (_values.isEmpty())  return QString();

    return _values.at(0);
}

bool QGetOpt::Option::isUsed() const
{
    return _isUsed;
}

void QGetOpt::Option::clearUsed()
{
    _isUsed = false;
    _values.clear();
}


qint32 QGetOpt::Option::maxValues() const
{
	return _maxValueNum;
}

qint32 QGetOpt::Option::minValues() const
{
	return _minValueNum;
}

void QGetOpt::Option::getValues(QStringListIterator &i)
{
    _isUsed = true;

	while( i.hasNext() )
	{
		QString str(i.next());
		if( str.startsWith('-') )
		{
			i.previous();
			break;
		}

		_values << str;
	}
}

void QGetOpt::Option::addValue(const QString &value)
{
    _values << value;
}

//======================================= Error

QGetOpt::Error::Error()
	:_err(none),
	 _opt(NULL)
{
}

void QGetOpt::Error::setErrorMessage(const QString &msg, ErrorIndex i,
									 QGetOpt::Option *opt)
{
	_msg = msg;
	_err = i;
	_opt = opt;
}

QString QGetOpt::Error::errorMessage() const
{
	return _msg;
}

QGetOpt::Error::ErrorIndex QGetOpt::Error::index() const
{
	return _err;
}

const QGetOpt::Option *QGetOpt::Error::option() const
{
	return _opt;
}
