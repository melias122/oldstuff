#include "factory.h"

QHash<QString, Factory::factory_method> Factory::factories= {
	{"Reverse", Factory::creator<Reverse>},
	{"Substitution", Factory::creator<Substitution>}
};
