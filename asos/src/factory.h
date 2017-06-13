#ifndef __factory_h
#define __factory_h

#include <QHash>

#include "filter.h"

class Factory
{
private:
	using factory_method = Filter *(*)();

	template<typename filter>
	static Filter * creator() { return new filter; }

	static QHash<QString, factory_method> factories;

public:
	static Filter *create(QString filter) {
		return factories[filter]();
	}
};

#endif
